#include "renderer.h"
#include <string>
#include "chip8.h"
#include <fstream>
#include <iostream>

CHIP8::CHIP8(int s) : renderer(s)
{
    memory = new uint8_t[4096];
    registers = new uint8_t[16];
    I = 0x200;                  //start I at 0x200, not sure if this is neccesary
    delayT = 0, soundT = 0;     //init timers at 0
    pc = 0x200;                 //start prog counter from 0x200, probably neccesary
    sp = 0;                     //init at 0, signifying empty stack
    stack = new uint16_t[16];
}

void CHIP8::loadROM(std::string path)
{
    std::ifstream romFile(path, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = romFile.tellg();

    romFile.seekg(0, std::ios::beg);
    romFile.read((char*)memory + 0x200, fileSize);
}

void CHIP8::update()
{
    //update timers
    //timerUpdate();
    
    //read OP code
    //perform action
    perfInstruction();
    
    //display
    renderer.displayScreen(16);
}

void CHIP8::perfInstruction()
{
    //extracting opcode from memeory
    uint16_t opcode = memory[pc];
    opcode = opcode<<8;
    opcode += memory[pc+1];

    //getting the most sig nibble of the opcode
    uint8_t firstNibble = getTop(opcode);

    switch (firstNibble)
    {
        case 0x0:  
            if(opcode == 0x00E0) renderer.clearScreen(); //clear screen
            else if(opcode == 0x00EE)                    //return subroutine
            {
                if(sp > 0)  //check if stack is not empty
                {
                    sp--; 
                    pc = stack[sp];
                }
            }
            else notImp();                               //dont really need this, not used mostly
            break;
        
        case 0x1:                                        //goto NNN
            pc = getNNN(opcode);  //set pc to NNN (bottom 12 bits)
            pc -= 2;  //since we increment pc in every loop, this is to make sure pc stays at this place after being incremented
            break;

        case 0x2:                                       //call subroutine at NNN
            if(sp < 16)     //check if stack has space
            {
                stack[sp] = pc;   //put pc at top of stack
                sp++;             
                pc = getNNN(opcode);  //move pc to NNN
                pc -= 2;
            }
            break;

        case 0x3:
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            if(registers[x] == nn) pc += 2;
            break;

        case 0x4:
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            if(registers[x] != nn) pc += 2;
            break;

        case 0x5:
            uint16_t x = getX(opcode);
            uint16_t y = getY(opcode);
            if(registers[x] == registers[y]) pc += 2;
            break;

        case 0x6:
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            registers[x] = nn;
            break;
        
        case 0x7:
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            registers[x] += nn;
            break;
    }
}

void CHIP8::notImp()
{
    std::cout<<"Not Imp\n";
}

uint16_t CHIP8::getNNN(uint16_t opc) {return (opc & ((1<<12) - 1));}
uint16_t CHIP8::getNN(uint16_t opc) {return (opc & ((1<<8) - 1));}
uint16_t CHIP8::getN(uint16_t opc) {return (opc & ((1<<4) - 1));}
uint16_t CHIP8::getX(uint16_t opc) {return (opc & (((1<<4) - 1) << 8));}
uint16_t CHIP8::getY(uint16_t opc) {return (opc & (((1<<4) - 1) << 4));}
uint16_t CHIP8::getTop(uint16_t opc) {return (opc >> 12);}

