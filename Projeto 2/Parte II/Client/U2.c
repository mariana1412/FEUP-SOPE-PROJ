#include "U2.h"

char fifoname[MAX_MSG_LEN];
int alarmOn;
clock_t begin;
static clock_t beginTime; 
 
void alarm_handler(int sig){ alarmOn = 0;}
 
void *thr_func(void *num){
    pthread_detach(pthread_self());//acrescentado
    int fd, fd2, pl = -1, tries = 0;
    int pid = getpid(), pid_s;
    int i = *(int *)num;
    int dur = rand() % 20000 + 1;
    pthread_t tid = pthread_self(), tid_s;
 
    
    char privatefifo[MAX_MSG_LEN];
    sprintf(privatefifo, "/tmp/%d.%lu", pid, tid);
 
    if (mkfifo(privatefifo, 0660) < 0){
        fprintf(stderr,"Error creating answer fifo\n");
        exit(2); 
    }
 
    //CREATE REQUEST//  
    char msg[MAX_MSG_LEN];
 
    sprintf(msg, "[%d,%d,%lu,%d,%d]", i, pid, tid, dur, pl);
 
    if ((fd = open(fifoname, O_WRONLY)) < 0){
        fprintf(stderr, "Oops server is closed\n");
        regOper("CLOSD", i, pid, tid, dur, pl, (double)(time(NULL) - beginTime));
        alarmOn = 0;
        pthread_exit(NULL);
    }
    else{
        if (write(fd, msg, MAX_MSG_LEN) < 0){
           pthread_exit(NULL);
        }
        regOper("IWANT", i, pid, tid, dur, pl, (double)(time(NULL) - beginTime));
        close(fd);
    }
    //
 
    //READ ANSWER//
    if ((fd2 = open(privatefifo, O_RDONLY)) < 0) {
        regOper("FAILD", i, pid, tid, dur, pl, (double)(time(NULL) - beginTime));
        close(fd2);

        if (unlink(privatefifo) < 0)
            fprintf(stderr,"Error when destroying private fifo\n");

        pthread_exit(NULL);
    }
 
    char str[MAX_MSG_LEN];
 
    while((read(fd2, str, MAX_MSG_LEN) <= 0) && tries<5) {
        usleep(3000);
        tries++;   
    }

    if(tries == 5){
        regOper("FAILD", i, pid, tid, dur, pl, (double)(time(NULL) - beginTime));
        close(fd2);

        if (unlink(privatefifo)<0) 
            fprintf(stderr,"Error when destroying private fifo\n");
    
        pthread_exit(NULL);
    }

    close(fd2);
 
    sscanf(str, "[%d,%d,%lu,%d,%d]", &i, &pid_s, &tid_s, &dur, &pl);

    if ((pl == -1) && (dur = -1))
        regOper("CLOSD", i, pid_s, tid_s, dur, pl, (double)(time(NULL) - beginTime));
    else
        regOper("IAMIN", i, pid_s, tid_s, dur, pl, (double)(time(NULL) - beginTime));
    

    if (unlink(privatefifo)< 0)
        fprintf(stderr,"Error when destroying private fifo\n");
 
    pthread_exit(NULL);
}
 
int main(int argc, char *argv[]){

    setbuf(stdout, NULL);//acrescentado

    struct ArgumentFlags args;
    pthread_t tid[NUM_MAX_THREADS];
    int num[NUM_MAX_THREADS], k = 0;
    

    srand(time(NULL));
 
    if (argc != 4){
        fprintf(stderr,"Usage: U2 <-t nsecs> fifoname\n");
        exit(1);
    }
 
    initArgumentFlags(&args);
 
    if (parseArgumentsClient(argc, argv, &args) != 0){
        fprintf(stderr,"Usage: U2 <-t nsecs> fifoname\n");
        exit(1);
    }
    
    strcpy(fifoname, args.fifoname);

    beginTime = time(NULL);
    signal(SIGALRM, alarm_handler);
    alarmOn = 1;
    alarm(args.nsecs);
    
    while (alarmOn){
        num[k] = k + 1;
        pthread_create(&tid[k], NULL, thr_func, &num[k]);
        usleep(20000);//20 miliseconds
        k++;
    }
    
    fprintf(stderr,"Finished work\n");
    
    
    return 0;
}