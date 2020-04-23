#ifndef REG_H
#define REG_H

#include <stdio.h>      
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

struct Reg {
    pid_t pid;
    double instant;
    pthread_t tid;
};

struct Info{
    int i; //o número sequencial do pedido (gerado por Un)
    pid_t pid;//dentificador de sistema do processo (cliente, no caso do pedido; servidor, no caso da resposta)
    pthread_t tid; //dentificador no sistema do  thread cliente (cliente, no caso do pedido; servidor, no caso da resposta)
    int dur; /*duração, em milissegundos, de utilização (de um lugar) do Quarto de Banho (valor atribuídono pedido e repetido na resposta, se se der a ocupação; se não se der, por motivo de o Quarto deBanho estar em vias de encerrar, o servidor responde aqui com o valor -1*/
    int pl;/*duração, em milissegundos, de utilização (de um lugar) do Quarto de Banho (valor atribuídono pedido e repetido na resposta, se se der a ocupação; se não se der, por motivo de o Quarto deBanho estar em vias de encerrar, o servidor responde aqui com o valor -1*/

};
void initExecReg();
void fillReg(struct Reg *reg);
void regOper();
void closeReg();

#endif /*REG_H*/