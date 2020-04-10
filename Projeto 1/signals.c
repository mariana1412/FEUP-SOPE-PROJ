#include "signals.h"
extern pid_t child;


void sigint_handler(int signal) {
    regRecvSignal(SIGINT);

    if (child){
        regSendSignal(SIGSTOP, child);
        sleep(1); 
        kill(-child, SIGSTOP);
    }

    printf("In SIGINT handler ...\n");
    char choice[500];
    while(1){
        printf("Are you sure you want to terminate (Y/N)? ");
        fflush(stdout);
        scanf("%s", choice);    
        if(strcmp(choice, "Y")==0 || strcmp(choice, "y")==0){
            regSendSignal(SIGTERM,child);
            regExit(1);
            kill(-child, SIGTERM); 
            return;
        }
        else if(strcmp(choice,"N") ==0 || strcmp(choice,"n") ==0){
            regSendSignal(SIGCONT,child);
            kill(-child, SIGCONT); 
            return;
        }
        else
            continue;
   }     
}

void sigterm_handler(int signal) {
    printf("sigterm\n");
    regRecvSignal(SIGTERM);
    regExit(1);
}

void sigcont_handler(int signal) {
    printf("sigcont\n");
    regRecvSignal(SIGCONT);
}
