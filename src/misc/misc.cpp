#include"misc.h"

float randf(int min, int max)
{
    return (float)((rand()%(2*max)) + min) + (float)(rand()+1)/(float)RAND_MAX;
}
float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}
