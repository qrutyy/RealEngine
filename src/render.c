#include "render.h"
#include "app.h"
#include "log.h"
#include "asset.h"
#include <stdint.h>

void render_background(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    
    int TILE_WIDTH = app->grid.tile_width;
    int TILE_HEIGHT = app->grid.tile_height;

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

    int screen_center_x = screen->w / 2;
    int screen_center_y = screen->h / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_Surface *image = app->grid.tiles[x][y];
            if (!image) {
                continue;
            }

            int grid_x = x - cam_x;
            int grid_y = y - cam_y;
            int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + WINDOW_WIDTH/2 - TILE_WIDTH/2;
            int sy = (grid_x + grid_y) * (TILE_HEIGHT/2) + WINDOW_HEIGHT/2 - TILE_HEIGHT/2;

            SDL_Rect rect_dest = {sx, sy, TILE_WIDTH, TILE_HEIGHT};

            SDL_BlitSurface(image, NULL, screen, &rect_dest);

            // log_debug("load tile on x,y: %d, %d\n", sx, sy);
        }
    }
}

void render_entities(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    int num = app->entities_num;
    entity_t *entities = app->entities;

    int layers_num = app->layers_num;
    layer_entities_t *layers = app->lentities;

    int TILE_WIDTH = app->grid.tile_width;
    int TILE_HEIGHT = app->grid.tile_height;

    // need to sort entities by layers. then
    // for (l in layers) { for (e in layer.entities) } 
    // for (int i = 0; i < num; i++) {
        // TODO: render only if entity is in camera

    for (int l = 0; l < layers_num; l++) {
        for (int i = 0; i < layers[l].num_entities; i++) {

            // entity_t ent = entities[i];

            entity_t ent = layers[l].entities[i];

            asset_t *asset = RE_get_asset(2);
            int asset_h = asset->height;
            int asset_w = asset->width;
            SDL_Surface *image = asset->img;

            int cam_x = app->cam.x;
            int cam_y = app->cam.y;
            int x = ent.x;
            int y = ent.y;
            int grid_x = x - cam_x;
            int grid_y = y - cam_y;

            int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + WINDOW_WIDTH/2 - asset_w/2;
            int sy = (grid_x + grid_y) * (TILE_HEIGHT/2) + WINDOW_HEIGHT/2 - asset_h/2;

            int TILE_WIDTH = app->grid.tile_width;
            int TILE_HEIGHT = app->grid.tile_height;

            // int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + (OFFSET_X - asset->width / 2);
            // int sy = (grid_x + grid_y) * (TILE_HEIGHT/2);

            SDL_Rect rect = {sx, sy, 0, 0};

            SDL_BlitSurface(image, NULL, screen, &rect);
            // log_debug("rendered entity %d in %d, %d", i, ent.x, ent.y);
        }
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
}
