#include <SDL3/SDL.h>
#include "world.h"
#include "log.h"

#define CAM_ZOOM_MAX 3.0f
#define CAM_ZOOM_MIN 0.2f

void cam_process_key_event(SDL_KeyboardEvent kb_event, world_t *world, uint32_t sdl_kb_event_type) {
    if (sdl_kb_event_type != SDL_EVENT_KEY_DOWN) return;
    switch (kb_event.key) {
        case SDLK_UP:
            world->cam_pos.y -= 1;
			world->cam_pos.x -= 1;
            log_info("Camera moved up: %d %d", world->cam_pos.x, world->cam_pos.y);
            break;
        case SDLK_DOWN:
            world->cam_pos.y += 1;
			world->cam_pos.x += 1;
            log_info("Camera moved down: %d %d", world->cam_pos.x, world->cam_pos.y);
            break;
        case SDLK_LEFT:
            world->cam_pos.x -= 1;
			world->cam_pos.y += 1;
            log_info("Camera moved left: %d %d", world->cam_pos.x, world->cam_pos.y);
            break;
        case SDLK_RIGHT:
            world->cam_pos.x += 1;
			world->cam_pos.y -= 1;
            log_info("Camera moved right: %d %d", world->cam_pos.x, world->cam_pos.y);
            break;
		case SDLK_PLUS: // TODO: fix the camera zoom feature
        case SDLK_EQUALS:
            if (world->cam_zoom < CAM_ZOOM_MAX) {
                world->cam_zoom *= 1.1f;
                if(world->cam_zoom > CAM_ZOOM_MAX) world->cam_zoom = CAM_ZOOM_MAX;
                log_info("Camera zoom in: %.2f", world->cam_zoom);
            }
            break;
        case SDLK_MINUS:
            if (world->cam_zoom > CAM_ZOOM_MIN) {
                world->cam_zoom /= 1.1f;
                if(world->cam_zoom < CAM_ZOOM_MIN) world->cam_zoom = CAM_ZOOM_MIN;
                log_info("Camera zoom out: %.2f", world->cam_zoom);
            }
            break;
        default:
            break;
    }
}

