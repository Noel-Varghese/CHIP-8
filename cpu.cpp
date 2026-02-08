#include <cstdint>
#include <ctime>

#include "cpu.h"
#include "logger.h"

TCpu::TCpu(TChip8* machine){
    m_machine = machine;
    m_logger = TLogger::getInstance();
}

TCpu::~TCpu(){
    
}

void TCpu::init(){
    m_pcreg = PC_STRT;//points to the strting point
    m_current_opcode = 0;
    m_ireg = 0;
    m_sp_reg = 0;
    m_instruction = 0;
    for(auto i=0;i<NUM_V_REGISTERS;i++){
        m_reg[i] = 0;
    }
    srand(time(0));
}

void TCpu::fetch(){
    m_current_opcode = ((uint16_t)m_machine->RAM[m_pcreg]<<8) | m_machine->RAM[m_pcreg+1];
    m_pcreg+=2;
}

void TCpu::decode(){
    m_instruction = m_current_opcode >> 12;//we need the highest 4 bits , so 16-4 = 12
}

void TCpu::exec(){
    m_logger->log("INST"+to_string(m_instruction), ELogLevel::DEBUG);
    switch(m_instruction){
        case 0x0:
            decode_0_instruction();
            break;
        case 0x1:
            jump_to(); 
            break;
        case 0x2:
            call_subroutine();
            break;
        case 0x3:
            skip_next_instruction_eq();
            break;
        case 0x4:
            skip_next_instruction_ne();
            break;
        case 0x5:
            if ((m_current_opcode & 0x000F) == 0){
                skip_next_instruction_vx_vy();
            }else{
                m_logger->log("Invalid 5XY opcode", ELogLevel::ERROR);
            }
            break;
        case 0x6:
            register_set();
            break;
        case 0x7:
            add_reg_imm();
            break;
        case 0x8:
            decode_8_instruction();
            break;
        case 0x9:
            if ((m_current_opcode & 0x000F) == 0x0) {
                skip_next_instruction_vx_vy_ne();
            } else {
                m_logger->log("Invalid 9XY opcode", ELogLevel::ERROR);
            }
            break;
        case 0xA:
            set_index_register();
            break;
        case 0xB:
            jump_with_v0();
            break;
        case 0xC:
            generate_random_number();
            break;
        case 0xD:
            draw_sprite();
            break;
        case 0xE:
            decode_E_instruction();
            break;
        case 0xF:
            decode_F_instruction();
            break; 
        default: m_logger->log("INSTRUCTION OPCODE NOT IMPLEMENTED!!!", ELogLevel::ERROR); exit(1); break;
    }
}

void TCpu::deinit(){

}


//private functions for the cpu
/*
0ZZZ
*/

void TCpu::decode_0_instruction(){
    if((m_current_opcode & 0xF000) != 0x0000){
        return;
    }
    switch(m_current_opcode&0xFF){
        case 0xE0:
            clear_screen();
            break;
        case 0xEE:
            return_from_subroutine();
            break;
        default:
            //m_logger->log("Instruction 0 with code"+to_string(m_current_opcode&0xFF), ELogLevel::ERROR);
            break;
    }
}
//0ZE0

void TCpu::clear_screen(){
    for(int i=0;i<SCREEN_H;i++){
        for(int j=0;j<SCREEN_W;j++){
            m_machine->m_screen[i][j] = 0;
        }
    }
}//clears the screen and sets it ready

//0ZEE

void TCpu::return_from_subroutine(){
    if (m_sp_reg == 0) {
        m_logger->log("STACK UNDERFLOW", ELogLevel::ERROR);
        return;
    }
    m_pcreg = m_machine->m_STACK[--m_sp_reg];
}//Loads the return address

//1NNN

void TCpu::jump_to(){
    m_pcreg = m_current_opcode&0x0FFF;
}//sets the program counter to a new address and continues execution from there with no return

//2NNN

void TCpu::call_subroutine(){
    uint16_t NNN = m_current_opcode&0x0FFF;
    m_machine->m_STACK[m_sp_reg] =m_pcreg;
    m_sp_reg++;
    m_pcreg = NNN; 
    if (m_sp_reg >= STACK_SIZE) {
        m_logger->log("STACK OVERFLOW", ELogLevel::ERROR);
        return;
    }
}//saves the current program counter to the stack, jumps to a subroutine, and then later through RET

