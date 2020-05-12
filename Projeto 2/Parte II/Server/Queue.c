#include "Queue.h"


void createQueue(unsigned capacity, struct Queue* q){
    q->capacity = capacity;
    q->size = 0;
    q->front = 0;
    q->rear = capacity - 1;
    q->queue = (unsigned*) malloc(sizeof(unsigned)*capacity);

    for (int i = 1; i <= q->capacity; i++) {
        insert(i, q);
    }
}

int peek(struct Queue* q) {
   return q->queue[q->front];
}

int isEmpty(struct Queue* q) {
   return q->size == 0;
}

int isFull(struct Queue* q) {
   return q->size == q->capacity;
}

int size(struct Queue* q) {
   return q->size;
}  

void insert(int data, struct Queue* q) {
   printf("está livre %d\n", data);
   if(!isFull(q)) {
	
      if(q->rear == q->capacity - 1 ) {
         q->rear = -1;            
      }       

      q->queue[++q->rear] = data;
      q->size++;
   }
}

int removeData(struct Queue *q) {
   if(isEmpty(q))
        return 0;

   int data = q->queue[q->front++];
   if(q->front == q->capacity) {
      q->front = 0;
   }
	
   q->size--;
   printf("estou na casa de banho %d\n", data);
   
   return data;  
}