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
#include "../Server/args.c"
#include "../Server/utils.c"



#define MAX_MSG_LEN 255
#define NUM_MAX_THREADS 500

void *thr_func(void *fname);
int main(int argc, char *argv[]);

#endif /*U1_H*/
