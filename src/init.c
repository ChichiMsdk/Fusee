#include "init.h"

#include <stdio.h>

void 
InitSDL(Instance *inst)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		printf("SDLfailed!\nSDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	// Either VSYNC or Vulkan causes problems
	SDL_Window *window = SDL_CreateWindow(
			"Fusee",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			gInst.width,
			gInst.height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);

	if (!window)
	{
		fprintf(stderr, "Window failed\nSDL_Error: %s\n", SDL_GetError()); 
		exit(1);
	}
	int index = -1;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, index, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		fprintf(stderr, "Renderer failed!\nSDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	if(SDL_RenderSetVSync(renderer, 1))
	{
		fprintf(stderr, "Vsync failed!\nSDL_Error: %s\n", SDL_GetError());
	}
	gInst.pRenderer = renderer;
	gInst.pWin = window;
}
