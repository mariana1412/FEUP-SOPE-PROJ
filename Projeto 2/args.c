#include "args.h"

int isNumber(char *n) {
    int size = sizeof(n)/sizeof(char*);
    for(int i = 0; i<size; i++){
        if(!(n[i]>47 && n[i]<58))
            return 0;         
    }
    return 1;
}

int idArguments(char *arg){
    char* nsecs = "-t";

    //nota: strcmp() retorn 0 quando são iguais
    if(!strcmp(arg, nsecs)){ 
        return NSECS;
    }
    else
        return FIFONAME;
}


void initArgumentFlags(struct ArgumentFlags *args){
    args->nsecs = 0;
    args->fifoname = "/tmp/requests";
}

int parseArguments(int argc, char *argv[], struct ArgumentFlags *args){
    for(int i = 2; i < argc; i++){
        int id = idArguments(argv[i]); 
        switch(id){
            case NSECS:

                if((i+1) < argc)
                    i++;
                else
                    return -1;
                if(isNumber(argv[i]))
                    args->nsecs = atoi(argv[i]);
                else
                    return -1;             
                break;

            case FIFONAME:
                args->fifoname = argv[i];
                break;
        }
    }
    return 0;
}

void checkFlags(struct ArgumentFlags* args){
    printf("Nsecs: %d\n", args->nsecs);
    printf("Fifoname: %s\n", args->fifoname); 
}
/*
void getArgv(char* dirpath, struct ArgumentFlags *args, char*res[]){

    res[0] = "simpledu";
    res[1] = "-l";
    res[2] = dirpath;

    int i = 3;
    if(args->all)
    {
        res[i] = "-a";
        i++;
    }
    if(args->bytes){
        res[i] = "-b";
        i++;
    }
    if(args->blockSize != 1024){
        res[i] = "-B";
        i++;
        char aux[10];
        sprintf(aux, "%d", args->blockSize);
        res[i] = aux;
        i++;
    }
    if(args->simbolicLinks){
        res[i] = "-L";
        i++;
    }
    if(args->noSubDir){
        res[i] = "-S";
        i++;
    }
    if(args->maxDepth != INT_MAX){
        char aux[10];
        int newdepth = args->maxDepth - 1;
        sprintf(aux, "%d", newdepth);
        char aux2[] = "--max-depth=";
        strcat(aux2, aux);
        res[i] = aux2;
        i++;
    } 
    for(int j = i; j < 12; j++){
        res[j] = NULL;
    }  
}*/