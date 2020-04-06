#include "reg.h"

static clock_t startTime;
static FILE *regFile;

void initExecReg(){
    char *reg; 
        
    startTime = clock();

    setenv("LOG_FILENAME","reg.txt", 0); 
    reg = getenv("LOG_FILENAME");
    /*caso a variavel nao for definida , nao regista nada */
    if((regFile = fopen(reg,"a"))==NULL){
        perror("Error: Could not open the file \n");
        exit(1);//nao sei se queremos isto
    }
 
}

void fillReg(struct Reg *reg){
    reg->instant = ((double) (clock() - startTime)) / (CLOCKS_PER_SEC/1000.0);
    reg->pid = getpid();
}   

void regCreate(int argc, char *argv[]){
   struct Reg reg;
   fillReg(&reg);
   char *info = malloc(sizeof(char)*256);

   for (int i = 0; i < argc - 1; i++) {
       strcat(info, argv[i]);
       strcat(info, " ");
    }
    strcat(info, argv[argc-1]);
    fprintf(regFile, "%.2f - %.8d - CREATE - %s\n", reg.instant, reg.pid, info);
}

void regExit(int exitStatus){
    struct Reg reg;

    fillReg(&reg);
    fprintf(regFile, "%.2f - %.8d - EXIT - %d\n", reg.instant, reg.pid, exitStatus);
}


void regRecvSignal(int signal){
    struct Reg reg;

    fillReg(&reg);
    fprintf(regFile, "%.2f - %.8d - RECV_SIGNAL - %d\n", reg.instant, reg.pid,signal);
}

void regSendSignal(int signal, pid_t pid){
    struct Reg reg;
    fillReg(&reg);
    fprintf(regFile, "%.2f - %.8d - SEND_SIGNAL - Signal %d to % d\n", reg.instant, reg.pid,signal, pid);
}

void regRecvPipe(char* message){
    struct Reg reg;
    fillReg(&reg);
    fprintf(regFile, "%.2f - %.8d - RECV_PIPE - %s\n", reg.instant, reg.pid, message);
}

void regSendPipe(char* message){
    struct Reg reg;

    fillReg(&reg);
    fprintf(regFile, "%.2f - %.8d - SEND_PIPE - %s\n", reg.instant, reg.pid, message);
}

void regEntry(){//do not know número de bytes (ou blocos) seguido do caminho.
    struct Reg reg;
    
    fillReg(&reg);
    fprintf(regFile, "%.2f - %.8d - ENTRY - \n", reg.instant, reg.pid);
}

