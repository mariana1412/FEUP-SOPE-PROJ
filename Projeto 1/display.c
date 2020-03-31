#include "display.h"



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

        switch(type){

            case 0:

                if(args->all){
                    if(args->bytes){
                        printf("Bytes: %d, File: %s\n", (int)stat_entry.st_size, dentry->d_name);
                    }
                    else{
                        int numBlocks = stat_entry.st_size / args->blockSize;
                        printf("Blocks: %d, File: %s\n", numBlocks, dentry->d_name);
                    }
                }
                break;

            case 1:
                if(args->all){
                    if(args->bytes){
                        printf("Bytes: %d, Dir: %s\n", (int)stat_entry.st_size, dentry->d_name);
                    }
                    else{
                        int numBlocks = stat_entry.st_size / args->blockSize;
                        printf("Blocks: %d, Dir: %s\n", numBlocks, dentry->d_name);
                    }
                }
                break;

            case 2:
                if(args->simbolicLinks) {
                    if(args->all){
                        if(args->bytes){
                            printf("Bytes: %d, Link: %s\n", (int)stat_entry.st_size, dentry->d_name);
                        }
                        else{
                            int numBlocks = stat_entry.st_size / args->blockSize;
                            printf("Blocks: %d, Link: %s\n", numBlocks, dentry->d_name);
                        }
                    }
                }
                break;
        }
        
        

                       
    }
}

