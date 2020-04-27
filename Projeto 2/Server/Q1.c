#include "Q1.h"


void *thr_func(void *msgCl){
    int fd2;
    int pid_s= getpid(), pid_cl, i, pl=-1, dur;
    pthread_t tid_s = pthread_self(), tid_cl;
    char privatefifo[MAX_MSG_LEN];
    char *info = malloc(sizeof(char)*256);
    info = (char *) msgCl;
 
    printf("\nIn Thread \n"); 
    //Receives the request in a string and forms privatefifo       
    sscanf(info, "[%d,%d,%ld,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
    regOper("ENTER", i, pid_cl, tid_cl, dur, pl); 
     
    sprintf(privatefifo, "/tmp/%d.%ld", pid_cl, tid_cl);

    //MANDAR RESPOSTAS - privatefifo//
    if ((fd2=open(privatefifo,O_WRONLY)) !=-1)
        printf("FIFO openned in WRITEONLY mode\n");
    else 
        regOper("GAVUP", i, pid_cl, tid_cl, dur, pl);

    char msg[MAX_MSG_LEN];
    
    sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid_s, tid_s, dur, pl);
    write(fd2, msg, MAX_MSG_LEN);
    close(fd2);
    ////

    usleep(dur);
    regOper("TIMUP", i, pid_cl, tid_cl, dur, pl); 
    
    
    pthread_exit(NULL);
    //free(info); 
   
}

int main(int argc, char *argv[]){
    int fd, n;
    char* threadArgs[2];
    char str[MAX_MSG_LEN];
    static struct ArgumentFlags args;
    
    pthread_t tid[NUM_MAX_THREADS];
    int i, pid_cl, tid_cl, dur, pl;

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
    
    start_time();

    if (mkfifo(args.fifoname,0660)<0) {
        if (errno==EEXIST) printf("FIFO already exists\n");
        else printf("Can't create FIFO\n");
    }

    if ((fd=open(args.fifoname, O_RDONLY)) !=-1)
        printf("FIFO openned in READONLY mode\n");

    int k=0;

    //LER REQUEST//
    while(timePassed() < args.nsecs){
        if((n = read(fd, str, MAX_MSG_LEN))==-1){
            printf("Error in read \n");
        }
        
        threadArgs[k] = str;

        sscanf(str, "[%d,%d,%d,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
        regOper("RECVD", i, pid_cl, tid_cl, dur, pl);
        
        pthread_create(&tid[k], NULL, thr_func, threadArgs[k]);//envia ja a mensagem lida para a thread
        pthread_join(tid[k], NULL);
        sleep(2);
        k++;
    }


    close(fd);
    if (unlink(args.fifoname)<0) printf("Error when destroying FIFO\n");
    else printf("FIFO has been destroyed\n");
    
    return(0);
   
} 