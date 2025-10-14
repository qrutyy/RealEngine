#include <SDL3/SDL.h>
#include "world.h"

#ifndef PLAYER_H
#define PLAYER_H

void pl_process_key_event(SDL_KeyboardEvent kb_event, world_t *world, uint32_t sdl_kb_event_type); 

#endif
