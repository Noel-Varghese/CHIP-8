#include <SDL2/SDL.h>
#include "soundSDL.h"

const int SR = 44100;//sample rate
const int AMP = 28000;//amplitude
const double FREQ = 440.0;//determines the frequency

void audioCallBack(void* userData, Uint8* stream, int len){
    static double phase = 0.0;
    double increment = (2.0*M_PI*FREQ)/SR;
    for(int i = 0;i < len;i+=2){
        short sample = static_cast<short>(AMP * sin(phase));
        phase += increment;
        if(phase >= 2.0 * M_PI){
            phase -= 2.0*M_PI;
        }
        stream[i] = sample & 0xFF;//low byte
        stream[i+1] = (sample >> 8) & 0xFF;//high byte

    }
}

TSoundSDL::TSoundSDL(){
    m_logger = TLogger::getInstance();
}

TSoundSDL::~TSoundSDL(){
    deinit();
}

void TSoundSDL::init(){
    if(SDL_Init(SDL_INIT_AUDIO)<0){
        std::string errorSDL(SDL_GetError());
        m_logger->log("SDL sound initialization error"+ errorSDL, ELogLevel::ERROR);
        exit(1);
    }
    SDL_zero(m_audio);
    m_audio.freq = SR;
    m_audio.format = AUDIO_S16SYS;//uses 16 bit samples
    m_audio.channels = 1;//for mono audio that chip8 uses
    m_audio.samples = 4096;
    m_audio.callback = audioCallBack;
    m_audio.userdata = nullptr;
    if(SDL_OpenAudio(&m_audio, nullptr) < 0){
        std::string errorSdl(SDL_GetError());
        m_logger->log("Failed to open audio device: " + errorSdl, ELogLevel::ERROR);
        exit(1);
    }

}

void TSoundSDL::playTune(){
    SDL_PauseAudio(0);//to play sound
}

void TSoundSDL::pauseT(){
    SDL_PauseAudio(1);//to not play sound
}

void TSoundSDL::deinit(){
    SDL_CloseAudio();
    //SDL_Quit();
}
