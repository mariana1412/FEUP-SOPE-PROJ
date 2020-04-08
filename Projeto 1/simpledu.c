#include "simpledu.h"

extern pid_t main_prg;

int main(int argc, char *argv[], char *envp[]){
    struct ArgumentFlags args;
    char pid[10];
    struct sigaction action;
    main_prg = getpgrp();

    if (getenv("MAIN_PID") == NULL) {
        sprintf(pid, "%d", getpid());
        setenv("MAIN_PID", pid, 0);
    }   


    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) < 0) { 
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }

    initExecReg();
    
    sleep(10);

    if (argc == 1 || argc > 10) {   
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]); 
        exit(1);
    }

    if(strcmp(argv[1], "-l") != 0 && strcmp(argv[1], "--count-links") != 0) {
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
        exit(2);
    }

    initArgumentFlags(&args);

    if (argc != 2){
        if(parseArguments(argc, argv, &args) != 0){
            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
            exit(1);
        }
    }
    else {
        printf("Fazer comando por omissão\n");
    }

    regCreate(argc, argv);
    //checkFlags(&args);

    display(&args);
    regExit(0);
    unsetenv("MAIN_PID");
    
    return 0; 
}