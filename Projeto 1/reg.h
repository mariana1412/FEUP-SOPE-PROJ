#ifndef REG_H
#define REG_H

#include <stdio.h>      
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


struct Reg {
    pid_t pid;
    double instant;
};


void initExecReg();
void fillReg(struct Reg *reg);
void regCreate(int argc, char *argv[]);
void regExit(int exitStatus);
void regRecvSignal(int signal);
void regSendSignal(int signal, pid_t pid);
void regRecvPipe(char* message);
void regSendPipe(char* message);
void regEntry();

#endif /*REG_H*/