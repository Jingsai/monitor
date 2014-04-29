// monitor.c

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "monitor.h"

// initialization 
int mon_init(monitor_t *m, int num_ranks_participating)
{
    // initialize the threads
    pthread_t* threads;
    threads = (pthread_t*)malloc(sizeof(pthread_t)*num_ranks_participating);
    m->threads = threads;
    // number of threads
    m->num_ranks = num_ranks_participating;
    // nubmer of threads in the waiting queue
    m->num_in_q = 0;
    // mutex
    pthread_mutex_init(&(m->mutex),NULL);
    // condition variable
    pthread_cond_init(&(m->queue), NULL);

    return 0;
}

// enter the monitor, set the mutex
int mon_enter(monitor_t *m)
{
    pthread_mutex_lock(&(m->mutex));
    return 0;
}

// exit the monitor, return the mutex
int mon_exit(monitor_t *m)
{
    pthread_mutex_unlock(&(m->mutex));
    return 0;
}

// exit the monitor, signal the threads in the 
// waiting queue and return the mutex
int mon_continue(monitor_t *m)
{
    // if some threads in the queue, release one of them
    if(m->num_in_q)
        m->num_in_q--;
    pthread_cond_signal(&(m->queue));
    pthread_mutex_unlock(&(m->mutex));
    return 0;
}

// add the thread in the waiting queue
int mon_wait_in_q(monitor_t *m)
{
    // increase the number of threads in the queue
    m->num_in_q++;
    pthread_cond_wait(&(m->queue),&(m->mutex));
    return 0;
}

// return the number of threads in the waiting queue
int mon_num_in_q(monitor_t *m)
{
    return m->num_in_q;
}

// destory the monitor
int mon_destroy(monitor_t *m)
{
    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->queue));
    free(m->threads);
    return 0;
}




