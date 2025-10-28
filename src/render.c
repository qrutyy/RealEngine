#include "render.h"
#include "app.h"
#include "log.h"
#include "asset.h"
#include "utils.h"
#include <stdint.h>

void render_asset(SDL_Surface *screen, int sx, int sy, asset_t *asset) {
    if (!screen) {
        log_debug("Given null screen, I give up.");
        return;
    }
    if (!asset) {
        log_debug("Given null asset, I give up.");
        return;
    }

    int asset_w = asset->width;
    int asset_h = asset->height;

    if (asset_w < 0) {
        log_debug("Given negative width %d, I give up.", asset_w);
        return;
    }
    if (asset_h < 0) {
        log_debug("Given negative height %d, I give up.", asset_h);
        return;
    }

    int bytes_per_pxl = (screen->pitch / screen->w);

    for (int tile_x = 0; tile_x < asset_w; tile_x++) {
        for (int tile_y = 0; tile_y < asset_h; tile_y++) {
            // log_debug ("x, y: %d, %d, tile_x, tile_y: %d, %d", x, y, tile_x, tile_y);

            int screen_x = sx + tile_x;
            int screen_y = sy + tile_y;

            if (screen_x < 0 || screen_x >= WINDOW_WIDTH || screen_y < 0 || screen_y >= WINDOW_HEIGHT) {
                continue;
            }

            SDL_Surface *img = asset->img;

            Uint32 *const screen_pxl = (Uint32*) ((Uint8 *) screen->pixels + screen_y * screen->pitch + screen_x * bytes_per_pxl);
            Uint32 *asset_pxl = (Uint32*) ((Uint8 *) img->pixels + tile_y * img->pitch + tile_x * bytes_per_pxl);
            // *screen_pxl = *new_pixel;

            Uint8 ar, ag, ab, aa;

            const SDL_PixelFormatDetails *aformat = SDL_GetPixelFormatDetails(img->format);
            SDL_GetRGBA(*asset_pxl, aformat, NULL, &ar, &ag, &ab, &aa);
            
            //     *screen_pxl = *asset_pxl;
            // }
            if (aa == 255) {
                *screen_pxl = *asset_pxl;
            } else if (aa > 0) {
                Uint8 sr, sg, sb;
                const SDL_PixelFormatDetails *sformat = SDL_GetPixelFormatDetails(screen->format);
                
                SDL_GetRGB(*screen_pxl, sformat, NULL, &sr, &sg, &sb);

                float norm_aa = aa / 255.0f;

                Uint8 r = MIN(255, sr + ar * norm_aa);
                Uint8 g = MIN(255, sg + ag * norm_aa);
                Uint8 b = MIN(255, sb + ab * norm_aa);
                
                *screen_pxl = SDL_MapRGB(sformat, NULL, r, g, b);
            }
        }
    }

}

// render background should be way more easy.
// to do that, need to store full background surface,
// then extract needed piece 
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

    int tile_num_x = app->grid.tile_num_x;
    int tile_num_y = app->grid.tile_num_y;

    int screen_center_x = screen->w / 2;
    int screen_center_y = screen->h / 2;

    int bytes_per_pxl = (screen->pitch / screen->w);

    for (int y = 0; y < tile_num_x; ++y) {
        for (int x = 0; x < tile_num_y; ++x) {
            SDL_Surface *image = app->grid.tiles[x][y];
            if (!image) {
                continue;
            }

            int grid_x = x - cam_x;
            int grid_y = y - cam_y;
            int sx = (grid_x - grid_y) * (TILE_WIDTH/2) + WINDOW_WIDTH/2 - TILE_WIDTH/2;
            int sy = (grid_x + grid_y) * (TILE_HEIGHT/2) + WINDOW_HEIGHT/2 - TILE_HEIGHT/2;

            for (int tile_x = 0; tile_x < TILE_WIDTH; tile_x++) {
                for (int tile_y = 0; tile_y < TILE_HEIGHT; tile_y++) {
                    // log_debug ("x, y: %d, %d, tile_x, tile_y: %d, %d", x, y, tile_x, tile_y);

                    int screen_x = sx + tile_x;
                    int screen_y = sy + tile_y;

                    if (screen_x < 0 || screen_x >= WINDOW_WIDTH || screen_y < 0 || screen_y >= WINDOW_HEIGHT) {
                        continue;
                    }

                    Uint32 *const screen_pxl = (Uint32*) ((Uint8 *) screen->pixels + screen_y * screen->pitch + screen_x * bytes_per_pxl);
                    Uint32 *new_pixel = (Uint32*) ((Uint8 *) image->pixels + tile_y * image->pitch + tile_x * bytes_per_pxl);
                    // *screen_pxl = *new_pixel;

                    Uint8 r, g, b, a;

                    const SDL_PixelFormatDetails *format = SDL_GetPixelFormatDetails(image->format);
                    SDL_GetRGBA(*new_pixel, format, NULL, &r, &g, &b, &a);
                    
                    if (a > 0) {
                        *screen_pxl = *new_pixel;
                    }
                }
            }
        }
    }

    // int bytes_per_pxl = 4;
    // int* pixels = screen->pixels;
    // for (int x = 0; x < 100; x++) {
    //     for (int y = 0; y < screen->h; y++) {
    //         Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) screen->pixels + y * screen->pitch + x * bytes_per_pxl);
    //         *target_pixel = 0; // set to some pixel
    //     }
    // }
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

            // asset_t *asset = RE_get_asset(2);
            animation_t anim = ent.animations[ent.curr_animation];
            asset_t *asset = &anim.assets[anim.curr_asset];
            if (!asset) {
                continue;
            }
            int asset_h = asset->height;
            int asset_w = asset->width;
            SDL_Surface *image = asset->img;
            if (!image) {
                continue;
            }

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

            render_asset(screen, sx, sy, asset);
            
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
