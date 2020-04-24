#include "Q1.h"
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

void *thr_func(void *fname)
{
    int fd2;
    char *info = malloc(sizeof(char)*256);
    info = (char *) fname;
    int pid_s= getpid(), pid_cl;
    pthread_t tid_s = pthread_self(), tid_cl;
    int pl = -1, dur = 3;
    int *i = malloc(sizeof(int)); 
    char privatefifo[MAX_MSG_LEN];

    pthread_mutex_lock(&buffer_lock);   

    //receives the request in a string and forms privatefifo       
    sscanf(info, "[%d,%d,%ld,%d,%d]", i, &pid_cl, &tid_cl, &dur, &pl);
    regOper("RECV",i,dur,pl);
     
    sprintf(privatefifo, "/tmp/%d.%ld", pid_cl, tid_cl);

    printf("1. %d\n",*i);
    //
    //é no private fifo que cria a resposta
    usleep(dur);
    printf("Msg: %s\n", info);
    free(info);
    if ((fd2=open(privatefifo,O_WRONLY)) !=-1)
        printf("FIFO openned in WRITEONLY mode\n");
    printf("2. %d\n",*i);
    //MANDAR RESPOSTA
    char msg[MAX_MSG_LEN];
    sprintf(msg, "[%d,%d,%ld,%d,%d]", *i, pid_s, tid_cl, dur, pl);
    write(fd2, msg, MAX_MSG_LEN);
    //
    pthread_mutex_unlock(&buffer_lock);
    free(i);
    close(fd2);
    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    int fd, n;
    char* threadArgs[2];
    char str[MAX_MSG_LEN];
    struct ArgumentFlags args;
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
        if (errno==EEXIST) printf("FIFO already exists\n");
        else printf("Can't create FIFO\n");
    }

    if ((fd=open(args.fifoname,O_RDONLY)) !=-1)
        printf("FIFO openned in READONLY mode\n");

    //LER REQUEST//
    for(int k = 0; k < 2; k++){
        if((n=read(fd, str, MAX_MSG_LEN))==-1){
            printf("Error in read \n");
        }
        threadArgs[k] = str;
        printf("%s has arrived\n",str);
        //envia ja a mensagem lida para a thread
        pthread_create(&tid[k], NULL, thr_func, threadArgs[k]);
    }
    for(int k = 0; k < 2; k++) {
        printf("%ld\n", tid[k]);
        pthread_join(tid[k], NULL);
    }
    

    close(fd);
    if (unlink(args.fifoname)<0) printf("Error when destroying FIFO\n");
    else printf("FIFO has been destroyed\n");
    
    return 0;
} 