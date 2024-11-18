#ifndef EVENT_H
#define EVENT_H

#include "init.h"

#include <SDL2/SDL.h>

void ResizeWindow(void);

void EventWindow(SDL_WindowEvent e);

void Event(SDL_Event *e, Fusee* pFusee);

#endif
