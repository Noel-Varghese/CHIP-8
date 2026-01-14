#ifndef __ROMLOADER_H__
#define __ROMLOADER_H__

#include <cstdint>
#include <iostream>
#include "logger.h"

using namespace std;

class TRomLoader{
    shared_ptr<TLogger> m_logger;
    public:
        TRomLoader();
            ~TRomLoader();
        void LoadRom(string& filepath, uint8_t* mem);
};

#endif