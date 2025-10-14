#include "app.h"
#include "SDL3/SDL_events.h"
#include "renderer.h"
#include "log.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


app_hlpr_t* app_create(void) {
    app_hlpr_t* app = calloc(1, sizeof(struct app_hlpr));
    if (!app) {
        log_error("Failed to allocate app\n");
        return NULL;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        log_error("SDL_Init Error: %s\n", SDL_GetError());
		goto err_ex;
    }

    app->window = SDL_CreateWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!app->window) {
        log_error("sdl_createwindow error: %s\n", SDL_GetError());
		goto err_ex;
    }

    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (!app->renderer) {
		log_debug("app render failed\n");
		SDL_Quit();
		goto err_ex;
    }

	app->world = world_create();
	if (!app->world) {
		SDL_Quit();
		goto err_ex;
	}

    app->is_running = true;
    return app;

err_ex:
    free(app);
    return NULL;
}

void app_destroy(app_hlpr_t* app) {
    if (!app) return;
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    free(app);
}

static void pl_process_key_event(SDL_KeyboardEvent kb_event, world_t *world, uint32_t sdl_kb_event_type) {
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
        world->pl_pos.x += dx;
        world->pl_pos.y += dy;
        // Optionally update camera here, e.g. world->cam_pos = world->pl_pos;
        log_info("Player moved to (iso grid): %d %d", world->pl_pos.x, world->pl_pos.y);
    }
}

static void cam_process_key_event(SDL_KeyboardEvent kb_event, world_t *world, uint32_t sdl_kb_event_type) {

}

static void process_input(app_hlpr_t* app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            app->is_running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
            app->is_running = false;
        }
		if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			if (event.key.key <= SDLK_Z && event.key.key >= SDLK_A) {
				app->key_event = event.key;	
				pl_process_key_event(app->key_event, app->world, event.type);
			} else if (event.key.key <= SDLK_UP && event.key.key >= SDLK_RIGHT) {
				app->key_event = event.key;	
				cam_process_key_event(app->key_event, app->world, event.type);
			}
		}
    }
}

static void update(void) {
	// game logic
}

static void render(app_hlpr_t* app) {
    SDL_SetRenderDrawColor(app->renderer, 20, 20, 30, 255); 
    SDL_RenderClear(app->renderer);
    // Draw grid
    renderer_draw_grid(app->renderer);
    // Draw player
    int tile_x = app->world->pl_pos.x;
    int tile_y = app->world->pl_pos.y;
    int sx = (tile_x - tile_y) * TILE_WIDTH_HALF + OFFSET_X;
    int sy = (tile_x + tile_y) * TILE_HEIGHT_HALF + OFFSET_Y;
    SDL_FRect player_rect = {(float)(sx-8), (float)(sy-8), 16.0f, 16.0f};
    SDL_SetRenderDrawColor(app->renderer, 255, 100, 30, 255);
    SDL_RenderRect(app->renderer, &player_rect);
    SDL_RenderPresent(app->renderer);
}

void app_run(app_hlpr_t* app) {
    while (app->is_running) {
        process_input(app);
        update();
        render(app);
        SDL_Delay(16);
    }
}
