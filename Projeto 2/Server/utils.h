#ifndef REG_H
#define REG_H

#include <stdio.h>      
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

void start_time();
double timePassed();
void regOper();

#endif /*REG_H*/