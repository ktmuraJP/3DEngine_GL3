#include"morse.h"

std::map<std::string, std::vector<int>> morse_code{
   {"イ"    ,    {1,3}},         {"い"    ,    {1,3}},        
   {"ロ"    ,    {1,3,1,3}},     {"ろ"    ,    {1,3,1,3}},  
   {"ハ"    ,    {3,1,1,1}},     {"は"    ,    {3,1,1,1}},  
   {"ニ"    ,    {3,1,3,1}},     {"に"    ,    {3,1,3,1}},  
   {"ホ"    ,    {3,1,1}},       {"ほ"    ,    {3,1,1}},    
   {"ヘ"    ,    {1,2,2}},       {"へ"    ,    {1,2,2}},    
   {"ト"    ,    {1,1,3,1,1}},   {"と"    ,    {1,1,3,1,1}},
   {"チ"    ,    {1,1,3,1,}},    {"ち"    ,    {1,1,3,1,}}, 
   {"リ"    ,    {3,3,1}},       {"り"    ,    {3,3,1}},    
   {"ヌ"    ,    {1,1,1,1,}},    {"ぬ"    ,    {1,1,1,1,}}, 
   {"ル"    ,    {3,1,3,3,1}},   {"る"    ,    {3,1,3,3,1}},
   {"ヲ"    ,    {1,3,3,3,}},    {"を"    ,    {1,3,3,3,}}, 
   {"ワ"    ,    {3,1,3,}},      {"わ"    ,    {3,1,3,}},   
   {"カ"    ,    {1,3,1,1,}},    {"か"    ,    {1,3,1,1,}}, 
   {"ヨ"    ,    {3,3}},         {"よ"    ,    {3,3}},      
   {"タ"    ,    {3,1}},         {"た"    ,    {3,1}},      
   {"レ"    ,    {3,3,3,}},      {"れ"    ,    {3,3,3,}},   
   {"ソ"    ,    {3,3,3,1}},     {"そ"    ,    {3,3,3,1}},  
   {"ツ"    ,    {1,3,3,1}},     {"つ"    ,    {1,3,3,1}},  
   {"ネ"    ,    {3,3,1,3}},     {"ね"    ,    {3,3,1,3}},  
   {"ナ"    ,    {1,3,1}},       {"な"    ,    {1,3,1}},    
   {"ラ"    ,    {1,1,1}},       {"ら"    ,    {1,1,1}},    
   {"ム"    ,    {3}},           {"む"    ,    {3}},        
   {"ウ"    ,    {1,1,3}},       {"う"    ,    {1,1,3}},    
   {"ヰ"    ,    {1,3,1,1,3}},   {"ゐ"    ,    {1,3,1,1,3}},
   {"ノ"    ,    {1,1,3,3}},     {"の"    ,    {1,1,3,3}},  
   {"オ"    ,    {1,3,1,1,1}},   {"お"    ,    {1,3,1,1,1}},
   {"ク"    ,    {1,1,1,3}},     {"く"    ,    {1,1,1,3}},  
   {"ヤ"    ,    {1,3,3}},       {"や"    ,    {1,3,3}},    
   {"マ"    ,    {3,1,1,3}},     {"ま"    ,    {3,1,1,3}},  
   {"ケ"    ,    {3,1,3,3}},     {"け"    ,    {3,1,3,3}},  
   {"フ"    ,    {3,3,1,1}},     {"ふ"    ,    {3,3,1,1}},  
   {"コ"    ,    {3,3,3,3}},     {"こ"    ,    {3,3,3,3}},  
   {"エ"    ,    {3,1,3,3,3}},   {"え"    ,    {3,1,3,3,3}},
   {"テ"    ,    {1,3,1,3,3}},   {"て"    ,    {1,3,1,3,3}},
   {"ア"    ,    {3,3,1,3,3}},   {"あ"    ,    {3,3,1,3,3}},
   {"サ"    ,    {3,1,3,1,3}},   {"さ"    ,    {3,1,3,1,3}},
   {"キ"    ,    {3,1,3,1,1}},   {"き"    ,    {3,1,3,1,1}},
   {"ユ"    ,    {3,1,1,3,3}},   {"ゆ"    ,    {3,1,1,3,3}},
   {"メ"    ,    {3,1,1,1,3}},   {"め"    ,    {3,1,1,1,3}},
   {"ミ"    ,    {1,1,3,1,3}},   {"み"    ,    {1,1,3,1,3}},
   {"シ"    ,    {3,3,1,3,1}},   {"し"    ,    {3,3,1,3,1}},
   {"ヱ"    ,    {1,3,3,1,1}},   {"ゑ"    ,    {1,3,3,1,1}},
   {"ェ"    ,    {1,3,3,1,1}},   {"え"    ,    {1,3,3,1,1}},
   {"ヒ"    ,    {3,3,1,1,3}},   {"ひ"    ,    {3,3,1,1,3}},
   {"モ"    ,    {3,1,1,3,1}},   {"も"    ,    {3,1,1,3,1}},
   {"セ"    ,    {1,3,3,3,1}},   {"せ"    ,    {1,3,3,3,1}},
   {"ス"    ,    {3,3,3,1,3}},   {"す"    ,    {3,3,3,1,3}},
   {"ン"    ,    {1,3,1,3,1}},   {"ん"    ,    {1,3,1,3,1}},
   {"゛"    ,    {1,1}},   {"\""    ,    {1,3,1,1,3}},
   {"ー"    ,    {1,3,3,1,3}},   {"-"     ,    {3,1,1,1,1,3}},
   {"("     ,    {3,1,3,3,1,3}}, {"（"    ,    {3,1,3,3,1,3}},
   {")"     ,    {1,3,1,1,3,1}}, {"）"    ,    {1,3,1,1,3,1}},
   {" "     ,    {-7}},          {"　"    ,    {-7}},
   {"１"    ,    {1,3,3,3,3}},   {"1"     ,    {1,3,3,3,3}},
   {"２"    ,    {1,1,3,3,3}},   {"2"     ,    {1,1,3,3,3}},
   {"３"    ,    {1,1,1,3,3}},   {"3"     ,    {1,1,1,3,3}},
   {"４"    ,    {1,1,1,1,3}},   {"4"     ,    {1,1,1,1,3}},
   {"５"    ,    {1,1,1,1,1}},   {"5"     ,    {1,1,1,1,1}},
   {"６"    ,    {3,1,1,1,1}},   {"6"     ,    {3,1,1,1,1}},
   {"７"    ,    {3,3,1,1,1}},   {"7"     ,    {3,3,1,1,1}},
   {"８"    ,    {3,3,3,1,1}},   {"8"     ,    {3,3,3,1,1}},
   {"９"    ,    {3,3,3,3,1}},   {"9"     ,    {3,3,3,3,1}},
   {"０"    ,    {3,3,3,3,3}},   {"0"     ,    {3,3,3,3,3}}, 
   {"A"     ,    {1,3}},         {"a"     ,    {1,3}},    
   {"B"     ,    {3,1,1,1}},     {"b"     ,    {3,1,1,1}},
   {"C"     ,    {3,1,3,1}},     {"c"     ,    {3,1,3,1}},
   {"D"     ,    {3,1,1}},       {"d"     ,    {3,1,1}},  
   {"E"     ,    {1}},           {"e"     ,    {1}},      
   {"F"     ,    {1,1,3,1}},     {"f"     ,    {1,1,3,1}},
   {"G"     ,    {3,3,1, }},     {"g"     ,    {3,3,1, }},
   {"H"     ,    {1,1,1,1}},     {"h"     ,    {1,1,1,1}},
   {"I"     ,    {1,1}},         {"i"     ,    {1,1}},    
   {"J"     ,    {1,3,3,3}},     {"j"     ,    {1,3,3,3}},
   {"K"     ,    {3,1,3}},       {"k"     ,    {3,1,3}},  
   {"L"     ,    {1,3,1,1}},     {"l"     ,    {1,3,1,1}},
   {"M"     ,    {3,3}},         {"m"     ,    {3,3}},    
   {"N"     ,    {3,1}},         {"n"     ,    {3,1}},    
   {"O"     ,    {3,3,3}},       {"o"     ,    {3,3,3}},  
   {"P"     ,    {1,3,3,1}},     {"p"     ,    {1,3,3,1}},
   {"Q"     ,    {3,3,1,3}},     {"q"     ,    {3,3,1,3}},
   {"R"     ,    {1,3,1}},       {"r"     ,    {1,3,1}},  
   {"S"     ,    {1,1,1}},       {"s"     ,    {1,1,1}},  
   {"T"     ,    {3}},           {"t"     ,    {3}},      
   {"U"     ,    {1,1,3}},       {"u"     ,    {1,1,3}},  
   {"V"     ,    {1,1,1,3}},     {"v"     ,    {1,1,1,3}},
   {"W"     ,    {1,3,3}},       {"w"     ,    {1,3,3}},  
   {"X"     ,    {3,1,1,3}},     {"x"     ,    {3,1,1,3}},
   {"Y"     ,    {3,1,3,3}},     {"y"     ,    {3,1,3,3}},
   {"Z"     ,    {3,3,1,1}},     {"z"     ,    {3,3,1,1}}
};


