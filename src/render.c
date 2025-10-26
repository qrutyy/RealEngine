#include "render.h"
#include "app.h"
#include "log.h"
#include "asset.h"
#include <stdint.h>

// this should be with other sprites, it's here only temporarily
static SDL_Surface *image_n, *image_s, *image_e, *image_w;

void render_background(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    
    int TILE_WIDTH = app->grid.tile_width;
    int TILE_HEIGHT = app->grid.tile_height;

    SDL_Rect rect_dest = {0, 0, TILE_WIDTH, TILE_HEIGHT};

    int cam_x = app->cam.x;
    int cam_y = app->cam.y;

    // light blue for sky
    Uint32 color = SDL_MapSurfaceRGB(screen, 144, 213, 255);

    SDL_FillSurfaceRect(screen, NULL, color);

    if (!app->grid.tiles) {
        log_debug("NO TILES!\n");
        return;
    }

    int width = app->grid.tile_num_x;
    int height = app->grid.tile_num_y;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_Surface *image = app->grid.tiles[x][y];
            if (!image) {
                continue;
            }

            int grid_x = x - cam_x;
            int grid_y = y - cam_y;
            int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + OFFSET_X;
            int sy = (grid_x + grid_y) * (TILE_HEIGHT/2) + OFFSET_Y;

            rect_dest.x = sx;
            rect_dest.y = sy;

            SDL_BlitSurface(image, NULL, screen, &rect_dest);

            // log_debug("%d, %d: load tile %p\n", x, y, image);
        }
    }
    // SDL_DestroySurface(image);
    // SDL_UpdateWindowSurface(window);
}

void render_main_char(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    int TILE_WIDTH = app->grid.tile_width;
    int TILE_HEIGHT = app->grid.tile_height;

    const SDL_Rect rect_src = {MAIN_CHAR_SRC_X, MAIN_CHAR_SRC_Y, MAIN_CHAR_WIDTH, MAIN_CHAR_HEIGHT};
    SDL_Rect rect_dest = {0, 0, 0, 0};

    SDL_Surface *image;

    // all images should be loaded BEFORE the render phase and CLEARED after
    switch (app->key_event.key) {
        case SDLK_UP:
            if (!image_n) {
                image_n = SDL_LoadPNG(MAIN_CHAR_IMG_N);
            }
            image = image_n;
            // log_debug("Camera moved up: %d %d", cam->x, cam->y);
            break;
        case SDLK_LEFT:
            if (!image_w) {
                image_w = SDL_LoadPNG(MAIN_CHAR_IMG_W);
            }
            image = image_w;
            // log_debug("Camera moved left: %d %d", cam->x, cam->y);
            break;
        case SDLK_RIGHT:
            if (!image_e) {
                image_e = SDL_LoadPNG(MAIN_CHAR_IMG_E);
            }
            image = image_e;
            // log_debug("Camera moved right: %d %d", cam->x, cam->y);
            break;
        default:
        case SDLK_DOWN:
            if (!image_s) {
                image_s = SDL_LoadPNG(MAIN_CHAR_IMG_S);
            }
            image = image_s;
            // log_debug("Camera moved down: %d %d", app->cam.x, app->cam.y);
            break;
    }
    // log_debug("Camera moved: %d %d", app->cam.x, app->cam.y);


    if (!image) {
        log_debug("Failed to load main character image.\n");
        return;
    }

    SDL_BlitSurface(image, &rect_src, screen, &rect_dest);
}

void render_entities(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    int num = app->entities_num;
    entity_t *entities = app->entities;

    // need to sort entities by layers. then
    // for (l in layers) { for (e in layer.entities) } 
    for (int i = 0; i < num; i++) {
        // ADD render only if entity is in camera

        entity_t ent = entities[i];

        asset_t *asset = RE_get_asset(2);
        SDL_Surface *image = asset->img;

        int cam_x = app->cam.x;
        int cam_y = app->cam.y;
        int x = ent.x;
        int y = ent.y;
        int grid_x = x - cam_x;
        int grid_y = y - cam_y;

        int TILE_WIDTH = app->grid.tile_width;
        int TILE_HEIGHT = app->grid.tile_height;

        int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + (OFFSET_X - asset->width / 2);
        int sy = (grid_x + grid_y) * (TILE_HEIGHT/2);

        SDL_Rect rect = {sx, sy, 0, 0};

        SDL_BlitSurface(image, NULL, screen, &rect);

        for (int j = 0; j < 10000000; j++) {}
        // log_debug("rendered entity %d in %d, %d", i, ent.x, ent.y);
    }

}

void render_scene(app_hlpr_t* app) {
    // do preparations, such as
    // find out positions of all the objects in the camera
    // intersect scene with the camera to not draw extra things

    // intersect_camera_scene(app);

	render_background(app);
    render_entities(app);
    SDL_UpdateWindowSurface(app->window);
	printf("%s\n", SDL_GetError());
}
