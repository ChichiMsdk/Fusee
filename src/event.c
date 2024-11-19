#include "event.h"

#include <stdbool.h>
#include <math.h>
#include <stdio.h>

void
ResizeWindow(void)
{
	SDL_GetWindowSize(gInst.pWin, &gInst.width, &gInst.height);
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
	float rotationSpeed = 0.3f;
	while(SDL_PollEvent(e))
	{
		if (e->type == SDL_QUIT) { gInst.running = false; }
		else if (e->type == SDL_WINDOWEVENT) EventWindow(e->window);
		else if (e->type == SDL_KEYDOWN) 
		{
			switch (e->key.keysym.sym)
			{
				case (SDLK_ESCAPE):
					gInst.running = false; 
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
	const uint8_t* pKeys = SDL_GetKeyboardState(NULL);

	if (pKeys[SDL_SCANCODE_LEFT] && pKeys[SDL_SCANCODE_SPACE] && pFusee->position.y > 0 && pFusee->position.y < gInst.height - pFusee->box.h)
	{
		pFusee->angle -= rotationSpeed;
		pFusee->fire.angle -= rotationSpeed;
		if (pFusee->angle < 0)
			pFusee->angle += 360;
		if (pFusee->fire.angle < 0)
			pFusee->fire.angle += 360;
		pFusee->firing = true;
	}
	else if (pKeys[SDL_SCANCODE_RIGHT] && pKeys[SDL_SCANCODE_SPACE] && pFusee->position.y > 0 && pFusee->position.y < gInst.height - pFusee->box.h)
	{
		pFusee->angle += rotationSpeed;
		pFusee->fire.angle += rotationSpeed;
		if (pFusee->angle >= 360)
			pFusee->angle -= 360;
		if (pFusee->fire.angle >= 360)
			pFusee->fire.angle -= 360;
		pFusee->firing = true;
	}
	else if (pKeys[SDL_SCANCODE_SPACE])
	{
		float radians = pFusee->angle * M_PI / 180.0f;
		pFusee->velocity.x += pFusee->power * sin(radians);
		pFusee->velocity.y -= pFusee->power * cos(radians);

		radians = pFusee->angle * M_PI / 180.0f;
		pFusee->fire.velocity.x += pFusee->power * sin(radians);
		pFusee->fire.velocity.y -= pFusee->power * cos(radians);

		pFusee->firing = true;
	}
	pFusee->firing = false;
}