//3XNN
void TCpu::skip_next_instruction_eq(){
    uint8_t value = m_current_opcode&0xFF;
    uint8_t reg = (m_current_opcode>>8)&0x0F;
    if(m_reg[reg] == value){
        m_pcreg += 2;
    }
}

//4XNN
void TCpu::skip_next_instruction_ne(){
    uint8_t val = m_current_opcode&0xFF;
    uint8_t reg = (m_current_opcode>>8)&0x0F;
    if(m_reg[reg] != val){
        m_pcreg += 2;
    }
}

//the 3XNN and 4XNN acts like a if condition unlike a branch the just jump to the next instruction

//5XYZ
void TCpu::skip_next_instruction_vx_vy(){
    uint8_t reg_x = (m_current_opcode>>8)&0x0F;
    uint8_t reg_y = (m_current_opcode>>4)&0x0F;
    if(m_reg[reg_x]==m_reg[reg_y]){
        m_pcreg +=2;
    }
}//controls the flow of a program in the ROM

//6XNN
void TCpu::register_set(){
    uint8_t val = m_current_opcode & 0xFF;
    uint8_t reg = (m_current_opcode >> 8)&0x0F;

    m_reg[reg] = val;
}//assigns value to the register

void TCpu::add_reg_imm(){
    uint8_t val = m_current_opcode & 0xFF;
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    m_reg[reg] += val;
}//adds value to register

//8XYZ
void TCpu::decode_8_instruction(){
    switch (m_current_opcode&0xF)
    {
    case 0x0:
        move_vy_to_vx();
        break;
    case 0x1:
        or_vx_vy();
        break;
    case 0x2:
        and_vx_vy();
        break;
    case 0x3:
        xor_vx_vy();
        break;
    case 0x4:
        add_vx_vy();
        break;
    case 0x5:
        sub_vx_vy();
        break;
    case 0x6:
        shift_right_reg();
        break;
    case 0x7:
        subn_vx_vy();
        break;
    case 0xE:
        shift_left_reg();
        break;
    default:
        m_logger->log("Instruction 8 with code: "+to_string(m_current_opcode&0xF), ELogLevel::ERROR);
    }
}//ALU-arithmatic logic unit

//8XY0

void TCpu::move_vy_to_vx(){
    uint8_t reg_x = (m_current_opcode >> 8)&0x0F;
    uint8_t reg_y = (m_current_opcode >> 4)&0x0F;

    m_reg[reg_x] = m_reg[reg_y];
}//moves data between code so tht the registers dont have to recalculate everything

//8XY1

void TCpu::or_vx_vy(){
    uint8_t reg_x = (m_current_opcode >> 8)&0x0F;
    uint8_t reg_y = (m_current_opcode >> 4)&0x0F;

    m_reg[reg_x] |= m_reg[reg_y];
    m_reg[0xF] = 0;
}//performs bitwiseOR in order to combine two registers to set flags, mask values etc

//8XY2

void TCpu::and_vx_vy(){
    uint8_t reg_x = (m_current_opcode >> 8)&0x0F;
    uint8_t reg_y = (m_current_opcode >> 4)&0x0F;

    m_reg[reg_x] &= m_reg[reg_y];
    m_reg[0xF] = 0;
}//performs AND operations , helps in masking bits and shows only the required ones ...Like Abstraction

//8XY3
void TCpu::xor_vx_vy(){
    uint8_t reg_x = (m_current_opcode >> 8)&0x0F;
    uint8_t reg_y = (m_current_opcode >> 4)&0x0F;

    m_reg[reg_x] ^= m_reg[reg_y];
    m_reg[0xF] = 0;
}//helps toggle and compare values , without branching and extra logic keeps it simple 

//8XY4
void TCpu::add_vx_vy(){
    uint8_t reg_x = (m_current_opcode >> 8)&0x0F;
    uint8_t reg_y = (m_current_opcode >> 4)&0x0F;
    uint16_t add = m_reg[reg_x]+m_reg[reg_y];
    uint8_t cf = (add > 0xFF)? 1:0;
    m_reg[reg_x] = add & 0xFF;
    m_reg[0xF] = cf;
 
}//allows the program to add two register values while also detecting overflows which is required for the math and game logic

//8XY5

void TCpu::sub_vx_vy(){
    uint8_t reg_x = (m_current_opcode >> 8)&0x0F;
    uint8_t reg_y = (m_current_opcode >> 4)&0x0F;
    uint8_t nbf = (m_reg[reg_x] >= m_reg[reg_y])?1:0;

    m_reg[reg_x] -= m_reg[reg_y];
    m_reg[0xF] = nbf;
}//subtracts two registers whilw giving borrow logic, for arithmatic, game logic etc


