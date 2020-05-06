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
    char* nplaces = "-l";
    char* nthreads = "-n";
    //nota: strcmp() retorn 0 quando são iguais
    if(!strcmp(arg, nsecs)){ 
        return NSECS;
    }
    else if(!strcmp(arg, nplaces)) {
        return NPLACES;
    }
    else if(!strcmp(arg, nthreads)) {
        return NTHREADS;
    }
    else
        return FIFONAME;
}

void initArgumentFlags(struct ArgumentFlags *args){
    
    args->nsecs = 0;
    args->nthreads = INT_MAX;
    args->nplaces = INT_MAX;
    args->fifoname = "/tmp/requests";
}
int parseArgumentsClient(int argc, char *argv[], struct ArgumentFlags *args){
    for(int i = 1; i < argc; i++){
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

            default:
                return -1;
        }
    }
    return 0;
}

int parseArgumentsServer(int argc, char *argv[], struct ArgumentFlags *args){
    for(int i = 1; i < argc; i++){
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
            case NPLACES:
                if((i+1) < argc)
                    i++;
                else
                    return -1;
                if(isNumber(argv[i]))
                    args->nplaces = atoi(argv[i]);
                else
                    return -1;             
                break;
            case NTHREADS:
                if((i+1) < argc)
                    i++;
                else
                    return -1;
                if(isNumber(argv[i]))
                    args->nthreads = atoi(argv[i]);
                else
                    return -1;             
                break;
            case FIFONAME:
                args->fifoname = argv[i];
                break;
            default:
                return -1;
        }
    }
    return 0;
}

void checkFlags(struct ArgumentFlags* args){
    printf("Nsecs: %d\n", args->nsecs);
    printf("Nplaces: %d\n", args->nplaces);
    printf("Nthreads: %d\n", args->nthreads);
    printf("Fifoname: %s\n", args->fifoname); 
}
