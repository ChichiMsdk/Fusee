#include "event.h"

#include <stdio.h>

void
ResizeWindow(void)
{
	printf("gInst.width %d, gInst.height %d\t->\t", gInst.width, gInst.height);
	SDL_GetWindowSize(gInst.pWin, &gInst.width, &gInst.height);
	printf("gInst.width %d, gInst.height %d\n", gInst.width, gInst.height);
}

void
EventWindow(SDL_WindowEvent e)
{
	switch (e.event)
	{
		/* case (SDL_WINDOWEVENT_EXPOSED): */
		case (SDL_WINDOWEVENT_RESIZED):
		case (SDL_WINDOWEVENT_MAXIMIZED):
		case (SDL_WINDOWEVENT_SIZE_CHANGED):
			ResizeWindow();
			break;
	}
}

void
Event(SDL_Event *e, Fusee* pFusee)
{
	float rotationSpeed = 1.0f;
	while(SDL_PollEvent(e))
	{
		/* sInfo sInfo = {.nbrPages = 5, 3, 800, 0}; */
		if (e->type == SDL_QUIT) { gInst.running = false; }
		else if (e->type == SDL_WINDOWEVENT) EventWindow(e->window);
		else if (e->type == SDL_KEYDOWN) 
		{
			switch (e->key.keysym.sym)
			{
				case (SDLK_ESCAPE):
					gInst.running = false; 
					break;
				case (SDLK_RIGHT):
					pFusee->angle += rotationSpeed;
					if (pFusee->angle >= 360)
						pFusee->angle -= 360;
					break;
				case (SDLK_LEFT):
					pFusee->angle -= rotationSpeed;
					if (pFusee->angle < 0)
						pFusee->angle += 360;
					break;
				case (SDLK_SPACE):
					float radians = pFusee->angle * M_PI / 180.0f;
					pFusee->velocity.x += pFusee->power * sin(radians);
					pFusee->velocity.y -= pFusee->power * cos(radians);
					break;
				case (SDLK_h):
				case (SDLK_l):
				case (SDLK_j):
				case (SDLK_k):
				case (SDLK_1):
				case (SDLK_2):
				case (SDLK_3):
					break;
			}
		}
		else if (e->type == SDL_MOUSEWHEEL)
		{
		}
	}
}
