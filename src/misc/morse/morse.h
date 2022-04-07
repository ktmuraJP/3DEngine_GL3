#ifndef MORSECODE_H
#define MORSECODE_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<string>
#include<map>
#include<vector>
#include<iostream>
#include<math.h>
#include<AL/al.h>
#include<AL/alc.h>
#include<AL/alut.h>
#define SAMPLINGRATE 44100
extern std::map<std::string, std::vector<int>> morse_code;
class SEObject
{
public:
    SEObject(unsigned int freq=870);
    ~SEObject();
    void Play();
    void Stop();
    void Destroy(); 
private:
    ALCdevice *device; 
    ALCcontext *context; 
    unsigned int buffer;
    unsigned int source;
    signed short* wav_data;
};
int get_utf8char_byte( char* p );
std::vector<int> string_to_morse(char *str);
std::vector<int> format_code(std::vector<int> code);
void morse_play(std::vector<int> code,int interval);
#endif
