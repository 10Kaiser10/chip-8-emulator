#include <iostream>
#include <SDL.h>
#include "renderer.h"

Renderer::Renderer(int s)
{
    scale = s;

    rendererFlags = SDL_RENDERER_ACCELERATED;
	windowFlags = 0;

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

void Renderer::setPixel(int x, int y)
{
    SDL_Rect r;
    r.x = scale*x, r.y = scale*y;
    r.h = scale, r.w = scale;

    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderFillRect( renderer, &r );
}

void Renderer::displayScreen(uint32_t delay)
{
    SDL_RenderPresent(renderer); 
    SDL_Delay(delay);
}

void Renderer::drawByte(uint8_t x, uint8_t y, uint8_t byte)
{
	//write this
}

Renderer::~Renderer()
{
    SDL_DestroyWindow( window );
    SDL_Quit();
}