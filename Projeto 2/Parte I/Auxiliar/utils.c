#include "utils.h"

void regOper(char * oper, int i, pid_t pid, pthread_t tid, int dur, int pl, double instant){
    fprintf(stdout, "%.2f ; %d ; %d ; %ld ; %d ; %d ; %s\n", instant, i, pid, tid, dur, pl, oper);
    fflush(stdout);
}
