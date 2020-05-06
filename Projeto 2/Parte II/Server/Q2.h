#ifndef Q2_H
#define Q2_H
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
#include "../Auxiliar/args.h"
#include "../Auxiliar/utils.h"

#define MAX_MSG_LEN 500
#define NUM_MAX_THREADS 4096

void *thr_funcStandard(void *fname);
void *thr_funcClosed(void *msgCl);
int main(int argc, char *argv[]);


#endif /*Q2_H*/
