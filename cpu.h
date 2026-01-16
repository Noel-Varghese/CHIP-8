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
    void decode_0_instruction();//0ZZZ
    void clear_screen();//0ZE0
    void return_from_subroutine();//0ZEE
    void jump_to();//1NNN
    void call_subroutine();//2NNN
    void skip_next_instruction_eq();//3XNN
    void skip_next_instruction_ne();//4XNN
    void skip_next_instruction_vx_vy();//5XYZ
    void register_set();//6XNN
    void add_reg_imm();//7XNN
    void decode_8_instruction();//8XYZ
    void move_vy_to_vx();//8XY0
    void or_vx_vy();//8XY1
    void and_vx_vy();//8XY2
    void xor_vx_vy();//8XY3
    void add_vx_vy();//8XY4
    void sub_vx_vy();//8XY5
    void shift_right_reg();//8XY6
    void subn_vx_vy();//8XY7
    void shift_left_reg();//8XYE
    void skip_next_reg_vx_vy_ne();//9XYZ
    void set_index_register();//ANNN
    void jump_with_v0();//BNNN
    void generate_random_number();//CXKK
    void draw_sprite();//DXYN
    void decode_E_instruction();//EZZZ
    void skip_next_instruction_if_key_pressed();//EX9E
    void skip_next_instruction_if_notKeyPressed();//EXA1
    void decode_F_instruction();//FZZZ
    void loadRegWithDelayTimer();//FX07
    void waitKeyPress();//FX0A
    void load_delay_timeWithReg();//FX15
    void load_soundTimerWithReg();//FX18
    void add_iregWith_reg();//FX1E
    void load_FontFrom_vx();//FX29
    void store_binary_code_decimal_representation();//FX33
    void load_memory_from_regs();//FX55
    void load_regs_from_memory();//FX65



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