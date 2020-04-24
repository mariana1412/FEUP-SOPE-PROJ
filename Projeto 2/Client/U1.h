#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <pthread.h> 
#include <sys/time.h>
#include "../Server/args.c"
#include "../Server/reg.c"

#define MAX_MSG_LEN 500
#define NUM_MAX_THREADS 2

int getI();
void *thr_func(void *fname);
int main(int argc, char *argv[]);