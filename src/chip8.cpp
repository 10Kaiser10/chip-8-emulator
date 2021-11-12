#include "renderer.h"
#include <random>
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
    //std::cout<<"Update"<<std::endl;
    //update timers
    timerUpdate();
    
    //read OP code
    //perform action
    perfInstruction();
    
    //display
    renderer.displayScreen(16);
}

void CHIP8::timerUpdate()
{
    delayT -= 1;
    soundT -= 1;
}

void CHIP8::perfInstruction()
{
    //extracting opcode from memeory
    //printStatus();
    uint16_t opcode = memory[pc];
    opcode = opcode<<8;
    opcode += memory[pc+1];

    //getting the most sig nibble of the opcode
    uint8_t firstNibble = getTop(opcode);
    //std::cout<<std::hex<<opcode<<std::endl;
    switch (firstNibble)
    {
        case 0x0:  
        {
            if(opcode == 0x00E0) renderer.clearScreen(); //clear screen
            else if(opcode == 0x00EE)                    //return subroutine
            {
                if(sp > 0)  //check if stack is not empty
                {
                    sp--; 
                    pc = stack[sp];
                }
            }
            else notImp(opcode);                               //dont really need this, not used mostly
            break;
        }
        case 0x1:                                        //goto NNN
        {
            pc = getNNN(opcode);  //set pc to NNN (bottom 12 bits)
            pc -= 2;  //since we increment pc in every loop, this is to make sure pc stays at this place after being incremented
            break;
        }
        case 0x2:                                       //call subroutine at NNN
        {
            if(sp < 16)     //check if stack has space
            {
                stack[sp] = pc;   //put pc at top of stack
                sp++;             
                pc = getNNN(opcode);  //move pc to NNN
                pc -= 2;
            }
            break;
        }
        case 0x3:
        {
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            if(registers[x] == nn) pc += 2;
            break;
        }
        case 0x4:
        {
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            if(registers[x] != nn) pc += 2;
            break;
        }
        case 0x5:
        {
            uint16_t x = getX(opcode);
            uint16_t y = getY(opcode);
            if(registers[x] == registers[y]) pc += 2;
            break;
        }
        case 0x6:
        {
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            registers[x] = nn;
            break;
        }
        case 0x7:
        {
            uint16_t x = getX(opcode);
            uint16_t nn = getNN(opcode);
            registers[x] += nn;
            break;
        }
        case 0x8:
        {
            uint16_t lastNibble = getN(opcode);
            uint16_t x = getX(opcode);
            uint16_t y = getY(opcode);

            switch(lastNibble)
            {
                case 0x0:
                    registers[x] = registers[y];                    
                    break;

                case 0x1:
                    registers[x] |= registers[y];
                    break;

                case 0x2:
                    registers[x] &= registers[y];
                    break;
                
                case 0x3:
                    registers[x] ^= registers[y];
                    break;
                
                case 0x4:    //might require some attention, next one too
                {
                    uint16_t sum = (uint16_t)registers[x] + (uint16_t)registers[y];
                    if(sum > 255)
                    {
                        registers[0xF] = 1;
                        registers[x] = sum;
                    } 
                    else
                    {
                        registers[0xF] = 0;
                        registers[x] = sum;
                    }
                    break;
                }
                case 0x5:
                {
                    if(registers[x] >= registers[y])
                    {
                        registers[0xF] = 1;
                        registers[x] -= registers[y];
                    }
                    else
                    {
                        registers[0xF] = 0;
                        registers[x] -= registers[y];
                    }
                    break;
                }
                case 0x6:
                    registers[0xF] = (registers[x] & 1);
                    registers[x] = registers[x] >> 1;
                    break;

                case 0x7:
                    if(registers[y] >= registers[x])
                    {
                        registers[0xF] = 1;
                    }
                    else
                    {
                        registers[0xF] = 0;
                    }
                    registers[x] = registers[y] = registers[x];
                    break;

                case 0xE:
                    registers[0xF] = registers[x]>>7;
                    registers[x] = registers[x]<<1;
                    break;
            }
            break;
        }
        case 0x9:
        {
            uint16_t x = getX(opcode);
            uint16_t y = getY(opcode);
            if(registers[x] != registers[y]) pc += 2;
            break;
        }
        case 0xA:
        {
            I = getNNN(opcode);
            break;
        }
        case 0xB:
        {
            pc = registers[0x0] + getNNN(opcode);
            break;
        }
        case 0xC:
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist(0,255);
            
            registers[getX(opcode)] = dist(rng) & getNN(opcode);
            break;
        }
        case 0xD:
        {
            //std::cout<<"DrawCall\n";
            uint16_t x = getX(opcode);
            uint16_t y = getY(opcode);
            uint16_t n = getN(opcode);

            uint8_t vf = 0;

            for(uint8_t i=0; i<n; i++)
            {
                vf != renderer.drawByte(registers[x], registers[y]+i, memory[I+i]);
            }
            //std::cout<<"Drawn\n";
            registers[0xF] = vf;
            break;
        }
        case 0xE:
        {
            notImp(opcode);
            break;
        }
        case 0xF:
        {
            uint16_t x = getX(opcode);
            switch(getNN(opcode))
            {
                case 0x07:
                {
                    registers[x] = delayT;
                    break;
                }
                case 0x0A:
                {
                    notImp(opcode);
                    break;
                }
                case 0x15:
                {
                    delayT = registers[x];
                    break;
                }
                case 0x18:
                {
                    soundT = registers[x];
                    break;
                }
                case 0x1E:
                {
                    I += registers[x];
                    break;
                }
                case 0x29:
                {
                    notImp(opcode);
                    break;
                }
                case 0x33:
                {
                    uint16_t vx = registers[x];
                    memory[I+2] = vx%10;
                    vx /= 10;
                    memory[I+1] = vx%10;
                    vx /= 10;
                    memory[I] = vx%10;
                    break;
                }
                case 0x55:
                {
                    for(int i=0; i<15; i++)
                    {
                        memory[I+i] = registers[0+i];
                    }
                    break;
                }
                case 0x65:
                {
                    for(int i=0; i<15; i++)
                    {
                        registers[0+i] = memory[I+i];
                    }
                    break;
                }
            }
        }
    }
    pc += 2;
}

void CHIP8::notImp(uint16_t opc)
{
    //printStatus();
    std::cout<<"Not Imp "<<std::hex<<opc<<std::endl;
}

void CHIP8::printStatus()
{
    for(int i=0; i<16; i++)
    {
        std::cout<<std::hex<<(int)registers[i]<<' ';
    }
    std::cout<<std::endl;
}

//helper fucntions to get part of the opcode
uint16_t CHIP8::getNNN(uint16_t opc) {return (opc & ((1<<12) - 1));}
uint16_t CHIP8::getNN(uint16_t opc) {return (opc & ((1<<8) - 1));}
uint16_t CHIP8::getN(uint16_t opc) {return (opc & ((1<<4) - 1));}
uint16_t CHIP8::getX(uint16_t opc) {return (opc & (((1<<4) - 1) << 8))>>8;}
uint16_t CHIP8::getY(uint16_t opc) {return (opc & (((1<<4) - 1) << 4))>>4;}
uint16_t CHIP8::getTop(uint16_t opc) {return (opc >> 12);}

