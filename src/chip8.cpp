#include "renderer.h"
#include <string>
#include "chip8.h"
#include <fstream>
#include <iostream>

CHIP8::CHIP8(int s) : renderer(s)
{
    memory = new char[4096];
    registers = new char[16];
    I = 0x200;                  //start I at 0x200, not sure if this is neccesary
    delayT = 0, soundT = 0;     //init timers at 0
    pc = 0x200;                 //start prog counter from 0x200, probably neccesary
    sp = 0;                     //init at 0, signifying empty stack
    stack = new short int[16];
}

void CHIP8::loadROM(std::string path)
{
    std::ifstream romFile(path, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = romFile.tellg();

    romFile.seekg(0, std::ios::beg);
    romFile.read(memory + 0x200, fileSize);
}

void CHIP8::update()
{
    //update timers
    //read OP code
    //perform action
    //display
}