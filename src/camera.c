#include <SDL3/SDL.h>
#include "log.h"
#include "types.h"
#include "app.h"

#define CAM_ZOOM_MAX 3.0f
#define CAM_ZOOM_MIN 0.2f

void cam_process_key_event(SDL_KeyboardEvent kb_event, cam_t *cam, entity_t *player_entity, uint32_t sdl_kb_event_type) {
    if (sdl_kb_event_type != SDL_EVENT_KEY_DOWN) return;
    switch (kb_event.key) {
        case SDLK_UP:
            cam->y -= 1;
            cam->x -= 1;
            player_entity->x -= 1;
            player_entity->y -= 1;
            // log_debug("Camera moved up: %d %d", cam->x, cam->y);
            break;
        case SDLK_DOWN:
            cam->y += 1;
			cam->x += 1;
            player_entity->x += 1;
            player_entity->y += 1;
            // log_debug("Camera moved down: %d %d", cam->x, cam->y);
            break;
        case SDLK_LEFT:
            cam->x -= 1;
            cam->y += 1;
            player_entity->x -= 1;
            player_entity->y += 1;
            // log_debug("Camera moved left: %d %d", cam->x, cam->y);
            break;
        case SDLK_RIGHT:
            cam->x += 1;
            cam->y -= 1;
            player_entity->x += 1;
            player_entity->y -= 1;
            // log_debug("Camera moved right: %d %d", cam->x, cam->y);
            break;
        default:
            break;
    }
}

