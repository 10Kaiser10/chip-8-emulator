#include <iostream>
#include "chip8.h"
#include "renderer.h"

int main(int argc, char* args[])
{
    CHIP8 emulator(20);
    emulator.loadROM("D:/CODE/chip-8-emulator/ROMs/BreakOut.ch8");

    emulator.update();

    return 0;
}