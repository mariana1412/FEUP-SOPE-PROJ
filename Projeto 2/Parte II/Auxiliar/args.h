#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <limits.h>
#include <linux/limits.h>

#define NSECS       1
#define NPLACES     2
#define NTHREADS    3
#define FIFONAME    4

struct ArgumentFlags{
    int nsecs;
    int nplaces;
    int nthreads;
    char* fifoname;
};

int isNumber(char *n);
int idArguments(char *arg);
int parseArgumentsClient(int argc, char *argv[], struct ArgumentFlags *args);
int parseArgumentsServer(int argc, char *argv[], struct ArgumentFlags *args);
void initArgumentFlags(struct ArgumentFlags *args);
void checkFlags(struct ArgumentFlags* args);

#endif /*ARGS_H*/