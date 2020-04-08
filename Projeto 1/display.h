#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <dirent.h> 
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "args.h"
#include "macros.h"
#include "reg.h"
#include <wait.h>
#include <errno.h>

int bytesToBlocks(int blocks, int blocksize);
void buildPath(char * dirname, struct ArgumentFlags * args, char * aux);
void display(struct ArgumentFlags *args);
int forkAux(struct ArgumentFlags * args, char * path,int n);
void print(struct ArgumentFlags *args, int size, char* name, int type);


#endif /*DISPLAY_H*/