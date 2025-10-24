#include "app.h"
#include "SDL3/SDL_events.h"
#include "render.h"
#include "log.h"
#include "scene.h"
#include "errors.h"
#include "camera.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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
			if (event.key.key <= SDLK_UP && event.key.key >= SDLK_RIGHT) {
				app->key_event = event.key;	
				cam_process_key_event(app->key_event, &app->cam, event.type);
			}
		}
    }
}

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

    app->is_running = true;
    return app;

err_ex:
    free(app);
    return NULL;
}

void app_destroy(app_hlpr_t *app) {
    if (!app) return;
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    free(app);
}

static void update(void) {
	// game logic
}

void app_run(app_hlpr_t *app) {
    // set cam coords to player entity coords
    app->cam.x = 10;
    app->cam.y = 20;
    while (app->is_running) {
        process_input(app);
        update();
        render_scene(app);
        // SDL_Delay(16);
    }
}

int RE_init_grid(grid_t *grid, int tile_num_x, int tile_num_y, int tile_width, int tile_height) {
    SDL_Surface ***tiles;

    if (!grid) {
        log_debug("Failed to init grid: a grid pointer should not be NULL pointer.\n");
        return ERR_ARGS;
    }

    tiles = malloc(sizeof(SDL_Surface **) * tile_num_x);
    if (!tiles) {
        log_error("Failed to allocate memory for tiles.\n");
        return ERR_NOMEM;
    }

    for (int k = 0; k < tile_num_x; k++) {
        tiles[k] = malloc(sizeof(SDL_Surface *) * tile_num_y);
        if (!tiles[k]) {
            log_error("Failed to allocate memory for tiles[%d].\n", k);
            for (int f = 0; f < k; f++) {
                free(tiles[f]);
            }
            free(tiles);
            return ERR_NOMEM;
        }
        for (int j = 0; j < tile_num_y; j++) {
            tiles[k][j] = NULL;
        }
    }

    grid->tiles = tiles;
    grid->tile_num_x = tile_num_x;
    grid->tile_num_y = tile_num_y;
    grid->tile_width = tile_width;
    grid->tile_height = tile_height;
    log_debug("Initialized grid %d x %d", tile_num_x, tile_num_y);

    return 0;
}