//8XY6

void TCpu::shift_right_reg(){
    uint8_t vx = (m_current_opcode >> 8) & 0x0F;
    uint8_t vy = (m_current_opcode >> 4) & 0x0F;
    uint8_t flag = m_reg[vx] & 0x1;
    m_reg[vx] = m_reg[vy];


    m_reg[vx] >>= 1;
    m_reg[0xF] = flag;
}
//lets the program to divide the value by 2 and to capture the bits tht gets shifted out
//usefull for arithmatic, movements and bit-level logic

//8XYz

void TCpu::subn_vx_vy(){
    uint8_t reg_x = (m_current_opcode >> 8) & 0x0F;
    uint8_t reg_y = (m_current_opcode >> 4) & 0x0F;
    uint8_t no_borrow_flag = (m_reg[reg_y] >= m_reg[reg_x]) ? 1 : 0;

    

    m_reg[reg_x] = m_reg[reg_y] - m_reg[reg_x];
    m_reg[0xF] = no_borrow_flag;
}//exist so tht the program can write or subtract in reverse order 

//8XYE
void TCpu::shift_left_reg(){
    uint8_t vx = (m_current_opcode >> 8) & 0x0F;
    uint8_t vy = (m_current_opcode >> 4) & 0x0F;
    uint8_t flag = (m_reg[vx] & 0x80) >> 7;
    m_reg[vx] = m_reg[vy];
    m_reg[vx] <<= 1;
    m_reg[0xF] = flag;

    // m_reg[0xF] = (m_reg[reg] & 0x80) >> 7;
    // m_reg[reg] <<= 1;
}//shifts the value 1bit to the left and gets the most significant bit into register


//9XYZ
void TCpu::skip_next_instruction_vx_vy_ne(){
    uint8_t reg_x = (m_current_opcode >> 8) & 0x0F;
    uint8_t reg_y = (m_current_opcode >> 4) & 0x0F;

    if(m_reg[reg_x]!= m_reg[reg_y]){
        m_pcreg +=2;
    }
}//helps programe make disitions without using branches also helps change flow of the programe

//ANNN
void TCpu::set_index_register(){
    m_ireg = m_current_opcode & 0x0FFF;//helps the programe point to a specific location in memory
}

//BNNN
void TCpu::jump_with_v0(){
    uint16_t NNN = m_current_opcode & 0x0FFF;
    m_pcreg = NNN + m_reg[0];
}
//allows offset based jumps for dynamically changing flow of the code

//CXKK

void TCpu::generate_random_number(){
    uint8_t reg = (m_current_opcode >> 8)&0x0F;
    uint8_t kk = m_current_opcode & 0xFF;
    uint8_t randNUM = rand()%256;

    m_reg[reg] = randNUM & kk;
}//helps give program controlled randomness for movement and behaviour

//DXYN
void TCpu::draw_sprite(){
    uint8_t v_reg_x = (m_current_opcode & 0x0F00)>>8;
    uint8_t v_reg_y = (m_current_opcode & 0x00F0)>>4;
    uint8_t sprite_H = m_current_opcode & 0x000F;
    uint8_t x_location = m_reg[v_reg_x] % 64;//drawing strts from here for x
    uint8_t y_location = m_reg[v_reg_y] % 32;//drawing strts from here for y
    

    m_reg[0xF] = 0;//collision flag
    for(int y_co = 0;y_co<sprite_H;y_co++){//each sprite row is 1byte=8pixels wide
        uint8_t pixel = m_machine->RAM[(m_ireg+y_co) & 0xFFF];
        for(int x_co = 0;x_co<8;x_co++){//each bit correspondes to 1 screen pixel
            if((pixel & (0x80 >> x_co))!=0){//shifts right inorder to test each bit
                uint8_t x = (x_location + x_co);
                uint8_t y = (y_location + y_co);//helps the sprites to wrap around the screen
                if(x>=64 || y>=32){
                    continue;
                }
                if (m_machine -> m_screen[y][x] == 1)
                {
                    m_reg[0xF] = 1;
                }
                m_machine -> m_screen[y][x] ^= 0x1;//helps in drawing efficiently, and gives smooth animation

            }
        }
    }
}