SEObject::SEObject(unsigned int freq)
{
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    wav_data = new signed short[SAMPLINGRATE];

    for (int i = 0; i < SAMPLINGRATE; i++) {

        wav_data[i] = 32767 * sin(i * 20 * freq / M_PI / SAMPLINGRATE);
    }

    alBufferData(buffer, 
            AL_FORMAT_MONO16, 
            &wav_data[0], 
            SAMPLINGRATE * sizeof(signed short), 
            SAMPLINGRATE);

    alSourcei(source, AL_BUFFER, buffer);
    alSourcei(source, AL_LOOPING, AL_FALSE);

    delete[] wav_data;
}
void SEObject::Play()
{
    alSourcePlay(source);
}

void SEObject::Stop()
{
    alSourceStop(source);
}

void SEObject::SetVolume(float volume)
{
    alSourcef(source, AL_GAIN, volume);
}

void SEObject::Destroy()
{
}

SEObject::~SEObject() 
{
    alDeleteBuffers(1, &buffer);
    alDeleteSources(1, &source);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}



int get_utf8char_byte( char* p )
{
    int byte = 0;
    
    for( int i = 0; i < 6; ++i )
        if( ( ( *p >> ( 7 - i ) ) & 0x01 ) == 0 ){
            byte = i;
            break;
        }
    if( byte == 6 )
        return -1;
    else if( byte == 1 )
        return -2;
    else if( byte != 0 )
        --byte;
    for( int i = 1; i < byte; ++i )
        if( ( ( p[ i ] >> 6 ) & 0x03 ) != 0x02 )
            return -3;
         
    return byte + 1;
}

