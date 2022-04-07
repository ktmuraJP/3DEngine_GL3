#include "src.h"
#include "misc.h"
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
char* fileread(const char *path)
{
    FILE *fp;
    char *buf;
    int size;
    int i;

    if (path){
        fp = fopen(path, "r"); 
        if (fp==NULL){
            fprintf(stderr,"file pointer NULL\n");
            exit(EXIT_SUCCESS);
        }
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        buf = (char*)malloc(size);
        for(i=0;i<size;++i)
            buf[i] = fgetc(fp);
         
        buf[i] = EOF;
        fclose(fp);
        return buf;
    }else{
        fprintf(stderr,"path set to NULL\n"); 
        return NULL;
    }
}

std::string loadfile(const char *path)
{
    char *s = fileread(path);
    if(s==NULL)
        printf("NULL\n");
    std::string str(s);
    return str;
}

/*
std::string loadfile(const char *path)
{
    std::ifstream file;

    file.open(path);
    if (file.fail()) {
        fprintf(stderr,"file input error\n");
        file.close();
        exit(1);
    }
    std::string str;
    std::string line;
    while (getline(file, line))
        str = str + line + "\n";
    str.pop_back();

    file.close();
    return str;
}
*/
void filewrite(const char *filename,const char *fstring)
{
    FILE *fp;
    
    fp = fopen(filename,"w");
    if(fp == NULL){
        fprintf(stderr,"file output error\n");
        exit(EXIT_FAILURE);
    }
    fputs(fstring,fp);
    fclose(fp);
}
