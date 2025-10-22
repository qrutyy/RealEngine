#include "app.h"
#include "SDL3/SDL_events.h"
#include "render.h"
#include "log.h"
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
				cam_process_key_event(app->key_event, &app->cam_pos, event.type);
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

    app->renderer = SDL_CreateRenderer(app->window, NULL);
    if (!app->renderer) {
		log_debug("app render failed\n");
		SDL_Quit();
		goto err_ex;
    }

    app->cam_pos.x = 0;
    app->cam_pos.y = 0;

    app->is_running = true;
    return app;

err_ex:
    free(app);
    return NULL;
}

void app_destroy(app_hlpr_t *app) {
    if (!app) return;
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    free(app);
}

static void update(void) {
	// game logic
}

void app_run(app_hlpr_t *app) {
    while (app->is_running) {
        process_input(app);
        update();
        render_scene(app);
        // SDL_Delay(16);
    }
}
