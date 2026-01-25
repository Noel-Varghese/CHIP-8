#include "chip8.h"
#include "logger.h"
#include "cpu.h"
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>


TChip8::TChip8(){
    m_logger = TLogger::getInstance();
    m_cpu = new TCpu(this);
    m_emulator_running = true;
    m_display = nullptr;
    m_keyboard = nullptr;
}
TChip8::~TChip8(){

}

void TChip8::init(std::string rom_path){
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
    m_cpu->init();
    delete m_loader;
    m_keyboard->init();
}

void TChip8::run(){
    using clock = std::chrono::high_resolution_clock;
    clock::time_point start, end;
    const std::chrono::milliseconds desired_cycle_time(2);
    int display_updateDelayTime = 0;
    static int time_accumulator = 0;
    while(m_emulator_running){
        start = clock::now();
        m_cpu->fetch();
        m_cpu->decode();
        m_cpu->exec();
        if(time_accumulator >= 16){//updates the timer in ~60Hz
            time_accumulator = 0;
            m_display->draw(m_screen, SCREEN_H, SCREEN_W);
            if(m_delay_timer > 0){
                m_delay_timer--;
            }
            if(m_sound_timer > 0){
                m_sound_timer--;
            }
        }
        //m_display->update();
        m_keyboard->update(m_KEYS, &m_emulator_running);
        end = clock::now();
        std::chrono::duration<double, std::micro> loop_time = end-start;
        //inorder to calculate the elapsed time in millisecond
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        //calculates minimum time to reach the desired cycle time
        //time_accumulator += elapsed_time.count();
        auto sleep_time = desired_cycle_time - elapsed_time;
        if(sleep_time.count() > 0){
            std::this_thread::sleep_for(sleep_time);
        }
        time_accumulator += desired_cycle_time.count();
        //display_updateDelayTime++;
        //SDL_Delay(2);
    }
}

void TChip8::deinit(){
    //to de initialize whatever we intialized :) for shutdown 
    m_cpu->deinit();
    m_display->deinit();
    m_keyboard->deinit();
}

void TChip8::setKeyboard(TKeyboard* keyboard){
    m_keyboard = keyboard;
}

void TChip8::setDisplay(TDisplay* display){
    m_display = display;
}
