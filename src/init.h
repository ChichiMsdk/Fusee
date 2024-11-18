#ifndef INIT_H
#define INIT_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define MAX_VISIBLE_PAGES 20

typedef struct Instance
{
	SDL_Renderer	*pRenderer;
	SDL_Window		*pWin;
	int				width;
	int				height;
	int				running;

}Instance;

typedef struct Vec2
{
	float x;
	float y;
} Vec2;

typedef struct Fusee
{
	Vec2			velocity;
	Vec2			position;
	Vec2			origin;
	float			power;
	float			angle;
	SDL_Rect		box;

	SDL_Texture*	texture;
} Fusee;

typedef struct Canvas
{
	int x, y;
	int w, h;
	SDL_Color color;
}Canvas;

extern Instance gInst;

void InitSDL(Instance *inst);

#endif //INIT_H
