#include "app.h"
#include "SDL3/SDL_events.h"
#include "renderer.h"
#include "log.h"
#include "world.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define ZOOM_MAX 3.0f
#define ZOOM_MIN 0.2f

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
        case SDLK_PLUS:
        case SDLK_EQUALS:
            if (world->cam_zoom < ZOOM_MAX) {
                world->cam_zoom *= 1.1f;
                if(world->cam_zoom > ZOOM_MAX) world->cam_zoom = ZOOM_MAX;
                log_info("Camera zoom in: %.2f", world->cam_zoom);
            }
            break;
        case SDLK_MINUS:
            if (world->cam_zoom > ZOOM_MIN) {
                world->cam_zoom /= 1.1f;
                if(world->cam_zoom < ZOOM_MIN) world->cam_zoom = ZOOM_MIN;
                log_info("Camera zoom out: %.2f", world->cam_zoom);
            }
            break;
        default:
            break;
    }
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
    world_t *world = app->world;
    float zoom = world->cam_zoom; // TODO: fix zoom feature
    int cam_x = world->cam_pos.x;
    int cam_y = world->cam_pos.y;
    // Draw grid (with zoom/camera offset adjustment)
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int grid_x = x - cam_x;
            int grid_y = y - cam_y;
            int sx = (grid_x - grid_y) * TILE_WIDTH_HALF * zoom + OFFSET_X;
            int sy = (grid_x + grid_y) * TILE_HEIGHT_HALF * zoom + OFFSET_Y;
            SDL_FPoint points[5] = {
                { (float)sx, (float)(sy + TILE_HEIGHT_HALF * zoom) },
                { (float)(sx + TILE_WIDTH_HALF * zoom), (float)sy },
                { (float)sx, (float)(sy - TILE_HEIGHT_HALF * zoom) },
                { (float)(sx - TILE_WIDTH_HALF * zoom), (float)sy },
                { (float)sx, (float)(sy + TILE_HEIGHT_HALF * zoom) }
            };
            SDL_SetRenderDrawColor(app->renderer, 100, 120, 150, 255);
            SDL_RenderLines(app->renderer, points, 5);
        }
    }
    // Draw player
    int tile_x = world->pl_pos.x - cam_x;
    int tile_y = world->pl_pos.y - cam_y;
    int sx = (tile_x - tile_y) * TILE_WIDTH_HALF * zoom + OFFSET_X;
    int sy = (tile_x + tile_y) * TILE_HEIGHT_HALF * zoom + OFFSET_Y;
    SDL_FRect player_rect = {(float)(sx-8*zoom), (float)(sy-8*zoom), 16.0f*zoom, 16.0f*zoom};
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
