#include "Q1.h"
/*
●invocado com o comando
    ○ Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname
        ■ nsecs - nº (aproximado) de segundos que o programa deve funcionar
        ■ nplaces - lotação do Quarto de Banho
        ■ nthreads – nº (máximo) de threads a atender pedidos 
        ■ fifoname – nome do canal público (FIFO) a criar pelo servidor para atendimento de pedidos
● inicialmente, recolhe os argumentos de linha de comando e executa as operações fundamentais para o 
funcionamento do programa;
● cada pedido é atendido por um thread, que comunica com o thread cliente e controla o tempo de utilização
 deum lugar do Quarto de Banho; se não houver lugares disponíveis, espera que haja e prossegue;
● após o tempo estipulado para o funcionamento do programa (nsecs), que corresponde ao encerramento do 
Quarto de Banho, o canal de comunicação fifoname é desativado, mas de forma a que os pedidos pendentesno
"buffer" do canal sejam notificados do facto e os pedidos em utilização do Quarto de Banho sejam 
completados;
● no final, todos os recursos tomados ao sistema devem ser libertados.
*/

int main(int argc, char *argv[])
{
    int fd, n, fd_dummy;
    char str[MAX_MSG_LEN];
    struct ArgumentFlags args;

    if (argc != 5) {
        printf("Usage: Q1 <-t nsecs> fifoname\n");
        exit(1);
    }

    initArgumentFlags(&args);

    if(parseArguments(argc, argv, &args)!= 0) {
        printf("Usage: Q1 <-t nsecs> fifoname\n");
        exit(1);
    }

    checkFlags(&args);

    if (mkfifo(args.fifoname,0660)<0) {
        if (errno==EEXIST) 
            printf("FIFO already exists\n");
        else 
            printf("Can't create FIFO\n");
    }
    else 
        printf("FIFO sucessfully created\n");

    if ((fd=open(args.fifoname,O_RDONLY)) !=-1)
        printf("FIFO openned in READONLY mode\n");

    if ((fd_dummy=open(args.fifoname,O_WRONLY)) !=-1)
        printf("FIFO openned in WRITEONLY mode\n");
    do
    {
        n=read(fd,str,MAX_MSG_LEN);
        if (n>0) printf("%s has arrived\n",str);
    } while (strcmp(str,"SHUTDOWN")!=0);

    close(fd);
    close(fd_dummy);
    if (unlink(args.fifoname)<0)
        printf("Error when destroying FIFO\n");
    else
        printf("FIFO has been destroyed\n");
    
    return 0;
} 