//EZZZ
void TCpu::decode_E_instruction(){
    switch (m_current_opcode & 0x00FF)
    {
    case 0x9E:
        skip_next_instruction_if_key_pressed();
        break;
    case 0xA1:
        skip_next_instruction_if_notKeyPressed();
        break;
    
    default:
        m_logger->log("Instruction E with code"+to_string(m_current_opcode&0xFF), ELogLevel::ERROR);
    }
}//helps react to user input

//EX9E
void TCpu::skip_next_instruction_if_key_pressed(){
    uint8_t reg = (m_current_opcode >> 8)&0x0F;
    uint8_t val = m_reg[reg] & 0x0F;

    if(m_machine->m_KEYS[val] != 0){
        m_pcreg +=2;
    }
}//skips to the next instruction if a key is pressed

//EXA1
void TCpu::skip_next_instruction_if_notKeyPressed(){
    uint8_t reg = (m_current_opcode >> 8)&0x0F;
    uint8_t val = m_reg[reg] & 0x0F;

    if(m_machine->m_KEYS[val] == 0){
        m_pcreg +=2;
    }
}//skips to the next instruction if a key is not pressed

//FZZZ

void TCpu::decode_F_instruction(){
    switch (m_current_opcode & 0x00FF)
    {
    case 0x07:
        loadRegWithDelayTimer();
        break;
    case 0x0A:
        waitKeyPress();
        break;
    case 0x15:
        load_delay_timeWithReg();
        break;
    case 0x18:
        load_soundTimerWithReg();break;
    case 0x1E:
        add_iregWith_reg();
        break;
    case 0x29:
        load_FontFrom_vx();
        break;
    case 0x33:
        store_binary_code_decimal_representation();
        break;
    case 0x55:
        load_memory_from_regs();
        break;
    case 0x65:
        load_regs_from_memory();
        break;
    default:
        m_logger->log("Instruction F with code: "+to_string(m_current_opcode & 0xFF), ELogLevel::ERROR);
        break;
    }
}//helps in decoding all of CHIP-8 instruction based on last byte of opcode

//FX07

void TCpu::loadRegWithDelayTimer(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    m_reg[reg] = m_machine->m_delay_timer;
}
//loads delay timer into vx


//FX0A
void TCpu::waitKeyPress(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    bool keypressed = false;

    for(int i=0;i<NUM_KEYS;i++){
        if(m_machine->m_KEYS[i] !=0){
            m_reg[reg] = i;
            keypressed = true;
        }
    }
    if(!keypressed){
        m_pcreg -= 2;
    }
}//basically waits for a key press and stores the key index into vx ...also pauses certain instruction until key pressed

//FX15

void TCpu::load_delay_timeWithReg(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    m_machine->m_delay_timer = m_reg[reg];
}//loads the Vx value into the delay timer 10/10

//FX18

void TCpu::load_soundTimerWithReg(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    m_machine->m_sound_timer = m_reg[reg];
}//Loads Vx into sound timer <>

//FX1E
void TCpu::add_iregWith_reg(){
    uint8_t reg = (m_current_opcode >> 8)&0x0F;
    m_ireg += m_reg[reg];
}//helps move the pointer forward , its basically a walk through memory

//FX29
void TCpu::load_FontFrom_vx(){
    uint8_t reg = (m_current_opcode >> 8)&0x0F;
    m_ireg = (m_reg[reg] & 0x0F) * 0x5;
}//sprites are loaded contiguously and are 5bytes tall hece we multiply with 0x5 inorder to fetch the next one

//FX33
void TCpu::store_binary_code_decimal_representation(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;

    m_machine->RAM[m_ireg & 0xFFF] = m_reg[reg]/100;
    m_machine->RAM[(m_ireg+1)&0xFFF] = (m_reg[reg]/10)%10;
    m_machine->RAM[(m_ireg+2)&0xFFF] = (m_reg[reg])%10;
}
//uses binary values to help display decimal on screen

//FX55
void TCpu::load_memory_from_regs(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    for(int i =0;i<=reg;i++){
        m_machine->RAM[(m_ireg+i)&0xFFF] = m_reg[i];
    }
    m_ireg += reg + 1;
}//helps in bulk saving values, its a register dump(copying current contents of CPU registers into memory)

//FX65
void TCpu::load_regs_from_memory(){
    uint8_t reg = (m_current_opcode >> 8) & 0x0F;
    for(int i=0; i<=reg; i++){
        m_reg[i] = m_machine->RAM[(m_ireg + i)&0xFFF];
    }
    m_ireg += reg + 1;
}//restores saved states, reload data from memory,....., it is the inverse of register dump

