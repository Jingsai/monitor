#
CC = gcc
CFLAGS = -g
LIBS = -lm 

queen.o:  queen.c
	$(CC) -c queen.c

queue.o:  queue.c
	$(CC) -c queue.c

monitor.o:  monitor.c
	$(CC) -c monitor.c

p4:  p4.c queen.o queue.o monitor.o
	$(CC) $(CFLAGS) -o p4 queen.o queue.o monitor.o p4.c -lpthread $(LIBS)

clean:
	rm -f p4 *.o
