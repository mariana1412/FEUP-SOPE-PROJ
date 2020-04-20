#include "U1.h"

/*
● é invocado com o comando
    ○ Un <-t nsecs> fifoname○nsecs - nº (aproximado) de segundos que o programa deve funcionar
    ○ fifoname – nome do canal público (FIFO) para comunicação com o servidor
● inicialmente,recolhe os argumentos de linha de comando e executa as operações fundamentais para o 
funcionamento do programa;
● lança continuamente (i.e. com intervalos de alguns milissegundos) threads, cada um ficando associado a 
um pedido;
● cada thread gera aleatoriamente parâmetros do pedido pelo qual é responsável (especificamente, a duração
 doacesso), trata de toda a comunicação com o servidor (os pedidos são enviados ao servidor através do canal

*/


int main(int argc, char *argv[])
{
    int fd;
    struct ArgumentFlags args;

    if (argc!=5) {
        printf("Usage: U1 <-t nsecs> fifoname\n");
        exit(1);
    }
    
    initArgumentFlags(&args);
    if(parseArguments(argc, argv, &args)!= 0) {
        printf("Usage: U1 <-t nsecs> fifoname\n");
        exit(1);
    }
    checkFlags(&args);

    fd = open(args.fifoname, O_WRONLY);
    if (fd == -1) {
        printf("Oops !!! Server is closed !!!\n");
        exit(1);
    }
    printf("FIFO openned in WRITEONLY mode\n");
    write(fd, "Quero ir a casa de banho!!!!", 30);
    close(fd);
    return 0;
} 