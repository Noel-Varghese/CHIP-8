#ifndef __DISPLAYSDL_H__
#define __DISPLAYSDL_H__

#include <cstdint>

#include "display.h"
#include "logger.h"


struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class TDisplaySDL:public TDisplay{
    SDL_Window* m_window;
    SDL_Renderer* m_rend;
    SDL_Texture* m_texture;
    int m_scale;
    std::shared_ptr<TLogger> m_logger;
    public:
        TDisplaySDL();
            ~TDisplaySDL();
        virtual void init() override;
        virtual void draw(
            uint8_t framebuffer[][64], 
            uint16_t height, 
            uint16_t width
        )override;
        virtual void update()override;
        virtual void deinit() override;
};

#endif