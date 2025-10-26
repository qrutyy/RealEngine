#include <SDL3/SDL.h>
#include "types.h"
#include "app.h"

#ifndef CAMERA_H
#define CAMERA_H

void cam_process_key_event(SDL_KeyboardEvent kb_event, cam_t *cam, entity_t *player_entity, uint32_t sdl_kb_event_type);

#endif // CAMERA_H
