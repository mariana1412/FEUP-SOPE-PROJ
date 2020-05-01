#include "U1.h"

char fifoname[MAX_MSG_LEN];
clock_t begin;
int alarmOn;
 
void alarm_handler(int sig){ alarmOn = 0;}
 
void *thr_func(void *num)
{
    int fd, fd2, pl = -1;
    int pid = getpid(), pid_s;
    int i = *(int *)num;
    int dur = rand() % 20000 + 1;
    pthread_t tid = pthread_self(), tid_s;
 
    //CREATE REQUEST//
    char msg[MAX_MSG_LEN];
 
    sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid, tid, dur, pl);
 
    if ((fd = open(fifoname, O_WRONLY)) < 0)
    {
        fprintf(stderr, "Oops server is closed\n");
        regOper("CLOSD", i, pid, tid, dur, pl);
        alarmOn = 0;
        return NULL;
    }
    else
    {
        if (write(fd, msg, MAX_MSG_LEN) < 0)
            exit(2);
        regOper("IWANT", i, pid, tid, dur, pl);
        close(fd);
    }
    //
 
    //READ RESPOSTA//
    char privatefifo[MAX_MSG_LEN];
    sprintf(privatefifo, "/tmp/%d.%ld", pid, tid);
 
    if (mkfifo(privatefifo, 0660) < 0)
    {
        printf("Error creating answer fifo\n");
        exit(2);
    }
 
    if ((fd2 = open(privatefifo, O_RDONLY)) < 0)
    {
        regOper("FAILD", i, pid, tid, dur, pl);
        close(fd2);
        if (unlink(privatefifo))
        {
            printf("Error when destroying private fifo\n");
            exit(2);
        }
        return NULL;
    }
 
    char str[MAX_MSG_LEN];
 
    if (read(fd2, str, MAX_MSG_LEN) < 0)
    {
        regOper("FAILD", i, pid, tid, dur, pl);
        close(fd2);
        if (unlink(privatefifo))
        {
            printf("Error when destroying private fifo\n");
            exit(2);
        }
        return NULL;
    }
 
    close(fd2);
 
    sscanf(str, "[%d,%d,%ld,%d,%d]", &i, &pid_s, &tid_s, &dur, &pl);
    if ((pl == -1) && (dur = -1))
        regOper("CLOSD", i, pid_s, tid_s, dur, pl);
    else
        regOper("IAMIN", i, pid_s, tid_s, dur, pl);
    ////
    if (unlink(privatefifo)< 0)
        printf("Error when destroying private fifo\n");
 
    return NULL;
}
 
int main(int argc, char *argv[])
{
    struct ArgumentFlags args;
    pthread_t tid[NUM_MAX_THREADS];
    int num[NUM_MAX_THREADS], k = 0;
 
    srand(time(NULL));
 
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
    start_time();
    strcpy(fifoname, args.fifoname);
 
    signal(SIGALRM, alarm_handler);
    alarmOn = 1;
    alarm(args.nsecs);
    while (alarmOn)
    {
        num[k] = k + 1;
        pthread_create(&tid[k], NULL, thr_func, &num[k]);
        pthread_detach(tid[k]);
        usleep(500 * 1000);
        k++;
    }
    printf("Finished work\n");
 
    pthread_exit(0);
}