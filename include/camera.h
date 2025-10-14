#include <SDL3/SDL.h>
#include "world.h"

#ifndef CAMERA_H
#define CAMERA_H

void cam_process_key_event(SDL_KeyboardEvent kb_event, world_t *world, uint32_t sdl_kb_event_type);

#endif // CAMERA_H
