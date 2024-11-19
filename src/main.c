#include "init.h"
#include "event.h"
#include <stdio.h>
#include <math.h>

Instance gInst = {.width = 1500, .height = 800, .running = 1};

static inline void
FuseeDraw(Fusee* pFusee)
{
	SDL_SetRenderDrawColor(gInst.pRenderer, 0, 0, 255, 255);
	pFusee->origin.x = (float) pFusee->box.w / 2;
	pFusee->origin.y = (float) pFusee->box.h / 2;

	SDL_Rect rect = {
		.x = pFusee->position.x - pFusee->origin.x,
		.y = pFusee->position.y - pFusee->origin.y,
		.w = pFusee->box.w,
		.h = pFusee->box.h,
	};

	SDL_Point origin = {.x = pFusee->origin.x, .y = pFusee->origin.y};
	SDL_RenderCopyEx(gInst.pRenderer, pFusee->texture, NULL, &rect, pFusee->angle, &origin, SDL_FLIP_NONE);
	/* SDL_RenderFillRect(gInst.pRenderer, &rect); */
}

static inline void
FuseeCheck(Fusee* pFusee)
{
	if (pFusee->position.x <= 0)
	{
		pFusee->position.x = 0;
		pFusee->velocity.x = 0;
	}

	if (pFusee->position.x + pFusee->box.w >= gInst.width) 
	{
		pFusee->position.x = gInst.width - pFusee->box.w;
		pFusee->velocity.x = 0;
	}

	if (pFusee->position.y <= 0)
	{
		pFusee->position.y = 0;
		pFusee->velocity.y = 0;
		pFusee->velocity.x = 0;
	}

	if (pFusee->position.y + pFusee->box.h >= gInst.height) 
	{
		pFusee->position.y = gInst.height - pFusee->box.h;
		pFusee->velocity.y = 0;
		pFusee->velocity.x = 0;
	}
}

static inline void
FuseeUpdate(Fusee* pFusee)
{
	pFusee->position.x += pFusee->velocity.x;
	pFusee->position.y += pFusee->velocity.y;
	FuseeCheck(pFusee);
}

float gravity = 0.1f;

int
main(int argc, char** ppArgv)
{
	SDL_Event	e;

	Fusee fusee = {
		.position.x = (float) gInst.width / 2,
		.position.y = (float) gInst.height / 2,
		.box.w = 30,
		.box.h = 150,
		.power = 0.3f,
	};

	InitSDL(&gInst);

	fusee.origin.x = fusee.position.x / 2;
	fusee.origin.y = fusee.position.y / 2;
	int flags = IMG_INIT_PNG;
	if (IMG_Init(flags) != flags)
	{
		fprintf(stderr, "Error %s:%d: %s", __FILE__, __LINE__, SDL_GetError());
		exit(1);
	}

	/* fusee.texture = SDL_CreateTexture(gInst.pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
	   fusee.box.w, fusee.box.h); */

	fusee.texture = IMG_LoadTexture(gInst.pRenderer, "assets/rocket.png");
	if (!fusee.texture)
	{
		fprintf(stderr, "Error %s:%d: %s", __FILE__, __LINE__, SDL_GetError());
		exit(1);
	}

	while (gInst.running)
	{
		Event(&e, &fusee);

		SDL_SetRenderDrawColor(gInst.pRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gInst.pRenderer);

		fusee.velocity.y += gravity;
		FuseeUpdate(&fusee);
		FuseeDraw(&fusee);

		SDL_RenderPresent(gInst.pRenderer);
		SDL_Delay(16);
	}
	IMG_Quit();
	SDL_DestroyTexture(fusee.texture);
	SDL_DestroyRenderer(gInst.pRenderer);
	SDL_DestroyWindow(gInst.pWin);
	SDL_Quit();
	return 0;
}
