#ifndef __CPU_H__
#define __CPU_H__

#include <cstdint>
#include "chip8.h"
#include "logger.h"
using namespace std;

#define NUM_V_REGISTERS 16
#define PC_STRT 0x200

class TCpu{

    //declaring these for the registers
    uint8_t m_reg[NUM_V_REGISTERS];
    uint16_t m_ireg;//tells where the program is stored:used to access where the spritesd data is etc
    uint16_t m_pcreg;//Program counter tells which line to read next
    uint16_t m_sp_reg;//for stack: to c how deep we r into the stack

    uint16_t m_current_opcode;//stores the exact opcode cpu just 
    uint16_t m_instruction;//stores the decoded version of the opcode

    TChip8* m_machine;
    shared_ptr<TLogger> m_logger;
    public:
    TCpu(TChip8* machine);
        ~TCpu();
    void init();
    void fetch();
    void decode();
    void exec();
    void deinit();

};  

#endif