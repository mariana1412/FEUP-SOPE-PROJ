#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <dirent.h> 
#include <errno.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <wait.h>
#include <errno.h>
#include "signals.h"
#include "args.h"
#include "macros.h"
#include "reg.h"

int bytesToBlocks(int blocks, int blocksize);
int toBlocks(int bytes, int blocksize);
void buildPath(char * dirname, struct ArgumentFlags * args, char * aux);
int forkAux(struct ArgumentFlags * args, char * path,int n);
void print(struct ArgumentFlags *args, int size, char* name, int type);
void printFile(struct ArgumentFlags *args, struct stat stat_entry, int *size, char fullpath[], int simblink);
void printDir(struct ArgumentFlags *args, struct stat stat_entry, struct dirent *dentry, int *size, char fullpath[], int simblink);
void display(struct ArgumentFlags *args);


#endif /*DISPLAY_H*/