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
    m_sound = nullptr;
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
    m_sound->init();
}

void TChip8::run(){
    const int instructions_per_frame = 12; 
    const int target_frame_time_ms = 16; // ~60 FPS

    while (m_emulator_running) {
        auto start_time = std::chrono::high_resolution_clock::now();

        // 1. Handle Input (Once per frame)
        m_keyboard->update(m_KEYS, &m_emulator_running);

        // 2. Run a "Burst" of CPU instructions
        // This is the key fix! We run multiple cycles before sleeping.
        for (int i = 0; i < instructions_per_frame; i++) {
            m_cpu->fetch();
            m_cpu->decode();
            m_cpu->exec();
        }

        // 3. Update Timers (Run at 60Hz standard)
        if (m_delay_timer > 0) {
            m_delay_timer--;
        }
        if (m_sound_timer > 0) {
            m_sound_timer--;
            m_sound->playTune(); // Assuming this handles the beep state
        } else {
            m_sound->pauseT();
        }

        // 4. Draw to Screen (Once per frame)
        m_display->draw(m_screen, SCREEN_H, SCREEN_W);

        // 5. Sleep to lock 60 FPS
        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        
        int sleep_ms = target_frame_time_ms - (int)elapsed_ms.count();
        if (sleep_ms > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
            // OR use SDL_Delay(sleep_ms); if you prefer SDL functions
        }
    }
}

void TChip8::deinit(){
    //to de initialize whatever we intialized :) for shutdown 
    m_cpu->deinit();
    m_display->deinit();
    m_keyboard->deinit();
    m_sound->deinit();
}

void TChip8::setKeyboard(TKeyboard* keyboard){
    m_keyboard = keyboard;
}

void TChip8::setDisplay(TDisplay* display){
    m_display = display;
}

void TChip8::setSound(TSound* sound){
    m_sound = sound;
}
