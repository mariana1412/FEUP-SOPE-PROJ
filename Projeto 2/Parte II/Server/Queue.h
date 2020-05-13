#ifndef Queue_H
#define Queue_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct Queue{
    unsigned front, rear, size , capacity;
    unsigned *queue;
    
};
void createQueue(unsigned capacity, struct Queue* q);
int peek(struct Queue* q);
int isEmpty(struct Queue* q);
int isFull(struct Queue* q);
int size(struct Queue* q);
void insert(int data, struct Queue* q);
int removeData(struct Queue *q);
void printQueue(struct Queue *q);



#endif /*Queue_H*/
