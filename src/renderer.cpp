#include <iostream>
#include <SDL.h>
#include "renderer.h"

Renderer::Renderer(int s)
{
    scale = s;

    rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;

	screenFill = new bool*[unscaledWidth];
	for(int i=0; i<unscaledWidth; i++)
	{
		screenFill[i] = new bool[unscaledHeight];
		for(int j=0; j<unscaledHeight; j++) screenFill[i][j] = 0;
	}

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scale*unscaledWidth, scale*unscaledHeight, windowFlags);

	if (!window)
	{
		printf("Failed to open %d x %d window: %s\n", scale*unscaledWidth, scale*unscaledHeight, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);

	if (!renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
}

void Renderer::clearScreen()
{
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
}

void Renderer::setPixel(int x, int y, int c)
{
    SDL_Rect r;
    r.x = scale*x, r.y = scale*y;
    r.h = scale, r.w = scale;

    SDL_SetRenderDrawColor(renderer, c*255,c*255,c*255,255);
    SDL_RenderFillRect( renderer, &r );
}

void Renderer::displayScreen(uint32_t delay)
{
    SDL_RenderPresent(renderer); 
    SDL_Delay(delay);
}

int Renderer::drawByte(uint8_t x, uint8_t y, uint8_t byte)
{
	//std::cout<<std::hex<<(int)x<<' '<<(int)y<<' '<<(int)byte<<std::endl;
	int retVal = 0;
	for(uint8_t i=0; i<8; i++)
	{
		uint8_t bit = (byte & 1<<(7-i))>>(7-i);
		if(bit == 1)
		{
			uint8_t xDraw = (x+i)%unscaledWidth;
			uint8_t yDraw = y%unscaledHeight;
			retVal |= (bit&screenFill[xDraw][yDraw]);
			setPixel(xDraw, yDraw, bit^screenFill[xDraw][yDraw]);
		}
	}
	return retVal;
}

Renderer::~Renderer()
{
    SDL_DestroyWindow( window );
    SDL_Quit();
}