#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>

std::string loadfile(const char *path)
{
    std::ifstream file;
    // open the file stream
    file.open(path);
    // check if opening a file failed
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

    // close the file stream
    file.close();
    return str;
}

int main(int argc, char **argv)
{

    std::string str = loadfile((const char*)"../../shader/plain/plainfs.glsl");

    for(int i=0;i<100;i++){
        //printf("\n%s\n",fstring((char*)"../../shader/plain/plainfs.glsl"));
        printf("%s\n",str.c_str());
    }
}
