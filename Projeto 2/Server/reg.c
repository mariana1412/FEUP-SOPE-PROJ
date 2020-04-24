#include "reg.h"

static clock_t startTime;
static FILE *regFile;

/*
“inst;i ; pid ; tid ; dur ; pl ; oper”
○ i , pid , tid , dur, pl– têm o mesmo significado que os campos apresentados acima
○ inst - valor retornado pela chamada ao sistema time(), na altura da produção da linha
○ oper – siglas de 5 letras ajustadas às fases da operação que cada processo/thread acabou de executar 
e que variam conforme se trate do cliente ou do servidor: 
■ IWANT- cliente faz pedido inicial
■ RECVD- servidor acusa receção de pedido
■ ENTER- servidor diz que aceitou pedido
■ IAMIN- cliente acusa a utilização do Quarto de Banho
■ TIMUP- servidor diz que terminou o tempo de utilização
■ 2LATE- servidor rejeita pedido por Quarto de Banho já ter encerrado
■ CLOSD- cliente acusa informação de que o Quarto de Banho está fechado
■ FAILD- cliente já não consegue receber resposta do servidor
■ GAVUP- servidor já não consegue responder a pedido porque FIFO privado do cliente fechou
*/

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
        if((regFile = fopen(reg,"a"))==NULL){//open in append mode
            perror("Error: Could not open the file \n");
            exit(1);
        }
    }
    
}

void fillReg(struct Reg *reg){
    reg->instant = ((double) (clock() - startTime)) / (CLOCKS_PER_SEC/1000.0);
    reg->pid = getpid();
    reg->tid = pthread_self();
}   

void regOper(char * oper, int i,int dur, int pl){
    struct Reg reg;
    struct Info info;

    if(regFile == NULL) return;

    fillReg(&reg);

    info.i = i; 
    info.pid = reg.pid;
    info.tid = reg.tid;
    info.dur = dur;
    info.pl = pl;

    fprintf(regFile, "%2f ; %8d ; %8d ; %ld ; %8d ; %8d ; %s",reg.instant, info.i, info.pid, info.tid, info.dur, info.pl, oper);
    fflush(regFile);
}

void closeReg(int exitStatus){
    fclose(regFile);
}


