#pragma once
#include "renderer.h"
#include <string>

class CHIP8{
    private:
    
    Renderer renderer; //renderer object, emulates the screen
    
    uint8_t* memory;    //8-bit array for RAM
    uint8_t* registers; //8-bit array for 16 registers
    uint16_t I;     //I pointer, 16 bit
    uint8_t delayT;     //delay timer, 8 bit
    uint8_t soundT;     //sound timer, 8 bit
    uint16_t pc;    //program counter, 16 bit
    uint8_t sp;         //stack pointer, 8 bit
    uint16_t* stack;//stack array, 16 bit * 16 places

    public:
    CHIP8(int s);
    void loadROM(std::string path);
    void update();
    void perfInstruction();
    void notImp();
    uint16_t getNNN(uint16_t opc); 
    uint16_t getNN(uint16_t opc); 
    uint16_t getN(uint16_t opc); 
    uint16_t getX(uint16_t opc); 
    uint16_t getY(uint16_t opc); 
    uint16_t getTop(uint16_t opc); 


};