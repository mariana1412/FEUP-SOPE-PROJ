#include "utils.h"


clock_t beginTime, beginTimeU; 

void start_time(){
    beginTime = clock();
}

void start_timeU(){
    beginTimeU = clock();
}

double timePassed(int server){

    if(server)
        return (double)(clock() - beginTime)/CLOCKS_PER_SEC;
    
    return (double)(clock() - beginTimeU)/CLOCKS_PER_SEC;
}


void regOper(char * oper, int i, pid_t pid, pthread_t tid, int dur, int pl){
    double instant = timePassed(1);
    fprintf(stdout, "%2f ; %d ; %d ; %ld ; %d ; %d ; %s\n", instant, i, pid, tid, dur, pl, oper);
    fflush(stdout);
}
