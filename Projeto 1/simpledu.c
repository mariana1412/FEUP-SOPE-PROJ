#include "simpledu.h"

int main(int argc, char *argv[], char *envp[]){
    struct ArgumentFlags args;
    
    char pid[10];
    if (getenv("MAIN_PID") == NULL) {
        sprintf(pid, "%d", getpid());
        setenv("MAIN_PID", pid, 0);
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
    checkFlags(&args);

    display(&args);

    return 0; 
}

