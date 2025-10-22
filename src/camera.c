#include <SDL3/SDL.h>
#include "log.h"
#include "types.h"

#define CAM_ZOOM_MAX 3.0f
#define CAM_ZOOM_MIN 0.2f

void cam_process_key_event(SDL_KeyboardEvent kb_event, m_vec2_t *cam_pos, uint32_t sdl_kb_event_type) {
    if (sdl_kb_event_type != SDL_EVENT_KEY_DOWN) return;
    switch (kb_event.key) {
        case SDLK_UP:
            cam_pos->y -= 1;
			cam_pos->x -= 1;
            // log_debug("Camera moved up: %d %d", cam_pos->x, cam_pos->y);
            break;
        case SDLK_DOWN:
            cam_pos->y += 1;
			cam_pos->x += 1;
            // log_debug("Camera moved down: %d %d", cam_pos->x, cam_pos->y);
            break;
        case SDLK_LEFT:
            cam_pos->x -= 1;
			cam_pos->y += 1;
            // log_debug("Camera moved left: %d %d", cam_pos->x, cam_pos->y);
            break;
        case SDLK_RIGHT:
            cam_pos->x += 1;
			cam_pos->y -= 1;
            // log_debug("Camera moved right: %d %d", cam_pos->x, cam_pos->y);
            break;
        default:
            break;
    }
}

