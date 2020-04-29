#include "utils.h"


static clock_t beginTime; 

void start_time(){
    beginTime = clock();
}

double timePassed(){
    return (double)(clock() - beginTime)/CLOCKS_PER_SEC;
}


void regOper(char * oper, int i, pid_t pid, pthread_t tid, int dur, int pl){
    double instant = timePassed();
    fprintf(stdout, "%2f ; %d ; %d ; %ld ; %d ; %d ; %s\n", instant, i, pid, tid, dur, pl, oper);
    fflush(stdout);
}
