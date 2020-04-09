#include "display.h"
pid_t main_prg;
pid_t child;

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
void print(struct ArgumentFlags * args, int size, char* name,int type){
    if(type==0){
        if(!args->all){
            return;
        }
    }
    printf("%-4d \t %s\n", size, name);
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
        if (getpgrp() == main_prg) {
            setpgid(pid, getpid());
        }
        close(fd[READ]);
        dup2(fd[WRITE], STDIN_FILENO);

        char* arguments[12];
        char newpath[100];
        if(n == 1){
            buildPath(path, args,newpath);
        }
        else 
            strcpy(newpath,path);

        getArgv(newpath, args, arguments);
        execv("./simpledu", arguments);
        printf("Exec failed!\n");
    }

    else if(pid > 0) {
        if (getpgrp() == main_prg) { child = pid; }
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

    
    char* parentpid = getenv("MAIN_PID");
    char pid[10];
    sprintf(pid, "%d", getpid());

    struct stat stat_entry;
    int numBlocks;

    if(strcmp(pid, parentpid) == 0) {
        if(lstat(path, &stat_entry) < 0)  //invalid path
            return ;
            
        numBlocks = bytesToBlocks((int)stat_entry.st_blocks, args->blockSize);
        if(args->bytes) {
            size = (int)stat_entry.st_size; 
        }                
        else {
            size = numBlocks;
        }
    }  
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
        stat_entry = getStat(); 
        
        numBlocks = bytesToBlocks((int)stat_entry.st_blocks, args->blockSize);
        int filesize = 0, childsize = 0;
        switch(type){
            case 0:
                if(args->bytes) {
                    filesize = (int)stat_entry.st_size;
                }
                else {
                    filesize = numBlocks;
                }
                size += filesize;
                print(args, filesize, fullpath,0);
                regEntry(numBlocks, fullpath);
                break;

            case 1:
                if(strcmp(dentry->d_name, ".")!=0 && strcmp(dentry->d_name, "..")!=0) {
                    if(args->bytes) {
                        filesize = (int)stat_entry.st_size;
                    }
                    else {
                        filesize = numBlocks;
                    }

                    if(args->maxDepth > 1) {
                        childsize=forkAux(args, dentry->d_name,1);
                        
                    } 
                    
                    filesize += childsize;
                    if(!args->noSubDir) {
                        size+=filesize;
                    }
                                       

                    print(args, filesize, fullpath,1);
                    regEntry(numBlocks, fullpath);
                }  
                

                break;
            case 2:
                if(args->simbolicLinks) { 
                    while(1){
                        if((len=readlink(fullpath,buf,sizeof(buf)-1))!=-1){//building the path to the simbolic link
                            
                            buf[len]='\0'; 
                            strcat(filename,buf);
                            int aux= verifyPath(filename);
                            stat_entry = getStat();
                            numBlocks = bytesToBlocks((int)stat_entry.st_blocks, args->blockSize);
                            
                            if(aux ==1){//if it is a directory 
                                strcat(filename,"/");
                                if(args->bytes) {
                                    filesize = (int)stat_entry.st_size;
                                }
                                else {
                                    filesize = numBlocks;
                                }

                                if(args->maxDepth > 1) {
                                    childsize=forkAux(args, dentry->d_name,1);
                                } 

                                filesize += childsize;
                                
                                if(!args->noSubDir) {
                                    size+=filesize; 
                                }
                                             
                                print(args, filesize, fullpath,2);
                                free(filename);
                                regEntry(numBlocks, fullpath);
                                break;
                            }
                            else if(aux ==0){ //if it is a file
                                
                                if(args->bytes) {
                                    filesize = (int)stat_entry.st_size;
                                }
                                else {
                                    filesize = numBlocks;
                                }
                                size += filesize;
                                print(args,size, fullpath,2);
                                regEntry(numBlocks, fullpath);
                                free(filename);
                                break;
                            }
                            else if(aux == 2) {
                                continue;
                            }
                            else {
                                break;
                            }
                            //dont know if we need to check if it is another link--we do 
                        }
                    }
                }
                else{
                    print(args, size, fullpath,2);    
                }
                break;
        }                  
    }
    
    sprintf(line, "%d", size);
    n = strlen(line);
    if(strcmp(pid, parentpid)) {
        write(STDIN_FILENO, line, n);
    } 
    else {
       print(args, size, path, 1);
    }
    
    regSendPipe(line);
}

