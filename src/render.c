#include "render.h"
#include "app.h"
#include <stdint.h>
#include <stdio.h>

void draw_scene(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    const SDL_Rect rect_src = {0, 0, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect rect_dest = {0, 0, TILE_WIDTH, TILE_HEIGHT};

    int cam_x = app->cam_pos.x;
    int cam_y = app->cam_pos.y;

    SDL_FillSurfaceRect(screen, NULL, 0);

    SDL_Surface *image = SDL_LoadPNG(TEST_TILE);

    for (int y = 0; y < SCENE_HEIGHT; ++y) {
        for (int x = 0; x < SCENE_WIDTH; ++x) {
            int grid_x = x - cam_x;
            int grid_y = y - cam_y;
            int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + OFFSET_X;
            int sy = (grid_x + grid_y) * (TILE_HEIGHT/2) + OFFSET_Y;

            rect_dest.x = sx;
            rect_dest.y = sy;

            SDL_BlitSurface(image, &rect_src, screen, &rect_dest);

        }
    }
    SDL_DestroySurface(image);
    SDL_UpdateWindowSurface(window);
}

void render_scene(app_hlpr_t* app) {
    // do preparations, such as
    // find out positions of all the objects
    // and intersect scene with the camera

	draw_scene(app);
}
