#include "Q2.h"

struct ArgumentFlags args;
int alarmOn;
int place=0;
static clock_t beginTime; 
static struct Queue wcQueue;
pthread_mutex_t placeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t strMutex = PTHREAD_MUTEX_INITIALIZER;
sem_t nthreads;
sem_t nplaces;

void alarm_handler(int sig){ alarmOn = 0; }
 
void *thr_funcStandard(void *msgCl){
    pthread_detach(pthread_self());
    
    int fd2;
    int placeDef = 0, tries = 0;
    int pid_s = getpid(), pid_cl = 0, i, pl =- 1, dur = 0;
    pthread_t tid_s = pthread_self(), tid_cl = 0;
    char privatefifo[MAX_MSG_LEN];
       
    //Receives the request in a string and forms privatefifo      
    sscanf((char *) msgCl, "[%d,%d,%lu,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
    sprintf(privatefifo, "/tmp/%d.%lu", pid_cl, tid_cl);

    //SENDING ANSWERS TO THE REQUEST THROUGH privatefifo//
    while(((fd2=open(privatefifo,O_WRONLY)) <= 0) && tries < 3) {
        usleep(3000);
        tries++;
    }

    if(tries == 3){
        regOper("GAVUP", i, pid_cl, tid_cl, dur, pl, (double)(time(NULL) - beginTime));
        if(args.nthreads){
            sem_post(&nthreads);
        }
        return NULL;
    }

    char msg[MAX_MSG_LEN];

    if(alarmOn){
        if(args.nplaces){
            sem_wait(&nplaces);
            pthread_mutex_lock(&placeMutex);
            placeDef = removeData(&wcQueue);
            pthread_mutex_unlock(&placeMutex);
            regOper("ENTER", i, pid_cl, tid_cl, dur, placeDef, (double)(time(NULL) - beginTime));
            printf("Enter 1 on %d\n", i);
            sprintf(msg, "[%d,%d,%lu,%d,%d]", i, pid_s, tid_s, dur, placeDef);
            
        }
        else{
            pthread_mutex_lock(&placeMutex);
            place++;
            pthread_mutex_unlock(&placeMutex);
            regOper("ENTER", i, pid_cl, tid_cl, dur, place, (double)(time(NULL) - beginTime));
            sprintf(msg, "[%d,%d,%lu,%d,%d]", i, pid_s, tid_s, dur, place);
        }

        printf("Before write on %i\n", i);
        if(write(fd2, msg, MAX_MSG_LEN) <= 0){
            fprintf(stderr, "Error trying to write to the private fifo\n");
            printf("Inside write on %d\n", i);
            if(args.nplaces){
                pthread_mutex_lock(&placeMutex);
                insert(placeDef,&wcQueue);
                pthread_mutex_unlock(&placeMutex);
                sem_post(&nplaces);
            }
            if(args.nthreads){
                sem_post(&nthreads);
            }
            close(fd2);
            return NULL;
        }
        printf("After write on %d\n", i);
        usleep(dur);
        close(fd2);
        if(args.nthreads){
            sem_post(&nthreads);
        }
        if(args.nplaces){
            pthread_mutex_lock(&placeMutex);
            insert(placeDef, &wcQueue);
            pthread_mutex_unlock(&placeMutex);
            sem_post(&nplaces);
            regOper("TIMUP", i, pid_cl, tid_cl, dur, placeDef, (double)(time(NULL) - beginTime));
        }  
        else{
            regOper("TIMUP", i, pid_cl, tid_cl, dur, place, (double)(time(NULL) - beginTime));
        }
    }
    printf("Exiting on %d\n", i);
    pthread_exit(NULL);
}


void *thr_funcClosed(void *msgCl){
    pthread_detach(pthread_self());
    int fd2;
    int pid_s = getpid(), pid_cl = 0, i, pl =- 1, dur = 0;
    pthread_t tid_s = pthread_self(), tid_cl = 0;
    char privatefifo[MAX_MSG_LEN];
       
    //Receives the request in a string and forms privatefifo      
    sscanf((char *) msgCl, "[%d,%d,%lu,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
    sprintf(privatefifo, "/tmp/%d.%lu", pid_cl, tid_cl);
 
    //SENDING ANSWERS TO REQUEST- privatefifo//
    if ((fd2=open(privatefifo,O_WRONLY)) == -1) {
        regOper("GAVUP", i, pid_cl, tid_cl, dur, pl, (double)(time(NULL) - beginTime));
        return NULL;
    }
 
    char msg[MAX_MSG_LEN];

    if(!alarmOn){
        dur =-1;
        regOper("2LATE", i, pid_cl, tid_cl, dur, pl, (double)(time(NULL) - beginTime));
        sprintf(msg, "[%d,%d,%lu,%d,%d]", i, pid_s, tid_s, dur,pl);
        if(write(fd2, msg, MAX_MSG_LEN) < 0){
            regOper("GAVUP", i, pid_cl, tid_cl, dur, pl, (double)(time(NULL) - beginTime));
            close(fd2);
            if(args.nthreads){
                sem_post(&nthreads);
            }
            return NULL;
        } 
    }
    close(fd2);
    if(args.nthreads){
        sem_post(&nthreads);
    }
 
    return NULL;
}

int main(int argc, char *argv[]){

    setbuf(stdout, NULL);

    int fd, i, pid_cl, dur, pl, k=0, bytesread;
    char str[MAX_MSG_LEN];    
    pthread_t *tid;
    pthread_t tid_cl;
    
    

    if (argc != 4 && argc != 6 && argc != 8) {
        fprintf(stderr,"Usage: Q2 <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
        exit(1);
    }
    
    initArgumentFlags(&args);
 
    if(parseArgumentsServer(argc, argv, &args)!= 0) {
        fprintf(stderr,"Usage: Q2 <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
        exit(1);
    }

    //checkFlags(&args);


    beginTime = time(NULL);
    signal(SIGALRM, alarm_handler);
    alarmOn = 1;
    alarm(args.nsecs);
 
    if (mkfifo(args.fifoname,0660)<0) {
        if (errno==EEXIST) fprintf(stderr, "FIFO already exists\n");
        else fprintf(stderr,"Can't create FIFO\n"); 
        exit(2);
    }  
 
    if ((fd = open(args.fifoname, O_RDONLY)) < 0) {
        fprintf(stderr, "Can't open public fifo for reading \n");
        if(unlink(args.fifoname)<0)
             fprintf(stderr,"Error when destroying FIFO\n");
       exit(3);
    }
 
    //READS REQUEST//

    if (args.nthreads != 0) {
        sem_init(&nthreads, 0, args.nthreads);
        tid = malloc(sizeof(pthread_t)*args.nthreads);
    }
    else
       tid = malloc(sizeof(pthread_t)*NUM_MAX_THREADS);

    if (args.nplaces != 0) {
        sem_init(&nplaces, 0, args.nplaces);
        createQueue(args.nplaces, &wcQueue);
    }
 
 
    while (alarmOn){
        if((bytesread = read( fd, &str, MAX_MSG_LEN - 1)) > 0){
            str[bytesread] = '\0';
            if(str[0] == '[' && str[strlen(str)-1] == ']'){
                dur = 0;
                tid_cl = 0;
                pid_cl = 0;
 
                pthread_mutex_lock(&strMutex);
                sscanf(str, "[%d,%d,%lu,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
                regOper("RECVD", i, pid_cl, tid_cl, dur, pl, (double)(time(NULL) - beginTime));

                if(args.nthreads != 0)
                    sem_wait(&nthreads);
                


                pthread_create(&tid[k], NULL, thr_funcStandard, str);//sends the read message to the thread
                pthread_mutex_unlock(&strMutex);
                k++;

                if(args.nthreads){
                    k = k % args.nthreads;
                }
                 pthread_mutex_unlock(&strMutex);
            }
        }
    }

    if (unlink(args.fifoname) < 0) fprintf(stderr,"Error when destroying FIFO\n");

    int n=0, j = k;

    while((n=read(fd,&str,MAX_MSG_LEN-1)>0)){
        str[bytesread] = '\0';
        if(str[0] == '[' && str[strlen(str)-1] == ']'){
                dur = 0;
                tid_cl = 0;
                pid_cl = 0;
 

                pthread_mutex_lock(&strMutex);
                sscanf(str, "[%d,%d,%lu,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
                regOper("RECVD", i, pid_cl, tid_cl, dur, pl, (double)(time(NULL) - beginTime));

                if(args.nthreads != 0)
                    sem_wait(&nthreads);

                
                    
                pthread_create(&tid[j], NULL, thr_funcClosed, str);//sends the read message to the thread
                pthread_mutex_unlock(&strMutex);
                j++;

                if(args.nthreads)
                    j %= args.nthreads;
                    
               pthread_mutex_unlock(&strMutex);
            }
    }

    //free(tid);
    close(fd);
    fprintf(stderr,"Bathroom is closed\n");
    
    pthread_exit(0);
}

