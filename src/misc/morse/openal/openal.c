#include <AL/al.h>
#include <AL/alc.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

#define SECOND 1
#define SAMPLING_HZ 44100
#define BUFFER_LENGTH (SECOND * SAMPLING_HZ)
#define SOUND_HZ 17700

int main(){
    ALCdevice *device;
    ALCcontext *context;
    ALshort data[BUFFER_LENGTH*2];
    ALuint buffer, source;
    int i;

    // Initialization
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &buffer);

    printf("%d\n",SHRT_MAX);
    // Generate sine wave data
    for (i = 0; i < BUFFER_LENGTH; ++i) {
      data[i*2] = cos(2 * M_PI * SOUND_HZ * i / BUFFER_LENGTH) * SHRT_MAX;
      data[i*2+1] = 0;//-1 * sin(2 * M_PI * SOUND_HZ * i / BUFFER_LENGTH) * SHRT_MAX; 
      // antiphase
    }

    // Output looping sine wave
 
    alBufferData(buffer, AL_FORMAT_STEREO16, data, sizeof(data), BUFFER_LENGTH*2);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_TRUE); 
    alSourcePlay(source);

    // Wait to exit
    printf("Press any key to exit.");
    getchar();

    // Finalization
    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}
