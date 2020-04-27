#include "utils.h"

//struct timespec startTime;
//struct timespec currentTime;
 struct timeval beginTime;
struct timeval currentTime;
double elapsed;
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


void start_time(){
    gettimeofday(&beginTime, NULL);
    elapsed = 0;   
}
double timePassed(){
    gettimeofday(&currentTime, NULL);

    elapsed = (currentTime.tv_sec - beginTime.tv_sec) * 1e6;
    elapsed = (elapsed + (currentTime.tv_usec - beginTime.tv_usec)) * 1e-6;

    return elapsed;

}



void regOper(char * oper, int i, pid_t pid, pthread_t tid, int dur, int pl){
    double instant = timePassed();
    fprintf(stdout, "%2f ; %d ; %d ; %ld ; %d ; %d ; %s\n", instant, i, pid, tid, dur, pl, oper);
    fflush(stdout);
}



    /*clock_gettime(CLOCK_MONOTONIC, &currentTime);
    double secsPassed = (currentTime.tv_sec - startTime.tv_sec);
    double nanoToSecPassed = (currentTime.tv_nsec - startTime.tv_nsec)/10e6;

    return secsPassed + nanoToSecPassed;*/

    //clock_gettime(CLOCK_MONOTONIC, &startTime);
    //CLOCK_MONOTONIC -> Clock that cannot be set and represents monotonic time since some unspecified starting point.
