//File:    queue.h
//
//This file contains the definition of the queue class implemented using

#ifndef QUEUE_H
#define QUEUE_H

typedef struct node   //a node in the Queue has this structure
{
    int data[2];
    struct node* next;
}Node;

typedef Node* NodePtr;   //pointer to a node in the Queue

typedef struct
{
    NodePtr frontPtr;
    NodePtr rearPtr;
}Queue;

// default constructor
Queue* InitQueue();

//queue test method
int IsEmpty(Queue* queue);

// queue access methods 
void getQueue(Queue* queue, int item[2]);
void putQueue(Queue* queue, int item[2]);

#endif

