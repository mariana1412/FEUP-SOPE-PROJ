#ifndef U1_H
#define U1_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <pthread.h> 
#include <errno.h>
#include <sys/time.h>
#include "../Auxiliar/args.h"
#include "../Auxiliar/utils.h"



#define MAX_MSG_LEN 255
#define NUM_MAX_THREADS 4096

void *thr_func(void *fname);
void alarm_handler(int sig);
int main(int argc, char *argv[]);

#endif /*U1_H*/
