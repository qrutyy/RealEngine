#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include "types.h"
#include "scene.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define WINDOW_NAME "RealEngine"

typedef struct cam {
    int x;
    int y;
    int width;
    int height;
} cam_t;

typedef struct app_hlpr {
    SDL_Window* window;
	SDL_KeyboardEvent key_event;
    SDL_Surface *grid[SCENE_WIDTH][SCENE_HEIGHT];
	cam_t cam;
    bool is_running;
} app_hlpr_t ;


app_hlpr_t* app_create(void);
void app_destroy(app_hlpr_t* app);
void app_run(app_hlpr_t* app);

#endif // APP_H
