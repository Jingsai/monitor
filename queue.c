//File:   queue.c
//
//This file contains the implementation of a Queue class using a linked list.

#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

// This is the default constructor.
Queue* InitQueue()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->frontPtr = NULL;
    queue->rearPtr  = NULL;
    return queue;
}

//tests for an empty queue.
int IsEmpty(Queue *queue) 
{
    return  queue->frontPtr == NULL;
} 

// This function gets an element from the front of the queue.
void getQueue(Queue *queue, int item[2])
{
  if (~IsEmpty(queue))
   { 
       NodePtr oldPtr = queue->frontPtr;
       queue->frontPtr = queue->frontPtr->next;
       item[0] = oldPtr->data[0];
       item[1] = oldPtr->data[1];
       free(oldPtr);
   }
} 

// This function adds an element to the rear of the queue.  
void putQueue(Queue *queue, int item[2])
{
    NodePtr newPtr = (NodePtr)malloc(sizeof(Node));
    newPtr->data[0] = item[0];
    newPtr->data[1] = item[1];
    newPtr->next = NULL;
    
    if (IsEmpty(queue))      //Was the queue empty?
    {
        queue->frontPtr = newPtr;
        queue->rearPtr = newPtr; 
    }
    else
    {
        queue->rearPtr->next = newPtr;
        queue->rearPtr = newPtr;
    }
}

