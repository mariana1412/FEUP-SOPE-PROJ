#include "simpledu.h"

void sigint_handler(int signo) {
    printf("In SIGINT handler ...\n");
    //aqui vai tratar as confirmações, kills, etc

    char choice;

    kill(-1, SIGSTOP); //acho que isto funciona -> "pausa" todos os processos menos o que está a executar

    while(1){
        printf("Are you sure do you want to terminate (Y/N)? ");
        scanf("%s", choice);

        if(choice == 'Y' || choice == 'y'){
            printf("\n olá confirmei \n");
            kill(-5, SIGTERM);
        }
        else if(choice == 'N' || choice == 'n'){
            printf("\n olá continua amor \n");
            kill(-1, SIGCONT);
        }
        else 
            continue;
    }

    
}


int main(int argc, char *argv[], char *envp[]){
    struct ArgumentFlags args;
    
    char pid[10];
    if (getenv("MAIN_PID") == NULL) {
        sprintf(pid, "%d", getpid());
        setenv("MAIN_PID", pid, 0);
    }   


    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) < 0) { 
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }

    initExecReg();
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
            printf("Error: There was an error on the provided arguments\n");//tirar no fim 
            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\n", argv[0]);
            exit(1);
        }
    }
    else {
        printf("Fazer comando por omissão\n");
    }

    regCreate(argc, argv);
    checkFlags(&args);

    display(&args);
    regExit(0);
    return 0; 
}