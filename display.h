#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <cstdint>

class TDisplay{
    public:
    virtual void init() = 0;//used to create windows, textures etc
    virtual void draw(
        uint8_t framebuffer[][64], 
        uint16_t height,
        uint16_t width
    ) = 0;//helps in drawing things on the host screen
    virtual void update() = 0;//modifies frame buffer and updates the screen
    virtual void deinit() = 0;//destroys windows, avoids: memory leaks, window zombies, GPU resource leaks
};


#endif