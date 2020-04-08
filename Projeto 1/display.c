#include "display.h"
int bytesToBlocks(int blocks, int blocksize) {
    return blocks * 512 / blocksize;
}

void buildPath(char * dirname, struct ArgumentFlags * args, char * aux) {
    strcpy(aux, args->path);
    if(aux[strlen(aux)-1] != '/'){
        strcat(aux, "/");
    }
    strcat(aux, dirname);
    if(aux[strlen(aux)-1] != '/'){
        strcat(aux, "/");
    }
}
void print(struct ArgumentFlags * args, struct stat stat_entry, char* name,int type){
    if(type==0){
        if(!args->all){
            return;
        }
    }
    int numBlocks = bytesToBlocks(stat_entry.st_blocks, args->blockSize);
    if(args->bytes){
        fflush(stdout);
        printf("%-4d \t %s\n", (int)stat_entry.st_size, name);
    }
    else{
        fflush(stdout);
        printf("%-4d \t %s\n", numBlocks, name);
    }
    regEntry(numBlocks, name);
}
int forkAux(struct ArgumentFlags * args, char * path, int n){
    int childsize = 0;
    pid_t pid;
    int fd[2];
    pipe(fd);
    pid=fork();

    
    if(pid < 0)
        fprintf(stderr, "fork error\n");

    else if(pid == 0) {
        close(fd[READ]);
        dup2(fd[WRITE], STDIN_FILENO);

        char* arguments[12];
        char newpath[100];
        if(n ==1)
            buildPath(path, args,newpath);
        else 
            strcpy(newpath,path);
        getArgv(newpath, args, arguments);
        execv("./simpledu", arguments);
        printf("Exec failed!\n");
    
        //escrever para o pipe
    }
    else if(pid > 0) {
        if(wait(NULL) != pid) {
            fprintf(stderr, "wait error\n");
        }
        char* auxPath = args->path;
        close(fd[WRITE]);
        char line[10];
        int n = read(fd[READ], line, 10);
        line[n] ='\0';
        regRecvPipe(line);
        sscanf(line, "%d", &childsize);
 
 
        args->path = auxPath;
        //ler do pipe
    }

    return childsize; 
}

void display(struct ArgumentFlags *args) {
    
    DIR *dir; 
    char *path = args->path;
    ssize_t len;
    char buf[1024];
    char * filename= malloc(1024);
    struct dirent *dentry; 
    int size=0;
    char line[10];
    int n;


    if ((dir = opendir(path)) == NULL) {
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
        
        int type = verifyPath(fullpath);
        struct stat stat_entry = getStat(); 
        
        
        switch(type){
            case 0:
                if(args->bytes) {
                    size += (int)stat_entry.st_size;
                }
                else {
                    size += bytesToBlocks((int)stat_entry.st_blocks, args->blockSize);
                }
                print(args, stat_entry, fullpath,0);
                //printf("SIZE : %d\n", size);
                break;

            case 1:
                if(strcmp(dentry->d_name, ".")!=0 && strcmp(dentry->d_name, "..")!=0) {
                    if(args->maxDepth > 0) {
                        size+=forkAux(args, dentry->d_name,1);
                        if(!args->noSubDir) {
                            if(args->bytes) {
                                size += (int)stat_entry.st_size;
                            }
                            else {
                                size += bytesToBlocks((int)stat_entry.st_blocks, args->blockSize);
                            }
                        }
                    } 
                    print(args, stat_entry, fullpath,1);
                }  
                //print(args, stat_entry, fullpath,1);  
                //falta printar no fim o diretorio pai de todos e o tamanho
                break;
            case 2:
                if(args->simbolicLinks) { /*seguir no link*/
                    if((len=readlink(fullpath,buf,sizeof(buf)-1))!=-1){//building the path to the simbolic link
                        buf[len]='\0'; 
                        strcat(filename,buf);
                        int aux= verifyPath(filename);
                       // printf("Filename: %s \n",filename);
                        //printf("Fullpath :  %s \n",fullpath);
                        if(aux ==1){//if it is a directory
                       // printf("Hello!\n");
                            strcat(filename,"/");
                            forkAux(args, fullpath,2);                
                            print(args, stat_entry, fullpath,2);
                            free(filename);
                        }
                        else{ //if it is a file
                        //printf("Da fuck!\n");
                            print(args,stat_entry, fullpath,2);
                            free(filename);
                        }
                        //dont know if we need to check if it is another link--we do 
                        //falta fazer a soma do symblink
                    }
                }
                else{
                    print(args, stat_entry, fullpath,2);    
                }
                break;
        }
                       
    }
    sprintf(line, "%d", size);
    n = strlen(line);
    printf("sending to pipe: %s\n", line);
    write(STDIN_FILENO, line, n);
    regSendPipe(line);
}

