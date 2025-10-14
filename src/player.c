#include <SDL3/SDL.h>
#include "renderer.h"
#include "world.h"
#include "log.h"
#include "utils.h"

void pl_process_key_event(SDL_KeyboardEvent kb_event, world_t *world, uint32_t sdl_kb_event_type) {
    static uint8_t player_wasd = 0;
    if (sdl_kb_event_type == SDL_EVENT_KEY_DOWN) {
        if (kb_event.key == SDLK_W) player_wasd |= 1;
        if (kb_event.key == SDLK_A) player_wasd |= 2;
        if (kb_event.key == SDLK_S) player_wasd |= 4;
        if (kb_event.key == SDLK_D) player_wasd |= 8;
    } else if (sdl_kb_event_type == SDL_EVENT_KEY_UP) {
        if (kb_event.key == SDLK_W) player_wasd &= ~1;
        if (kb_event.key == SDLK_A) player_wasd &= ~2;
        if (kb_event.key == SDLK_S) player_wasd &= ~4;
        if (kb_event.key == SDLK_D) player_wasd &= ~8;
    }
    // Calculate isometric direction
    int dx = 0, dy = 0;
    if (player_wasd & 1) { dx -= 1; dy -= 1; }
    if (player_wasd & 2) { dx -= 1; dy += 1; }
    if (player_wasd & 4) { dx += 1; dy += 1; }
    if (player_wasd & 8) { dx += 1; dy -= 1; }
    if (dx != 0 || dy != 0) {
        world->pl_pos.x = CLAMP(world->pl_pos.x + dx, 0, WORLD_BORDER_X - 1);
        world->pl_pos.y = CLAMP(world->pl_pos.y + dy, 0, WORLD_BORDER_Y - 1);
        // Optionally update camera here, e.g. world->cam_pos = world->pl_pos;
        log_info("Player moved to (iso grid): %d %d", world->pl_pos.x, world->pl_pos.y);
    }
}


