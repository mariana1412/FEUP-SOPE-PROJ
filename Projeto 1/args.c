#include "args.h"

static struct stat pathStat;

int isNumber(char *n) {
    int size = sizeof(n)/sizeof(char*);
    for(int i = 0; i<size; i++){
        if(!(n[i]>47 && n[i]<58))
            return 0;         
    }
    return 1;
}

int idArguments(char *arg){

    char *arguments[11] = {"-a", "--all", "-b", "--bytes", "-B", "--block-size=", "-L", "--dereference", "-S", "--separate-dirs", "--max-depth="};
 
    //nota: strcmp() retorn 0 quando são iguais
    if(!strcmp(arg, arguments[0]) || !strcmp(arg, arguments[1])){ 
        return ALL;
    }
    else if(!strcmp(arg, arguments[2]) || !strcmp(arg, arguments[3])){ 
        return BYTES;
    }
    else if(!strcmp(arg, arguments[4]) || !strncmp(arg, arguments[5], 13)){ 
        return BLOCK_SIZE;
    }
    else if(!strcmp(arg, arguments[6]) || !strncmp(arg, arguments[7], 13)){ 
        return DEREFERENCE ;
    }
    else if(!strcmp(arg, arguments[8]) || !strncmp(arg, arguments[9], 15)){ 
        return SEPARATE_DIRS ;
    }
    else if(!strncmp(arg, arguments[10], 12)){
        return MAX_DEPTH;
    }
    else if(verifyPath(arg) != -1)
        return PATH;

    else if(verifyPath(arg) == -1){
        printf("Error: No such file or directory\n");
        return -1;
    }
    else
        return -1;
   
}


int verifyPath(char *path){

    if(lstat(path, &pathStat) < 0)  //invalid path
        return -1;

    else if(S_ISREG(pathStat.st_mode)) //file
        return 0;

    else if(S_ISDIR(pathStat.st_mode)) //directory
        return 1;

    else if(S_ISLNK(pathStat.st_mode)) //link
        return 2;
    
    return -1;
}

struct stat getStat(){
    return pathStat;
}

void initArgumentFlags(struct ArgumentFlags *args){
    args->all = 0;
    args->bytes = 0;
    args->blockSize = 1024;
    args->countLinks = 1;
    args->simbolicLinks = 0;
    args->noSubDir = 0;
    args->maxDepth = INT_MAX;
    args->path = ".";
}

int parseArguments(int argc, char *argv[], struct ArgumentFlags *args){
    for(int i = 2; i < argc; i++){
        int id = idArguments(argv[i]); 
        switch(id){
            case ALL:
                args->all = 1;
                break;

            case BYTES:
                args->bytes = 1;
                break;

            case BLOCK_SIZE:    
                if(strcmp(argv[i], "-B") == 0) {

                    if((i+1) < argc)
                        i++;
                    else
                        return -1;
                    
                    

                    if(isNumber(argv[i])){
                        args->blockSize = atoi(argv[i]);
                    }
                    else{
                        return -1;
                    }
 
                }
                else {
                    char *num = strstr(argv[i], "=");
                     

                    if(isNumber(num+1)){
                       args->blockSize = atoi(num+1);
                    }
                    else{
                        return -1;
                    }

                }
                break;

            case DEREFERENCE:
                args->simbolicLinks = 1;
                break;

            case SEPARATE_DIRS:
                args->noSubDir = 1;
                break;

            case MAX_DEPTH: {
                char *num = strstr(argv[i], "=");
                
                if(isNumber(num+1)){
                    args->maxDepth = atoi(num+1);
                }
                else{
                    return -1;
                }

                break;
            }
            case PATH:
                args->path = argv[i];
                break;
            case -1:
                return -1;

        }
    }
    return 0;
}

void checkFlags(struct ArgumentFlags* args){
    printf("All: %d\n",args->all);
    printf("Bytes: %d\n",args->bytes);
    printf("BlockSize: %d\n",args->blockSize);
    printf("CountLinks: %d\n",args->countLinks);
    printf("SimbolicLinks: %d\n",args->simbolicLinks);
    printf("NoSubDir: %d\n",args->noSubDir);
    printf("MaxDepth: %d\n",args->maxDepth);
    printf("Path: %s\n", args->path);    
}

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
}
