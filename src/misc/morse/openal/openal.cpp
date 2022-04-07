#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<AL/al.h>
#include<AL/alc.h>
#include<AL/alut.h>
#define FILENAME "../../data/sound/yarimasune.wav"
#define SAMPLINGRATE 44100
#define _USE_MATH_DEFINES

int main()
{
    ALCdevice *device = alcOpenDevice(NULL);
    ALCcontext *context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    //バッファ(保存領域)とソース(音源)を宣言
    ALuint buffer;
    ALuint source;
    //それを生成
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    //new演算子で音源を入れる配列を動的確保
    signed short* wav_data = new signed short[SAMPLINGRATE];
    //ここで440Hzのsin波を1秒間分生成
    for (int i = 0; i < SAMPLINGRATE; i++) {
        wav_data[i] = 32767 * sin(2 * M_PI*i * 440 / SAMPLINGRATE);
    }
    //バッファに音源データを入れる
    alBufferData(buffer, 
            AL_FORMAT_MONO16, 
            &wav_data[0], 
            SAMPLINGRATE * sizeof(signed short), 
            SAMPLINGRATE);
    //ソースにバッファを適用
    alSourcei(source, AL_BUFFER, buffer);
    //ループ再生をON
    alSourcei(source, AL_LOOPING, AL_TRUE);
    //ソースを再生！
    alSourcePlay(source);

    //ここで一時停止


    //動的確保したメモリを破棄
    delete[] wav_data;
    // バッファの破棄
    alDeleteBuffers(1, &buffer);
    // ソースの破棄
    alDeleteSources(1, &source);

    //OpenALの後始末
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return 0;

    /*
    ALuint buffer, source;
    ALint state;

    // Initialize the environment
    alutInit(0, NULL);

    // Capture errors
    alGetError();

    // Load pcm data into buffer
    buffer = alutCreateBufferFromFile(FILENAME);

    // Create sound source (use buffer to fill source)
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    // Play
    alSourcePlay(source);

    // Wait for the song to complete
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    // Clean up sources and buffers
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);

    // Exit everything
    alutExit();

    return 0;
    */
}
