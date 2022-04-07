#ifndef MISC_H
#define MISC_H
#include <string>
unsigned int SetupTexture(const char* imgpath);
unsigned int SetupTexture(const char* imgpath, int *width, int *height);
std::string loadfile(const char *path);
void filewrite(const char *filename,const char *fstring);
float randf(int min, int max);
float lerp(float a, float b, float f);
#endif