std::vector<int> string_to_morse(char *str)
{
    int length = strlen(str);
    std::vector<int> code;
    while( *str ){
        int utf8char_size;
        char utf8char[ 20 ];
        utf8char_size = get_utf8char_byte( str );

        strncpy( utf8char, str, utf8char_size );
        utf8char[ utf8char_size ] = '\0';
        //printf("%s :: ",utf8char);
        for(int i : morse_code[utf8char]){
            //printf("%d",i);
            code.push_back(i);
            if(utf8char != " " || utf8char != "　")
                code.push_back(-1);
        }
        //printf("\n");
        if(utf8char != " " || utf8char != "　")
            code.push_back(-3);

        str += utf8char_size;
    }
    code.push_back(0);
    return code;
}

std::vector<float> format_code(std::vector<int> code)
{ 
    std::vector<float> formatted;
    for(int op : code){
        if(op > 0)
            for(int i=0; i<op*2; i++)
                formatted.push_back(1.0f);
        if(op < 0)
            for(int i=0; i<-op*2; i++)
                formatted.push_back(0.0f);
    }

    return formatted;
}

void morse_play(std::vector<int> code,int interval)
{

    SEObject sound = SEObject(877);

    for(int op : code){
        if(op > 0){
            sound.Play();
            fprintf(stderr,"%c",op == 3 ? '_' : '.');
            usleep(interval * op);
            sound.Stop();
        }
        else if(op < 0){
            sound.Stop();
            fprintf(stderr,"%s",op == -3 ? " " : op == -7 ? "(space)" : "");
            usleep(interval * -op);
            sound.Stop();
        }
    }
}
/*
int main(int argc, char **argv)
{
    char *str = argv[1];
    std::vector<int> code = string_to_morse(str);
    std::vector<int> formatted = format_code(code);
    for(int op : formatted)
        printf("%d",op);
    morse_play(code, atoi(argv[2]));
}
*/

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
