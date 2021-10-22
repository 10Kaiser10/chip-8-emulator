#pragma once
#include "renderer.h"
#include <string>

class CHIP8{
    private:
    
    Renderer renderer; //renderer object, emulates the screen
    
    char* memory;    //char array for RAM
    char* registers; //char array for 16 registers
    short int I;     //I pointer, 16 bit
    char delayT;     //delay timer, 8 bit
    char soundT;     //sound timer, 8 bit
    short int pc;    //program counter, 16 bit
    char sp;         //stack pointer, 8 bit
    short int* stack;//stack array, 16 bit * 16 places

    public:
    CHIP8(int s);
    void loadROM(std::string path);
    void update();
};