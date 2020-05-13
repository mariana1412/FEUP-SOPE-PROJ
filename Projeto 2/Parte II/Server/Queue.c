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

// function to insert an element at the rear of the queue 
void insert(int data, struct Queue* q) {
   printf("está livre %d\n", data);
   if(!isFull(q)) {
	
      if(q->rear == q->capacity - 1 ) {
         q->rear = -1;            
      }       
       // insert element at the rear
      q->queue[++q->rear] = data;
      q->size++;
   }  
}
 // function to delete an element from the front of the queue 
int removeData(struct Queue *q) {
   if(isEmpty(q))//if the queue is empty 
        return 0;

   int data = q->queue[q->front];
   if(q->front == q->capacity) {
      q->front = 0;
   }
	else{// shift all the elements from index 2 till rear to the left by one 
      for (int i = 0; i < q->rear; i++) { 
         q->queue[i] = q->queue[i + 1]; 
      } 
  
   }
   q->size--;
   q->rear--;
   printf("estou na casa de banho %d\n", data);
   
   return data;  
}

// print queue elements 
void printQueue(struct Queue *q){
   for(int i = 0;i < q->size; i++){
      printf("-> %d: %d\n", i, q->queue[i]);
   }
   if(isEmpty(q)){
      printf("You got it empty\n");
   }
}