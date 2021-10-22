#include "renderer.h"
#include <string>
#include "chip8.h"

CHIP8::CHIP8(int s) : renderer(s)
{
    memory = new char[4096];
    registers = new char[16];
    I = 0x200;                  //start I at 0x200, not sure if this is neccesary
    delayT = 0, soundT = 0;     //init timers at 0
    pc = 0x200;                 //start prog counter from 0x200, probably neccesary
    sp = 0;                    //init at 0, signifying empty stack
    stack = new short int[16];
}

void CHIP8::loadROM(std::string path)
{
    //load ROM from file to memory
}

void CHIP8::update()
{
    //update timers
    //read OP code
    //perform action
    //display
}