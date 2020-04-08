#include "signals.h"
extern pid_t child;
void sigint_handler(int signo) {
    regRecvSignal(SIGINT);

    if (child) {
        kill(-child, SIGSTOP);
        regSendSignal(SIGSTOP, -child);
    }
    printf("In SIGINT handler ...\n");
    char choice[500];
    while(1){
        printf("Are you sure you want to terminate (Y/N)? ");
        fflush(stdout);
        scanf("%s", choice);    
        if(strcmp(choice, "Y")==0 || strcmp(choice, "y")==0){
            regSendSignal(SIGTERM,-child);
            regExit(1);
            kill(-child, SIGTERM); 
            return;
        }
        else if(strcmp(choice,"N") ==0 || strcmp(choice,"n") ==0){
            regSendSignal(SIGCONT,-child);
            kill(-child, SIGCONT); 
            return;
        }
        else
            continue;
   }     
}
