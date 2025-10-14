#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include "world.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_NAME "RealEngine"

typedef struct app_hlpr {
    SDL_Window* window;
    SDL_Renderer* renderer;
	SDL_KeyboardEvent key_event;
	world_t *world;
    bool is_running;
} app_hlpr_t ;


app_hlpr_t* app_create(void);
void app_destroy(app_hlpr_t* app);
void app_run(app_hlpr_t* app);

#endif // APP_H
