#include "reg.h"

static clock_t startTime;
static FILE *regFile;

void initExecReg(){
    char *reg; 

    startTime = clock();
    reg = getenv("LOG_FILENAME");

    if(reg!= NULL) {
        char* parentpid = getenv("MAIN_PID");
        char pid[10];
        sprintf(pid, "%d", getpid());
        if(strcmp(pid, parentpid)==0) {
            regFile = fopen(reg, "w");
            fclose(regFile);
        }
        if((regFile = fopen(reg,"a"))==NULL){
            perror("Error: Could not open the file \n");
            exit(1);
        }
    }
    
}

void fillReg(struct Reg *reg){
    reg->instant = ((double) (clock() - startTime)) / (CLOCKS_PER_SEC/1000.0);
    reg->pid = getpid();
}   

void regCreate(int argc, char *argv[]){
    struct Reg reg;
    char *info = malloc(sizeof(char)*256);

    if(regFile == NULL) return;

    fillReg(&reg);

    for (int i = 0; i < argc - 1; i++) {
        strcat(info, argv[i]);
        strcat(info, " ");
    }

    strcat(info, argv[argc-1]);        
    fprintf(regFile, "%.2f - %.8d - CREATE - %s\n", reg.instant, reg.pid, info);
    free(info);
}

void regExit(int exitStatus){
    struct Reg reg;

    if(regFile == NULL) return;

    fillReg(&reg);
    fflush(regFile);
    fprintf(regFile, "%.2f - %.8d - EXIT - %d\n", reg.instant, reg.pid, exitStatus);
    fclose(regFile);
}


void regRecvSignal(int signal){
    struct Reg reg;
    if(regFile == NULL) return;

    fillReg(&reg);
    fflush(regFile);
    fprintf(regFile, "%.2f - %.8d - RECV_SIGNAL - %d\n", reg.instant, reg.pid,signal);
}

void regSendSignal(int signal, pid_t pid){
    struct Reg reg;
    if(regFile == NULL) return;

    fillReg(&reg);
    fflush(regFile);
    fprintf(regFile, "%.2f - %.8d - SEND_SIGNAL - Signal %d to % d\n", reg.instant, reg.pid,signal, pid);
}

void regRecvPipe(char* message){
    struct Reg reg;
    if(regFile == NULL) return;
    fillReg(&reg);
    fflush(regFile);
    fprintf(regFile, "%.2f - %.8d - RECV_PIPE - %s\n", reg.instant, reg.pid, message);
}

void regSendPipe(char* message){
    struct Reg reg;
    if(regFile == NULL) return;
    
    fillReg(&reg);
    fflush(regFile);
    fprintf(regFile, "%.2f - %.8d - SEND_PIPE - %s\n", reg.instant, reg.pid, message);
}

void regEntry(int blocks, char* path){
    struct Reg reg;
    char *info = malloc(sizeof(char)*256);
    
    if(regFile == NULL) return;

    sprintf(info, "%d", blocks);
    strcat(info, " ");
    strcat(info, path);
    fillReg(&reg);
    fflush(regFile);
    fprintf(regFile, "%.2f - %.8d - ENTRY - %s\n", reg.instant, reg.pid, info);
    free(info);
}

