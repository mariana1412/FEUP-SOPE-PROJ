#include "display.h"

int bytesToBlocks(int bytes, int blocksize) {
    return bytes/blocksize + 1;
}

char * buildPath(char * dirname) {
    char *cwd;
    char aux[PATH_MAX + 1];
    cwd = getcwd(aux, PATH_MAX + 1);
    strcat(cwd, "/");
    strcat(cwd, dirname);

    return cwd;
}

/*int pipe(){
    int fd[2];
    pipe(fd);
    if (pid ==0){
        close(fd[READ]);
        dup2(fd[WRITE],STDOUT_FILENO);
        

    }
    else{
        close(fd[WRITE]);

    }


}*/
void display(struct ArgumentFlags *args) {
    DIR *dir; 
    char *path = args->path;
    struct dirent *dentry; 
    

    if ((dir = opendir(path)) == NULL) {   
        perror(path); 
        exit(2);
    }  

    chdir(path);  
    printf("Ficheiros regulares do directorio '%s'\n", path);  


    while ((dentry = readdir(dir)) != NULL) {   
        int type = verifyPath(dentry->d_name);
        struct stat stat_entry = getStat(); 
        pid_t pid;
        int status;

        switch(type){
            case 0:
                if(args->all){
                    if(args->bytes){
                        printf("Bytes: %d, File: %s\n", (int)stat_entry.st_size, dentry->d_name);
                    }
                    else{
                        int numBlocks = bytesToBlocks(stat_entry.st_size, args->blockSize);
                        printf("Blocks: %d, File: %s\n", numBlocks, dentry->d_name);
                    }
                }
                break;

            case 1:
                if((pid = fork()) < 0)
                    fprintf(stderr, "fork error\n");
                else if(pid == 0) {
                    char* dirpath = buildPath(dentry->d_name);
                    char * arguments = getArgv(dirpath, args);
                    execv("./simpledu", &arguments);
                }
                else if(pid > 0) {
                    /*esperar pelo filho e mostrar tamanho?*/
                    if(wait(&status) != pid) {
                        fprintf(stderr, "wait error\n");
                    }
                }
                if(args->bytes){
                    printf("Bytes: %d, Dir: %s\n", (int)stat_entry.st_size, dentry->d_name);
                }
                else{
                    int numBlocks = bytesToBlocks(stat_entry.st_size, args->blockSize);                        
                    printf("Blocks: %d, Dir: %s\n", numBlocks, dentry->d_name);
                }
                break;

            case 2:
                if(args->simbolicLinks) {
                    /*seguir no link*/
                }
                else{
                    if(args->all){
                        if(args->bytes){
                            printf("Bytes: %d, Link: %s\n", (int)stat_entry.st_size, dentry->d_name);
                        }
                        else{
                            int numBlocks = bytesToBlocks(stat_entry.st_size, args->blockSize);
                            printf("Blocks: %d, Link: %s\n", numBlocks, dentry->d_name);
                        }
                    }
                }
                break;
        }
        
        

                       
    }
}

