#include "display.h"

int bytesToBlocks(int bytes, int blocksize) {
    return bytes/blocksize + 1;
}

char * buildPath(char * dirname, struct ArgumentFlags * args) {
    char *aux = args->path; 
    strcat(aux, "/");
    printf("\n1.  %s\t", aux);
    strcat(aux, dirname);
    printf("2.  %s\n", aux);
    strcat(aux,"/");
    return aux;
}

void printFile(struct ArgumentFlags * args, int size, char* name) {
    if(args->all){
        if(args->bytes){
            fflush(stdout);
            printf("Bytes: %d, File: %s\n", size, name);
        }
        else{
            int numBlocks = bytesToBlocks(size, args->blockSize);
            fflush(stdout);
            printf("Blocks: %d, File: %s\n", numBlocks, name);
        }
    }
}

void printDir(struct ArgumentFlags * args, int size, char* name) {
        if(args->bytes){
            fflush(stdout);
            printf("Bytes: %d, Dir: %s\n", size, name);
        }
        else{
            int numBlocks = bytesToBlocks(size, args->blockSize);  
            fflush(stdout);                      
            printf("Blocks: %d, Dir: %s\n", numBlocks, name);
        }
}

void printLink(struct ArgumentFlags * args, int size, char* name) {
    if(args->bytes){
        fflush(stdout);
        printf("Bytes: %d, Link: %s\n", size, name);
    }
    else{
        int numBlocks = bytesToBlocks(size, args->blockSize);
        fflush(stdout);
        printf("Blocks: %d, Link: %s\n", numBlocks, name);
    }
}
    
int forkAux(struct ArgumentFlags * args, char * path){
    pid_t pid;
    int status;
    int fd[2];
    pipe(fd);
    pid = fork();

    if(pid  < 0)
        fprintf(stderr, "fork error\n");

    else if(pid == 0) {
        close(fd[READ]);
        //char * arguments = getArgv(path, args);
        char *arguments[]={"./simpledu","-l","-a",path};
        execv("./simpledu", arguments);
        //escrever do pipe
    }
    else if(pid > 0) {
        close(fd[WRITE]);
        /*esperar pelo filho e mostrar tamanho?*/
        /*if(waitpid(-1,NULL,0) != pid) {
            fprintf(stderr, "wait error\n");
        }*/
        //ler do pipe
    }
    return 0; 
}

void display(struct ArgumentFlags *args) {
    DIR *dir; 
    char *path = args->path;
    struct dirent *dentry; 
    printf("\nEntered display\n");
    

    if ((dir = opendir(path)) == NULL) {   //opens a directory returning the stream of it or NULL if error
        perror(path); 
        exit(2);
    }  

    chdir(path);  //changes the directory to path
    printf("Ficheiros regulares do directorio '%s'\n", path);  


    while ((dentry = readdir(dir)) != NULL) { 
        /*The readdir() function returns a pointer to a structure representing the directory entry at the
         current position in the directory stream specified by the argument dirp, and positions the 
         directory stream at the next entry. It returns a null pointer upon reaching the end of the 
         directory stream.*/ 
        int type = verifyPath(dentry->d_name);
        struct stat stat_entry = getStat(); 
        char * path;

        switch(type){
            case 0:
                printFile(args, (int)stat_entry.st_size, dentry->d_name);
                break;

            case 1:
                if(strcmp(dentry->d_name, ".")!=0 && strcmp(dentry->d_name, "..")!=0) {
                    if(args->maxDepth){
                        path = buildPath(dentry->d_name, args);
                        printf("\nI'm here bitches : %s\n", path);
                        //forkAux(args, path);
                    }
                    printDir(args, (int)stat_entry.st_size, dentry->d_name);  
                }  
                break;
            case 2:
                if(args->simbolicLinks) {
                    /*seguir no link*/
                }
                else{
                    printLink(args, (int)stat_entry.st_size, dentry->d_name);    
                }
                break;
        }
                       
    }
}

