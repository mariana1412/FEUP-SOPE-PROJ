#include "U1.h"

char fifoname[MAX_MSG_LEN];

void *thr_func(void *num){
    int fd, fd2,pl=-1;    
    int pid = getpid(), pid_s;
    int i = *(int *) num;
    int dur = rand()%20 + 1; //generating random duration
    pthread_t tid = pthread_self(), tid_s; 
    
    //CRIAR REQUEST//
    char msg[MAX_MSG_LEN];
    
    sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid, tid, dur, pl);
    
    if ((fd = open(fifoname, O_WRONLY)) < 0){
        regOper("CLOSD", i, pid, tid, dur, pl);
        printf("Oops !!! Server is closed !!!\n");
        exit(1);
    } 
    else{
        printf("FIFO openned in WRITEONLY mode\n");
        write(fd, msg, MAX_MSG_LEN);
        regOper("IWANT", i, pid, tid, dur, pl);
        close(fd);
    }
    //

    //LER RESPOSTA//
    char privatefifo[MAX_MSG_LEN];
    sprintf(privatefifo, "/tmp/%d.%ld", pid, tid);

    if (mkfifo(privatefifo, 0660) < 0){
        printf("Error creating answer fifo\n"); 
        exit(2);
    } 
    else printf("All good creating fifo %s \n", privatefifo);

    if ((fd2=open(privatefifo,O_RDONLY)) >= 0)
        printf("All good opening fifo %s\n", privatefifo);
    else{
        regOper("FAILD", i, pid, tid, dur, pl);
        exit(2);
    }

    char str[MAX_MSG_LEN];
    
    read(fd2,str,MAX_MSG_LEN);
    /*if (n>0) 
        printf("%s has arrived\n",str);*/

    close(fd2);

    sscanf(str, "[%d,%d,%ld,%d,%d]", &i, &pid_s, &tid_s, &dur, &pl);
    if((pl == -1) && (dur =-1))
        regOper("CLOSD", i, pid_s, tid_s, dur, pl);
    else
        regOper("IAMIN", i, pid_s, tid_s, dur, pl);   
    ////    

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
    int num[NUM_MAX_THREADS], k=0;

    srand(time(NULL));

    if (argc != 4){
        printf("Usage: U1 <-t nsecs> fifoname\n");
        exit(1);
    }

    initArgumentFlags(&args);

    if (parseArguments(argc, argv, &args) != 0){
        printf("Usage: U1 <-t nsecs> fifoname\n");
        exit(1);
    }
    
    start_timeU();
    strcpy(fifoname, args.fifoname);



    while(timePassed(0) < args.nsecs){
        num[k] = k+1;
        pthread_create(&tid[k], NULL, thr_func, &num[k]);
        sleep(2);
        k++;
    }
    printf("Finished work\n");

    pthread_exit(0);
}   