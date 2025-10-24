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

typedef struct grid {
    SDL_Surface ***tiles; // a matrix of pointers to assets
    int tile_num_x;
    int tile_num_y;
    int tile_width; // tile width in pixels
    int tile_height; // tile height in pixels
} grid_t;

typedef struct app_hlpr {
    SDL_KeyboardEvent key_event;
    SDL_Window* window;
    grid_t grid;
	cam_t cam;
    bool is_running;
} app_hlpr_t ;

app_hlpr_t* app_create(void);
void app_destroy(app_hlpr_t* app);
void app_run(app_hlpr_t* app);

/*
    Initialize game grid.

    @param grid a grid handle.
    @param tile_num_x width of a grid in tiles.
    @param tile_num_y height of a grid in tiles.
    @param tile_width width of a tile asset in pixels.
    @param tile_height height of a tile asset in pixels.

    @returns 0 on success, error code otherwise.
*/
int RE_init_grid(grid_t *grid, int tile_num_x, int tile_num_y, int tile_width, int tile_height);

/*
    Destroy game grid.

    @param grid a grid handle.
*/
void RE_destroy_grid(grid_t *grid);


#endif // APP_H
