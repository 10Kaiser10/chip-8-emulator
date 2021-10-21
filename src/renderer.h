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
    void displayScreen();
    ~Renderer();
};