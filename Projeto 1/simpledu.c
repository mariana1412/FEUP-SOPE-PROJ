#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "macros.h"
#include "simpledu.h"
#include <string.h>
#include <sys/stat.h>

int idArguments(char *arg){
    
    char *arguments[11] = {"-a", "--all", "-b", "--bytes", "-B", "--block-size=", "-L", "--dereference", "-S", "--separate-dirs", "--max-depth="};
    
    
    //nota: strcmp() retorn 0 quando são iguais
    if(!strcmp(arg, arguments[0]) || !strcmp(arg, arguments[1])){ 
        return ALL;
    }
    
    if(!strcmp(arg, arguments[2]) || !strcmp(arg, arguments[3])){ 
        return BYTES;
    }

    if(!strcmp(arg, arguments[4]) || !strncmp(arg, arguments[5], 13)){ 
        return BLOCK_SIZE;
    }
    
    if(!strcmp(arg, arguments[6]) || !strncmp(arg, arguments[7], 13)){ 
        return DEREFERENCE;
    }

    if(!strcmp(arg, arguments[8]) || !strncmp(arg, arguments[9], 15)){ 
        return DEREFERENCE;
    }

    if(!strncmp(arg, arguments[10], 12)){
        return MAX_DEPTH;
    }

    return -1;
   
}


int verifyPath(char *path){

    struct stat pathStat;

    if(stat(path, &pathStat) < 0)  //invalid path
        return -1;

    else if(S_ISREG(pathStat.st_mode)) //file
        return 0;

    else if(S_ISDIR(pathStat.st_mode)) //directory
        return 1;
    
    
    return -1;
}

void initArgumentFlags(struct ArgumentFlags *args){
    args->all = 0;
    args->bytes = 0;
    args->blockSize = 1024;
    args->countLinks = 1;
    args->simbolicLinks = 0;
    args->noSubDir = 0;
    args->maxDepth = 0;
}



int parseArguments(int argc, char *argv[], struct ArgumentFlags *args){ //tem sempre o l
    
    //verificar se path é valido ('.' é pasta atual)
    //no caso de idArguments retornar o valor de "-B", obrigatoriamente o argv[seguinte] é um número  
    //no caso de "qualquercoisa= ", é preciso verificar se tem inteiro a seguir
    
    for(int i = 3; i < argc; i++){
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
                    i++;
                    args->blockSize = atoi(argv[i]);
                }
                else {
                    char *num = strstr(argv[i], "=");
                    args->blockSize = atoi(num+1);
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
                args->maxDepth = atoi(num+1);
                break;
            }
                

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
    
}

int main(int argc, char *argv[], char *envp[]){
    struct ArgumentFlags args;

    if (argc == 1 || argc > 10) {   
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]", argv[0]); 
        exit(1);
    }
    else if (argc == 2){
        if(strcmp(argv[1], "-l") != 0) {
            printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]", argv[0]);
            exit(1);
        }
        printf("Fazer comando por omissão\n");
        return 0;
        //fazer por omissão -> "simpledu -l"
        /*
        1.apresenta o espaço ocupado em número de blocos de 1024 bytes;
        2.apenas lista diretórios;
        3.não segue links simbólicos,
        4.contabiliza uma única vez cada ficheiro;
        5.apresenta de forma cumulativa o tamanho de subdiretórios e ficheiros incluídos;
        6.não restringe os níveis de profundidade na estrutura de diretórios.
        */
        //defaultAction();
        //return 0;
        
    }

    if(strcmp(argv[1], "-l") != 0) {
        printf("Usage: %s -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]", argv[0]);
        exit(1);
    }
    
    if (verifyPath(argv[2]) == -1){//invalid path
        printf("Error: No such file or directory\n");
        exit(2);
    }
    
    initArgumentFlags(&args);

    if(parseArguments(argc, argv, &args) != 0){
        printf("Error: There was an error on the provided arguments\n");
    }


    checkFlags(&args);
    return 0; 
}

/*

DIR *dir; 
    int line; 
    struct dirent *dentry; 
    struct stat stat_entry; 


    if (argc != 2) {   
        printf("Usage: %s <dir_path>\n", argv[0]); 
        return 1;  
    }

    if ((dir = opendir(argv[1])) == NULL) {   
        perror(argv[1]); 
        return 2; 
    }  

    chdir(argv[1]);  
    printf("Ficheiros regulares do directorio '%s'\n", argv[1]);  

    line = 1; 

    while ((dentry = readdir(dir)) != NULL) {   
        stat(dentry->d_name, &stat_entry); 
        if (S_ISREG(stat_entry.st_mode)) {    
            printf("%-25s%12d%3d\n", dentry->d_name,(int)stat_entry.st_size,(int)stat_entry.st_nlink); 
        if (line++ % 20 == 0) {      
            printf("Press <enter> to continue");      
            getchar();        
        }

        }
    }
    


*/