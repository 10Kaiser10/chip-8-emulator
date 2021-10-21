#include <iostream>
#include <SDL.h>
#include "renderer.h"

int main(int argc, char* args[])
{
    const int scale = 20;
    Renderer renderer = Renderer(scale);

    renderer.setPixel(10,10);
    renderer.setPixel(50,20);
    renderer.setPixel(30,31);
    renderer.displayScreen();
}