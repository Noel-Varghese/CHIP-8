#ifndef __CHIP8_h__
#define __CHIP8_h__

#include <cstdint>//includes all the uint8_t, uint16_t
#include <iostream>
#include "logger.h"
#include "romLoader.h"
#include "display.h"
//defines basic utilities like font etc.
#define NUM_KEYS 16
#define TOTAL_RAM 4096
#define STACK_SIZE 16
#define FONTSET_SIZE 80//16*5=80
#define TIMER_MAX 255//2^8-1=255
//allocates space from where to where
#define CHIP8_RAM_STRT_ADDR 0x000
#define CHIP8_RAM_END_ADDR 0x1FF
#define CHIP8_STRT_ADDR 0x200
#define CHIP8_END_ADDR 0xFFF
//size of the screen
#define SCREEN_H 32
#define SCREEN_W 64

using namespace std;
//used to draw characters in the chip8 system converts hex->binary digits
const static uint8_t FONTSET[] = { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };



class TCpu;//does all the opcode
class TChip8{
    friend class TCpu;
    TCpu* m_cpu;

    uint8_t m_screen[SCREEN_H][SCREEN_W];
    uint8_t m_KEYS[NUM_KEYS];
    bool m_key_pressed;
    uint8_t RAM[TOTAL_RAM];
    uint16_t m_STACK[STACK_SIZE];
    //sound
    uint8_t m_delay_timer;
    uint8_t m_sound_timer;

    bool m_emulator_running;

    shared_ptr<TLogger> m_logger;
    //rom loader
    TRomLoader* m_loader;
    TDisplay* m_display;
    public:
        TChip8();
        /*We use a constructor to put the CHIP-8 machine into a valid starting state,
and a destructor to cleanly shut it down and free resources.The destructor runs automatically when the emulator object dies —
which usually happens when the program exits (i.e., you close the emulator). */
            ~TChip8();
            void init(string rom_path);
            void run();
            void deinit();
            void setDisplay(TDisplay* display);

};  


#endif