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
/* [  i, pid, tid, dur, pl ] */
static int i = 0;

void *thr_func(void *fname)
{
    int fd, fd2;
    char * fifoname = (char *) fname;
    int pid = getpid();
    pthread_t tid = pthread_self();
    int pl = -1, dur = 3, n;
    char str[MAX_MSG_LEN];
    
    fd = open(fifoname, O_WRONLY|O_NONBLOCK);
    
    if (fd == -1)
    {
        printf("Oops !!! Server is closed !!!\n");
        exit(1);
    }
    printf("FIFO openned in WRITEONLY mode\n");
    
    //CRIAR REQUEST//
    char msg[MAX_MSG_LEN];
    sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid, tid, dur, pl);
    write(fd, msg, MAX_MSG_LEN);
    //////

    char privatefifo[MAX_MSG_LEN];
    sprintf(privatefifo, "/tmp/%d.%ld", pid, tid);//tid bue grande
    
    if (mkfifo(privatefifo, 0660)<0) {
        printf("Error creating answer fifo\n");
    } else{
        printf("All good creating fifo\n");
    }

    if ((fd2=open(privatefifo,O_RDONLY|O_NONBLOCK)) !=-1)
        printf("All good opening fifo %s\n", privatefifo);
    else
        printf("FIFO not opened\n");

    //LER RESPOSTA//
    n=read(fd,str,MAX_MSG_LEN);
    if (n>0) 
        printf("%s has arrived\n",str);
    /////

    close(fd);
    close(fd2);
    if (unlink(privatefifo)<0)
        printf("Error when destroying private fifo\n");
    else
        printf("private fifo has been destroyed\n");
    
    return NULL;

}

int main(int argc, char *argv[])
{
    struct ArgumentFlags args;
    pthread_t tid[NUM_MAX_THREADS];
    struct timeval begin, end; 
    int t=0;
    double time;   

    if (argc != 4)
    {
        printf("Usage: U1 <-t nsecs> fifoname\n");
        exit(1);
    }

    initArgumentFlags(&args);
    if (parseArguments(argc, argv, &args) != 0)
    {
        printf("Usage: U1 <-t nsecs> fifoname\n");
        exit(1);
    }
    checkFlags(&args);

    gettimeofday(&begin,NULL); 
    gettimeofday(&end,NULL);
    
    
    time=(end.tv_sec-begin.tv_sec)*1e6;
    time=(time+(end.tv_usec-begin.tv_usec))*1e6;

    while(time<args.nsecs){
        i++;
        pthread_create(&tid[t], NULL, thr_func, args.fifoname);
        pthread_join(tid[t], NULL);
        
        gettimeofday(&end,NULL);
        time=(end.tv_sec-begin.tv_sec)*1e6;
        time=(time+(end.tv_usec-begin.tv_usec))*1e-6;
        printf("Time occurred: %f\n", time);
        pthread_exit(NULL);
        t++;
    }

    printf("Finished work\n");

    return 0;
}