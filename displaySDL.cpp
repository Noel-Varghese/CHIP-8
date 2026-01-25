#include <cstdint>
#include <SDL2/SDL.h>

#include "displaySDL.h"
using namespace std;

TDisplaySDL::TDisplaySDL(){
    m_logger = TLogger::getInstance();
    m_window = nullptr;
    m_rend = nullptr;
    m_texture = nullptr;
}
TDisplaySDL::~TDisplaySDL(){
    
}

void TDisplaySDL::init()
{
    // To initialize it
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::string errorSdl(SDL_GetError());
        m_logger->log("SDL Initialization Error: " + errorSdl, ELogLevel::ERROR);
        exit(1);
    }

    m_window = SDL_CreateWindow("CHIP 8",//names the window and creates it accordingly
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                1280, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if(!m_window)
    {
        std::string errorWindow(SDL_GetError());
        m_logger->log("Window creation Error: " + errorWindow, ELogLevel::ERROR);
        exit(1);
    }
    //activates GPU for better and smoother functioning
    m_rend = SDL_CreateRenderer(m_window, -1,  SDL_RENDERER_ACCELERATED);
    if(!m_rend){
        m_logger->log("Renderer Error: " + string(SDL_GetError()), ELogLevel::ERROR);
        exit(1);
    }
    m_texture = SDL_CreateTexture(
        m_rend, SDL_PIXELFORMAT_RGBA8888
        , SDL_TEXTUREACCESS_STREAMING, 
        64, 
        32);//helps in creating pixels constantly
}

void setPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        pixels[(y * surface->w) + x] = color;
    }
}

void TDisplaySDL::draw(uint8_t framebuffer[][64], uint16_t height, uint16_t width)
{
    uint32_t pixels[2048];

    for(int y=0;y< 32;++y){
        for(int x = 0;x<64;++x){
            uint8_t pixel = framebuffer[y][x];
            int index = (y*64)+x;
            if(pixel == 1){
                pixels[index] = 0x00FF00FF;//green
            }else{
                pixels[index] = 0x000000FF;//black
            }
        }
    }

    // if(SDL_MUSTLOCK(m_surface)){
    //     SDL_LockSurface(m_surface);
    // }
    // // Clear the screen
    // Uint32 color = SDL_MapRGB(m_surface->format, 0x00, 0x00, 0x00); // For Black color

    // // Fills the entire surface with the black color
    // SDL_FillRect(m_surface, NULL, color);

    // //Uint32 white = SDL_MapRGB(m_surface->format, 255, 255, 255);//For white color SDL_MapRGB(m_surface->format, 255, 255, 255); 
    // Uint32 green = SDL_MapRGB(m_surface->format, 0, 255, 0);  //For Green color

    // for(auto i=0; i<height; i++)
    // {
    //     for(auto j=0; j<width; j++)
    //     {
    //         if(framebuffer[i][j] == 1)
    //         {//helps in scalling the pixel
    //             SDL_Rect pixelRect;
    //             pixelRect.x = j*zoomFactor;
    //             pixelRect.y = i*zoomFactor;
    //             pixelRect.w = zoomFactor;
    //             pixelRect.h = zoomFactor;

    //             SDL_FillRect(m_surface, &pixelRect, green);//simpler way of draw pixels compared to for loop
    //         }
    //     }   
    // }
    // if(SDL_MUSTLOCK(m_surface)){
    //     SDL_UnlockSurface(m_surface);
    // }
    // SDL_UpdateWindowSurface(m_window);
    SDL_UpdateTexture(
        m_texture,
        nullptr,
        pixels, 
        64*sizeof(uint32_t)
    );//helps update texture with new pixels

    SDL_RenderClear(m_rend);//clear screen
    SDL_RenderCopy(m_rend, m_texture, nullptr, nullptr);//handles texture to the screen
    SDL_RenderPresent(m_rend);
    
}

void TDisplaySDL::update()
{
    // SDL_Event e;
    // while(SDL_PollEvent(&e) != 0) 
    // {
    //     if (e.type == SDL_QUIT) {
    //         m_logger->log("CLOSING: ", ELogLevel::ERROR);
    //         exit(0);
    //     }
    // }
    //since the keyboard handles event this part is not necessary
}

void TDisplaySDL::deinit()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_rend);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}