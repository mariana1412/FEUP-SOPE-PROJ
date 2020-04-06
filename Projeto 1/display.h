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
char * buildPath(char * dirname);
void display(struct ArgumentFlags *args);


#endif /*DISPLAY_H*/