#include "Q1.h"
struct ArgumentFlags args;
int place = 0;

void *thr_func(void *msgCl){
    int fd2;
    int pid_s = getpid(), pid_cl = 0, i, pl =- 1, dur = 0;
    pthread_t tid_s = pthread_self(), tid_cl = 0;
    char privatefifo[MAX_MSG_LEN];
    
 
    
    //Receives the request in a string and forms privatefifo       
    sscanf((char *) msgCl, "[%d,%d,%ld,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
    

    sprintf(privatefifo, "/tmp/%d.%ld", pid_cl, tid_cl);

    //MANDAR RESPOSTAS - privatefifo//
    if ((fd2=open(privatefifo,O_WRONLY)) == -1) {
        regOper("GAVUP", i, pid_cl, tid_cl, dur, pl);
        return NULL;
    }

    //build answer to request
    char msg[MAX_MSG_LEN];
    if(timePassed()+(dur/1000) < args.nsecs){
        place ++;
        regOper("ENTER", i, pid_cl, tid_cl, dur, place);
        sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid_s, tid_s, dur, place);
        write(fd2, msg, MAX_MSG_LEN);
        usleep(dur);
        regOper("TIMUP", i, pid_cl, tid_cl, dur, place); 
    }
    else{
        dur =-1;
        regOper("2LATE", i, pid_cl, tid_cl, dur, pl);
        sprintf(msg, "[%d,%d,%ld,%d,%d]", i, pid_s, tid_s, dur,pl);
        write(fd2, msg, MAX_MSG_LEN);
        
    } 
    
    close(fd2);

    return NULL;
   
}

int main(int argc, char *argv[]){
    int fd;
    char str[MAX_MSG_LEN];
    
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

        
    start_time();

    if (mkfifo(args.fifoname,0660)<0) {
        if (errno==EEXIST) printf("FIFO already exists\n");
        else printf("Can't create FIFO\n");
    }  

    if ((fd = open(args.fifoname, O_RDONLY)) < 0) {    // will block until clients open it
        unlink(args.fifoname);
        pthread_exit(NULL);
    } 

    int k=0;
    int bytesread;

    //LER REQUEST//
    while(timePassed() < args.nsecs){
        if((bytesread = read( fd, &str, MAX_MSG_LEN - 1)) > 0){
            str[bytesread] = '\0';
            if(str[0] == '[' && str[strlen(str)-1] == ']'){
                dur = 0;
                tid_cl = 0;
                pid_cl = 0;

                sscanf(str, "[%d,%d,%d,%d,%d]", &i, &pid_cl, &tid_cl, &dur, &pl);
                regOper("RECVD", i, pid_cl, tid_cl, dur, pl);
                pthread_create(&tid[k], NULL, thr_func, str);//envia ja a mensagem lida para a thread
                //pthread_detach(tid[k]);
                k++;
            }

        }

    }
        
    close(fd);
    if (unlink(args.fifoname)<0) printf("Error when destroying FIFO\n");
    

    pthread_exit(0);
} 