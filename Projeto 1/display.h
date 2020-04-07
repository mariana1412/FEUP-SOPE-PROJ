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
#include <wait.h>

int bytesToBlocks(int bytes, int blocksize);
//void buildPath(char* dest, char * dirname, struct ArgumentFlags * args) 
char * buildPath(char * dirname, struct ArgumentFlags * args);
void display(struct ArgumentFlags *args);

void printFile(struct ArgumentFlags * args, int size, char* name);
void printDir(struct ArgumentFlags * args, int size, char* name);
void printLink(struct ArgumentFlags * args, int size, char* name);


#endif /*DISPLAY_H*/