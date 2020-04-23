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

void *thr_func(void *fname)
{
    int fd2;
    char * info = (char *) fname;//receives the request in a string
    int pid = getpid();
    pthread_t tid = pthread_self();
    int pl = -1, dur = 3;
    int i; 
    char privatefifo[MAX_MSG_LEN];
  
    sscanf(info, "[%d,%d,%ld,%d,%d]", &i, &pid, &tid, &dur, &pl);

    sprintf(privatefifo, "/tmp/%d.%ld", pid, tid);
    //é no private fifo que cria a resposta
    

    //////NAO ESTÀ NADA REVISTO DENTRO DESTA SECCÃO- ponto 2  de Qn
    printf("Msg: %s", info);
    if ((fd2=open(privatefifo,O_WRONLY|O_NONBLOCK)) !=-1)
        printf("FIFO openned in WRITEONLY mode\n");
    //MANDAR RESPOSTA
    char msg[MAX_MSG_LEN];
    sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid, tid, dur, pl);
    write(fd2, msg, MAX_MSG_LEN);
    
    ///////

    close(fd2);

    return NULL;
}


int main(int argc, char *argv[])
{
    int fd, n;
    char str[MAX_MSG_LEN];
    struct ArgumentFlags args;
    int t=0;
    pthread_t tid[NUM_MAX_THREADS];

    if (argc != 4) {
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
    
        printf("FIFO sucessfully created\n");

    if ((fd=open(args.fifoname,O_RDONLY|O_NONBLOCK)) !=-1)
        printf("FIFO openned in READONLY mode\n");

    //LER REQUEST//
    for(int k = 0; k < 2; k++){
        if((n=read(fd,str,MAX_MSG_LEN))==-1)
            printf("Error in read \n");
        printf("%s has arrived\n",str);
        pthread_create(&tid[t], NULL, thr_func, str);
        pthread_join(tid[t],NULL);
        pthread_exit(NULL);
        /* após o tempo estipulado para o funcionamento do programa (nsecs), que corresponde ao
         encerramento doQuarto de Banho, o canal de comunicação fifoname é desativado, mas de 
         forma a que os pedidos pendentesno "buffer" do canal sejam notificados do facto e 
         os pedidos em utilização do Quarto de Banho sejamcompletados*/
    }
    
    close(fd);
    if (unlink(args.fifoname)<0)
        printf("Error when destroying FIFO\n");
    else
        printf("FIFO has been destroyed\n");
    
    return 0;
} 