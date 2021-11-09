#pragma once
#include<SDL.h>

class Renderer{
    public:
    int scale;
    int unscaledHeight = 32;
    int unscaledWidth = 64;

    SDL_Renderer *renderer;
	SDL_Window *window;
    int rendererFlags, windowFlags;

    float frameRate;

    Renderer(int s);
    void clearScreen();
    void setPixel(int x, int y);
    void displayScreen(uint32_t delay);
    void drawByte(uint8_t x, uint8_t y, uint8_t byte);
    ~Renderer();
};