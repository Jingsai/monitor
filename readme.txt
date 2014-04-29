monitor
=======

A C/C++ program that uses pthreads to solve the nqueens problem.  This program is a means to demonstrate correct implementation of the concept of monitors. Primitive monitor operations is provided that together provide a complete framework.  For example:
    monitor_t m;
    mon_init(&m,num_ranks_participating);
    mon_enter(&m);
    mon_exit(&m);
    mon_continue(&m);
    mon_wait_in_q(&m);
    mon_num_in_q(&m);

Command-line arguments:
    -n  size of board (N)
    -q  quiet (prints number of solutions found but *not* the solutions)
    -p  number of processes or threads to do the work (may be 1 - 16)

