#include "chip8.h"
#include "logger.h"
#include "cpu.h"

using namespace std;

TChip8::TChip8(){
    m_logger = TLogger::getInstance();
    m_emulator_running = true;
}
TChip8::~TChip8(){

}

void TChip8::init(string rom_path){
    for(auto i=0;i<SCREEN_H;i++){//for the frome buffer
        for(auto j=0;j<SCREEN_W;j++){
            m_screen[i][j] = 0;
        }
    }
    for(auto i=0;i<STACK_SIZE;i++){//clears the stack
        m_STACK[i] = 0;
    }
    for(auto i=0;i<TOTAL_RAM;i++){
        RAM[i] = 0;
    }
    for(auto i=0;i<FONTSET_SIZE;i++){
        RAM[i] = FONTSET[i];
    }//loads the font into memory
    m_delay_timer =0;
    m_sound_timer = 0;
    //making keyboard stats unpressed;
    for(auto i=0;i<NUM_KEYS;i++){
        m_KEYS[i] = 0;
    }
    m_key_pressed = false;
    m_loader = new TRomLoader();
    m_loader->LoadRom(rom_path, RAM+CHIP8_STRT_ADDR);//loads the ROM
    delete m_loader;
}

void TChip8::run(){
    while(m_emulator_running){
        m_cpu->fetch();
        m_cpu->decode();
        m_cpu->exec();
    }
}

void TChip8::deinit(){
    //to de initialize whatever we intialized :) for shutdown 
    m_cpu->deinit();
}
