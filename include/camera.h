#include <SDL3/SDL.h>
#include "types.h"

#ifndef CAMERA_H
#define CAMERA_H

void cam_process_key_event(SDL_KeyboardEvent kb_event, m_vec2_t *cam_pos, uint32_t sdl_kb_event_type);

#endif // CAMERA_H
