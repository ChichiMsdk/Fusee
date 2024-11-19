#include "fusee.h"

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

void
FuseeDraw(Fusee* pFusee)
{
	SDL_SetRenderDrawColor(gInst.pRenderer, 0, 0, 255, 255);
	pFusee->origin.x = (float) pFusee->box.w / 2;
	pFusee->origin.y = (float) pFusee->box.h / 2;

	pFusee->fire.origin.x = (float) pFusee->box.w / 2;
	pFusee->fire.origin.y = (float) pFusee->box.h / 2;

	SDL_Rect fuseeRect = {
		.x = pFusee->position.x - pFusee->origin.x,
		.y = pFusee->position.y - pFusee->origin.y,
		.w = pFusee->box.w,
		.h = pFusee->box.h,
	};

	SDL_Rect fireRect = {
		.x = pFusee->fire.position.x - pFusee->fire.origin.x,
		.y = pFusee->fire.position.y + pFusee->fire.origin.y,
		.w = pFusee->box.w,
		.h = pFusee->box.h,
	};

	SDL_Point fireOffset = {.x = pFusee->fire.origin.x, .y = pFusee->fire.origin.y};

	SDL_Point originFusee = {.x = pFusee->origin.x, .y = pFusee->origin.y};

    // Update the fire texture's position accordingly

	if (pFusee->firing == true)
	{
		SDL_RenderCopyEx(
				gInst.pRenderer,
				pFusee->fire.fireTexture,
				NULL,
				&fireRect,
				pFusee->angle,
				&fireOffset,
				SDL_FLIP_NONE);
	}

	SDL_RenderCopyEx(
			gInst.pRenderer,
			pFusee->fuseeTexture,
			NULL,
			&fuseeRect,
			pFusee->angle,
			&originFusee,
			SDL_FLIP_NONE);
	/* SDL_RenderFillRect(gInst.pRenderer, &rect); */
}

void
FuseeUpdate(Fusee* pFusee, float gravity)
{
	pFusee->velocity.y += gravity;
	pFusee->position.x += pFusee->velocity.x;
	pFusee->position.y += pFusee->velocity.y;
	FuseeCheck(pFusee);
}

void
FuseeInit(Fusee* pFusee)
{
	pFusee->position.x = (float) gInst.width / 2;
	pFusee->position.y = (float) gInst.height / 2;
	pFusee->box.w = 30;
	pFusee->box.h = 150;
	pFusee->power = 0.3f;
	pFusee->fire.position.x = pFusee->position.x;
	pFusee->fire.position.y = pFusee->position.y + pFusee->box.h;
	pFusee->origin.x = pFusee->position.x / 2;
	pFusee->origin.y = pFusee->position.y / 2;

	int flags = IMG_INIT_PNG;
	if (IMG_Init(flags) != flags)
		goto ErrorCheck;

	pFusee->fuseeTexture = IMG_LoadTexture(gInst.pRenderer, "assets/rocket.png");
	if (!pFusee->fuseeTexture)
		goto ErrorCheck;

	pFusee->fire.fireTexture = IMG_LoadTexture(gInst.pRenderer, "assets/flame.png");
	if (!pFusee->fire.fireTexture)
		goto ErrorCheck;

	return ; 

ErrorCheck:
	fprintf(stderr, "Error %s:%d: %s", __FILE__, __LINE__, SDL_GetError());
	exit(1);
}
