#ifndef Q1_H
#define Q1_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "args.h"
#include "utils.h"

#define MAX_MSG_LEN 500
#define NUM_MAX_THREADS 500 //fuck my life

void *thr_func(void *fname);
int main(int argc, char *argv[]);


#endif /*Q1_H*/
