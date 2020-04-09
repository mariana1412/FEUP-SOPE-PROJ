#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "macros.h"
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <limits.h>
#include <linux/limits.h>

struct ArgumentFlags{
    int all; // can be either -a or -all, displays also the information about files
    int bytes; // can be either -b or -bytes, shoes the real number of bytes of data(files) or alocated (directories)
    int blockSize; // can be either -B or --block-size=SIZE, defines the size(bytes) of the block to representation effects
    int countLinks; // can be either -l or --count-links, counts multiple times the same file
    int simbolicLinks; // can be either -L or --dereference, follows the simbolic links
    int noSubDir; // can be either -S or --separate-dirs, the displayed info does not include the size of subdirectories
    int maxDepth; // --max-depth=N, limits the displayed info to N (0,1,..) levels of directories depth
    char *path;// path of 
};
int idArguments(char *arg);
int parseArguments(int argc, char *argv[], struct ArgumentFlags *args);
int verifyPath(char * path);
void initArgumentFlags(struct ArgumentFlags *args);
void checkFlags(struct ArgumentFlags* args);
struct stat getStat();
void getArgv(char* dirpath, struct ArgumentFlags *args, char*res[]);

#endif /*ARGS_H*/