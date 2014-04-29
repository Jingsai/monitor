#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "queen.h" // solve N-queen problem
#include "queue.h" // support queue operation
#include "monitor.h" // monitor implement

#define true 1
#define false 0
// jobfalg
#define FindJob 0 // try to find one job
#define HasJob 1 // get a job to do
#define NoJob 3 // all jobs have done
// shared variable
int TotalNum = 0; // total solution number
int row = 0; // initial row number
int col = 0; // initial column number
Queue *jobqueue; // job queue
int jobnum = 0; // number of jobs which have done
int done = false; // flag is true if all jobs done
// constant
int N; // size of board
int ifPrint = true; // if print out solution
int JobSize = 1; // divide job as (N-2)*(N-2)
int TotalJobNum = 0; // total job number
// params for theads
struct ThreadParams{
    int **board; // board of the problem
    int num; // solution number found in one time call solveNQ
    int jobnum; // is one if finishing one job
    int job[2]; // the job 
    int jobflag; // job status, check above define
    int rank;  // rank of this thread
};
// threads func
void* threads_func(void*);
// moitor
monitor_t m;

int main(int argc, char* argv[])
{
    int i,j,k;
    int nprocs;
    if (argc < 5)
    {
        printf("Please use -n -p\n");
        return 0;
    }
    for (i=1; i<argc; i++)
    {
        // size of problem
        if(!strcmp(argv[i],"-n"))
            N = atoi(argv[i+1]);
        // number of threads
        if(!strcmp(argv[i],"-p"))
            nprocs = atoi(argv[i+1]);
        // input q if not print
        if(!strcmp(argv[i],"-q"))
            ifPrint = false;
    }    
    
    // TotalJobNum
    if (N==1)
        TotalJobNum = 1;
    else if (N < 4)
        TotalJobNum = 0;
    else
        TotalJobNum = N*N-3*N+2; // 2*(N-2)+(N-3)*(N-2)

    // initialization of monitor and job queue
    mon_init(&m, nprocs);
    jobqueue = InitQueue();

    // initialization of params
    struct ThreadParams params[nprocs];
    for (i=0; i<m.num_ranks; i++)
    {
        // every thread of its own board
        int ** board;
        board = (int**)malloc(sizeof(int*)*N);
        board[0] = (int*)malloc(sizeof(int)*N*N);
        for(j=1; j<N; j++)
                board[j] = board[j-1]+N;
        for(j=0; j<N; j++)
            for(k=0; k<N; k++)
                board[j][k] = 0;
        
        params[i].board = board;
        params[i].num = 0;
        params[i].jobnum = 0;
        params[i].job[0] = 0;
        params[i].job[1] = 0;
        params[i].jobflag = FindJob;
        params[i].rank = i;
    }

    // create the threads
    for (i=0; i<m.num_ranks; i++)
        pthread_create(&(m.threads[i]),NULL,threads_func,(void*)&params[i]);
    // join the threads
    for (i=0; i<m.num_ranks; i++)
        pthread_join(m.threads[i],NULL);

    // print out the result
    printf("Solution Number of %d Queen Problem is %d\n", N, TotalNum); 

    // free and quit
    for (i=0; i<m.num_ranks; i++)
    {
        free(params[i].board[0]);
        free(params[i].board);
    }
    mon_destroy(&m);
    return 0;
}
 
/* 
    Threads funtion for parallel computing of N-queen problem.

    Use back tracking algorithm in serial when each single thread 
    does the job. 
    Divide the whole problem into jobs with size (N-2)*(N-2). Fix
    two queens in the first two columns, calculate the reaminding
    part.

    all threads enter the monitor one by one, pick up one job or
    create some jobs. if all jobs done but not post yet, now thread
    which enter the monitor go to the waiting queue. these threads will
    exit the monitor one by one when signal sent by the thread which
    finds all jobs done and posts the signal OR signal by new jobs.
*/
void* threads_func(void* s)
{
    struct ThreadParams *p = s;
    int *pnum = &(p->num);

    int i,j;
    int job[2]; // job array for get&put in the queue
    
    // continue the loop if all jobs have not done
    while(!done)
    {
        p->jobflag = FindJob;
        // continue the loop if thread is trying to finding job
        // stop if thread has a job or all jobs done
        while(p->jobflag == FindJob)
        {

            // thread enters the monitor
            mon_enter(&m);
                // total solution
                if(p->num != 0)
                {
                    TotalNum += p->num;
                    p->num = 0; // reset to 0 for next call
                }
                // use total number of jobs to check if all jobs done 
                if(p->jobnum == 1)
                {
                    jobnum++;
                    p->jobnum = 0;
                }
                // done
                if(jobnum == TotalJobNum)
                {
                    done = true;
                    p->jobflag = NoJob;
                    mon_continue(&m);
                }
                // not done, has job
                else if (!IsEmpty(jobqueue))
                {
                    getQueue(jobqueue, p->job);
                    p->jobflag = HasJob;
                    mon_exit(&m);
                }
                // not done, create job
                else if (row < N)
                {
                    // create the jobs for this point
                    if(N == 1)
                    {
                        job[0] = row;
                        job[1] = row;
                        putQueue(jobqueue, job);
                    }
                    for(i=0; i<=row-2; i++)
                    {
                        job[0] = row;
                        job[1] = i;
                        putQueue(jobqueue, job);
                    }
                    for(i=row+2; i<N; i++)
                    {
                        job[0] = row;
                        job[1] = i;
                        putQueue(jobqueue, job);
                    }
                    row++;
                    // if has job, pick up one
                    // N=2, no job created, just exit
                    if (N>2)
                    {
                        getQueue(jobqueue, p->job);
                        p->jobflag = HasJob;
                        // if some threads in the waiting list, signal them
                        // otherwise, just exit
                        if(mon_num_in_q(&m))
                            mon_continue(&m);
                        else
                            mon_exit(&m);
                    }
                    else
                        mon_exit(&m);
                }
                // not done, nothing to do, go to waiting queue
                else
                {
                    mon_wait_in_q(&m);
                    // after wake up
                    if(done) // all jobs have done
                    {
                        p->jobflag = NoJob;
                        mon_continue(&m);
                    }
                    else if(!IsEmpty(jobqueue)) // has a job to do
                    {
                        getQueue(jobqueue, p->job);
                        p->jobflag = HasJob;
                        // if some threads in the waiting list, signal them
                        // otherwise, just exit
                        if(mon_num_in_q(&m))
                            mon_continue(&m);
                        else
                            mon_exit(&m);
                    }
                }

            //exit the monitor

        } // if job status is HasJob or NoJob, break 

        // do the job
        if(p->jobflag == HasJob)
        {
            solveNQ(p->board, p->job[0], p->job[1], JobSize, pnum, N, ifPrint);
            p->jobnum=1;
        }
    }
}


