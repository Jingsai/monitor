// monitor.h

#ifndef MONITOR_H
#define MONITOR_H

#include <pthread.h>

// definition of a monitor
typedef struct monitor
{
    int num_ranks; // number of threads
    int num_in_q; // number of threads in the waiting queue
    pthread_mutex_t mutex; // mutex
    pthread_cond_t queue; // condition
    pthread_t* threads; // threads
}monitor_t;

// initialization 
int mon_init(monitor_t *m, int num_ranks_participating);

// enter the monitor, set the mutex
int mon_enter(monitor_t *m);

// exit the monitor, return the mutex
int mon_exit(monitor_t *m);

// exit the monitor, signal the threads in the 
// waiting queue and return the mutex
int mon_continue(monitor_t *m);

// add the thread in the waiting queue
int mon_wait_in_q(monitor_t *m);

// return the number of threads in the waiting queue
int mon_num_in_q(monitor_t *m);

// destory the monitor
int mon_destroy(monitor_t *m);

#endif


