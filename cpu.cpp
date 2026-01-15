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
        case 0x1: 
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: m_logger->log("INSTRUCTION OPCODE NOT IMPLEMENTED!!!", ELogLevel::ERROR); exit(1); break;
    }
}

void TCpu::deinit(){

}
