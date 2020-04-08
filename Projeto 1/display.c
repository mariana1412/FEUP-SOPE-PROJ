#include "display.h"
int bytesToBlocks(int bytes, int blocksize) {
    return bytes/blocksize + 1;
}

char * buildPath(char * dirname, struct ArgumentFlags * args) {
    char *aux = malloc(100);

    strcpy(aux, args->path);
    if(aux[strlen(aux)-1] != '/'){
        strcat(aux, "/");
    }
    strcat(aux, dirname);
    if(aux[strlen(aux)-1] != '/'){
        strcat(aux, "/");
    }

    return aux;
}

void printFile(struct ArgumentFlags * args, int size, char* name) {
    if(args->all){
        int numBlocks = bytesToBlocks(size, args->blockSize);
        if(args->bytes){
            fflush(stdout);
            printf("Bytes: %d, File: %s, PID: %d\n", size, name, getpid());
        }
        else{
            fflush(stdout);
            printf("Blocks: %d, File: %s, PID: %d\n", numBlocks, name, getpid());
        }
        regEntry(numBlocks, name);
    }
}

void printDir(struct ArgumentFlags * args, int size, char* name) {
    int numBlocks = bytesToBlocks(size, args->blockSize); 
    if(args->bytes){
        fflush(stdout);            
        printf("Bytes: %d, Dir: %s, PID: %d\n", size, name, getpid());
    }
    else{ 
        fflush(stdout);                      
        printf("Blocks: %d, Dir: %s, PID: %d\n", numBlocks, name, getpid());
    }
    regEntry(numBlocks, name);
}

void printLink(struct ArgumentFlags * args, int size, char* name) {
    int numBlocks = bytesToBlocks(size, args->blockSize);
    if(args->bytes){
        fflush(stdout);
        printf("Bytes: %d, Link: %s\n", size, name);
    }
    else{
        fflush(stdout);
        printf("Blocks: %d, Link: %s\n", numBlocks, name);
    }
    regEntry(numBlocks, name);
}
    
int forkAux(struct ArgumentFlags * args, char * path){
    pid_t pid;
    int fd[2];
    pipe(fd);
    pid=fork();
    if(pid < 0)
        fprintf(stderr, "fork error\n");

    else if(pid == 0) {
        printf("I'm the child\n");
        close(fd[READ]);
        char* arguments[12];
        char *newpath = "";
        newpath = buildPath(path, args);
        
        getArgv(newpath, args, arguments);

        execv("./simpledu", arguments);
        printf("Exec failed!\n");
    
        //escrever para o pipe
    }
    else if(pid > 0) {
        char* auxPath = args->path;
        printf("\nEntering pai: %s\n", args->path);
        close(fd[WRITE]);
        /*esperar pelo filho e mostrar tamanho?*/
        
        if(waitpid(-1, NULL, 0) != pid) {
            fprintf(stderr, "wait error\n");
        }
        
        args->path = auxPath;
        printf("\nLeaving pai: %s\n", args->path);
        //ler do pipe
    }

    
    
    return 0; 
}

void display(struct ArgumentFlags *args) {
    DIR *dir; 
    char *path = args->path;
    ssize_t len;
    char buf[1024];
    char * filename= malloc(1024);

    struct dirent *dentry; 
    printf("\nEntered display with : %s\n", args->path);

    if ((dir = opendir(path)) == NULL) {   //opens a directory returning the stream of it or NULL if error
        perror(path); 
        exit(2);
    }  
  

    while ((dentry = readdir(dir)) != NULL ) { 
        
        char fullpath[1024];
    
        strcpy(fullpath, args->path);
        if(fullpath[strlen(fullpath)-1] != '/'){
            strcat(fullpath, "/");
        }
        strcat(fullpath, dentry->d_name);

        //printf("\nEntered while with : %s , -> %s -> %s\t", args->path, dentry->d_name, fullpath);
        int type = verifyPath(fullpath);
    

        struct stat stat_entry = getStat(); 
        
        switch(type){
            case 0:
                printFile(args, (int)stat_entry.st_size, dentry->d_name);
                break;

            case 1:
                if(strcmp(dentry->d_name, ".")!=0 && strcmp(dentry->d_name, "..")!=0) {
                    if(args->maxDepth > 0) {
                        forkAux(args, dentry->d_name);
                    }
                    printDir(args, (int)stat_entry.st_size, dentry->d_name);  
                }  
                break;
            case 2:
                if(args->simbolicLinks) {
                    /*seguir no link*/
                    if((len=readlink(dentry->d_name,buf,sizeof(buf)-1))!=-1){//building the path to the simbolic link
                        buf[len]='\0'; 
                        strcat(filename,buf);
                        int aux= verifyPath(filename);
                        if(aux ==1)//if it is a directory
                            strcat(filename,"/");
                        printf("Filename: %s",filename);    
                    }
                    args->path=filename;
                    display(args);

                }
                else{
                    printLink(args, (int)stat_entry.st_size, dentry->d_name);    
                }
                break;
        }
                       
    }
}

