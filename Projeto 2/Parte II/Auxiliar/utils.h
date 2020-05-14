#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>      
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>


void regOper(char * oper, int i, pid_t pid, pthread_t tid, int dur, int pl, double instant);

#endif /*UTILS_H*/