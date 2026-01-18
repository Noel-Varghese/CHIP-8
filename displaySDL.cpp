#include <cstdint>
#include <SDL3/SDL.h>

#include "displaySDL.h"
using namespace std;

TDisplaySDL::TDisplaySDL(){
    m_logger = TLogger::getInstance();
    m_window = nullptr;
    m_rend = nullptr;
    m_texture = nullptr;
    m_scale = 20;//for the window 64*20 = 1280 and 32*20 = 640
}
TDisplaySDL::~TDisplaySDL(){
    void deinit();
}

void TDisplaySDL::init(){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        string errorSdl(SDL_GetError());
        m_logger->log("SDL initialization error: "+ errorSdl, ELogLevel::ERROR);
        exit(1);
    }
    m_window = SDL_CreateWindow(
        "CHIP-8", 
        1280, 720, 0
    );//used to create a window
    if(!m_window){
        //error in window creation
        string errorWindow(SDL_GetError());
        m_logger->log("ERROR in creating a Window: "+errorWindow, ELogLevel::ERROR);
        exit(1);
    }
    m_rend = SDL_CreateRenderer(m_window, nullptr);
    if(!m_rend){
        string errorRender(SDL_GetError());
        m_logger->log("ERROR during rendering: "+errorRender, ELogLevel::ERROR);
        exit(1);
    }
    m_texture = SDL_CreateTexture(
        m_rend, 
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        64,
        32
    );
    if(!m_texture){
        string errorTexture(SDL_GetError());
        m_logger->log("ERROR in texture: "+ errorTexture, ELogLevel::ERROR);
        exit(1);
    }

}

void TDisplaySDL::draw(uint8_t framebuffer[][64], uint16_t height, uint16_t width){
    void* pixels = nullptr;
    int pitch = 0;
    if(SDL_LockTexture(m_texture, nullptr, &pixels, &pitch)!=0){
        string errortexture(SDL_GetError());
        m_logger->log("Texture error: "+errortexture, ELogLevel::ERROR);
        return;
    }
    uint32_t* textPixels = static_cast<uint32_t*>(pixels);
    for(int y =0 ;y<height;y++){
        for(int x=0;x<width;x++){
            uint32_t color = framebuffer[y][x]?0xFF00FF00/*green*/:0xFF000000/*Black*/;
            textPixels[y*(pitch/4)+x] = color;
        }
    }   
    SDL_UnlockTexture(m_texture);
}

void TDisplaySDL::update(){
    SDL_RenderClear(m_rend);

    SDL_FRect dst;
    dst.x = 0.0f;
    dst.y = 0.0f;
    dst.w = 64.0f*m_scale;
    dst.h = 32.0f*m_scale;

    SDL_RenderTexture(m_rend, m_texture, nullptr, &dst);
    SDL_RenderPresent(m_rend);

    SDL_Event e;
    while(SDL_PollEvent(&e)){
        if(e.type == SDL_EVENT_QUIT){
            m_logger->log("Closing Emulator: ", ELogLevel::INFO);
        }
    }
}

void TDisplaySDL::deinit(){
    if(m_texture){
        SDL_DestroyTexture(m_texture);
    }
    if(m_rend){
        SDL_DestroyRenderer(m_rend);
    }
    if(m_window){
        SDL_DestroyWindow(m_window);
    }
    SDL_Quit();
}
