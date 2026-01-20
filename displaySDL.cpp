#include <cstdint>
#include <SDL2/SDL.h>

#include "displaySDL.h"
using namespace std;

TDisplaySDL::TDisplaySDL(){
    m_logger = TLogger::getInstance();
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
    //raw texture setup
    void* pixels = nullptr;//points to raw texture memory
    int pitch = 0;//number of bytes per row
    if(SDL_LockTexture(m_texture, nullptr, &pixels, &pitch)!=0){
        string errortexture(SDL_GetError());
        m_logger->log("Texture error: "+errortexture, ELogLevel::ERROR);
        return;
    }
    uint32_t* textPixels = static_cast<uint32_t*>(pixels);//makes pixel useable
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

    SDL_FRect dst;//used for scaling and positioning
    dst.x = 0.0f;//draws from the left edge
    dst.y = 0.0f;//draws from the top edge
    dst.w = 64.0f*m_scale;//sets the width
    dst.h = 32.0f*m_scale;//sets the height

    SDL_RenderTexture(m_rend, m_texture, nullptr, &dst);//takes in texture renders it and scales it using dst
    SDL_RenderPresent(m_rend);//makes the frame visible on screen

    SDL_Event e;
    while(SDL_PollEvent(&e)){//helps pull all pending events
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
