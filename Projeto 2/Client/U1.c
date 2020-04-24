#include "U1.h"

char fifoname[MAX_MSG_LEN];


void *thr_func(void *num)
{
    int fd, fd2;
    int i = *(int *) num;
    printf("%d\n",i);
    int pid = getpid();
    pthread_t tid = pthread_self();
    int pl = -1, n;
    char str[MAX_MSG_LEN];
    //generating random duration
    int dur = rand()%10000 +1;

    if ((fd = open(fifoname, O_WRONLY|O_NONBLOCK))== -1){
        printf("Oops !!! Server is closed !!!\n");
        exit(1);
    } 
    else
        printf("FIFO openned in WRITEONLY mode\n");
    
    //CRIAR REQUEST//
    char msg[MAX_MSG_LEN];
    sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid, tid, dur, pl);
    write(fd, msg, MAX_MSG_LEN);
    regOper("IWANT",i,dur,pl);
    //////
    close(fd);

    char privatefifo[MAX_MSG_LEN];
    sprintf(privatefifo, "/tmp/%d.%ld", pid, tid);
    
    if (mkfifo(privatefifo, 0660)<0) printf("Error creating answer fifo\n");
    else printf("All good creating fifo\n");

    if ((fd2=open(privatefifo,O_RDONLY)) !=-1)
        printf("All good opening fifo %s\n", privatefifo);
    else
        printf("FIFO not opened\n");

    
    //LER RESPOSTA//
    n=read(fd2,str,MAX_MSG_LEN);
    if (n>0) 
        printf("%s has arrived\n",str);
    /////

    
    close(fd2);
    if (unlink(privatefifo)<0)
        printf("Error when destroying private fifo\n");
    else
        printf("private fifo has been destroyed\n");

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct ArgumentFlags args;
    pthread_t tid[NUM_MAX_THREADS];
    int num[2];
    //struct timeval begin, end; 
    //int t=0;
    //double time;   );
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
    checkFlags(&args);
    
    strcpy(fifoname, args.fifoname);
    printf("Here? %s\n", fifoname);
    //gettimeofday(&begin,NULL); 
    //gettimeofday(&end,NULL);
    
    
    //time=(end.tv_sec-begin.tv_sec)*1e6;
    //time=(time+(end.tv_usec-begin.tv_usec))*1e6;

    //while(time<args.nsecs){
    
    for(int k = 0; k < 2; k++) {
        //i++;
        num[k] = k+1;
        printf("i na main: %d\n",num[k]);
        pthread_create(&tid[k], NULL, thr_func, &num[k]);
        //gettimeofday(&end,NULL);
        //time=(end.tv_sec-begin.tv_sec)*1e6;
        //time=(time+(end.tv_usec-begin.tv_usec))*1e-6;
        //printf("Time occurred: %f\n", time);
        
        //t++;
    }
    for(int k = 0; k < 2; k++) {
        printf("%ld\n", tid[k]);
        pthread_join(tid[k], NULL);
    }
    
    printf("Finished work\n");

    return 0;
}   