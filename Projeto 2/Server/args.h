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
#define FIFONAME    2

struct ArgumentFlags{
    int nsecs;
    char* fifoname;
};

int idArguments(char *arg);
int parseArguments(int argc, char *argv[], struct ArgumentFlags *args);
void initArgumentFlags(struct ArgumentFlags *args);
void checkFlags(struct ArgumentFlags* args);
//struct stat getStat();
void getArgv(char* dirpath, struct ArgumentFlags *args, char*res[]);

#endif /*ARGS_H*/