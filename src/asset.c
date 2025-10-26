#include "asset.h"
#include "log.h"
#include "asset.h"
#include "errors.h"
#include "hash.h"
#include <stdlib.h>
#include <SDL3/SDL.h>

static asset_t assets[MAX_ASSETS_NUM];

static int curr_assets_num;

int RE_load_asset(char *filename, int src_x, int src_y, int width, int height) {
    if (curr_assets_num >= MAX_ASSETS_NUM) {
        log_debug("Cannot load asset, reached maximum assets num: %d.\n", MAX_ASSETS_NUM);
        return -1;
    }

    SDL_Surface *src_asset_img = SDL_LoadPNG(filename);
    SDL_Surface *asset_img = SDL_CreateSurface(width, height, src_asset_img->format);

    const SDL_Rect rect = {src_x, src_y, width, height};
    int ret = SDL_BlitSurface(src_asset_img, &rect, asset_img, NULL);
    if (!ret) {
        log_error("Failed to extract asset from image %s with specified parameters.\n", filename);
        return -1;
    }
    SDL_DestroySurface(src_asset_img);

    int id = curr_assets_num++;

    asset_t asset = {.img = asset_img, .width = width, .height = height};
    assets[id] = asset;

    log_debug("Loaded asset from %s with id %d\n", filename, id);

    return id;
}

asset_t *RE_get_asset(int id) {
    if (id > MAX_ASSETS_NUM) {
        log_debug("%d exceeds maximum assets num %d.\n", id, MAX_ASSETS_NUM);
        return NULL;
    }
    return &assets[id];
}

int RE_assign_asset_static(grid_t *grid, int id, int layer, int x, int y) {
    if (id > MAX_ASSETS_NUM) {
        log_debug("%d exceeds maximum assets num %d.\n", id, MAX_ASSETS_NUM);
        return ERR_ARGS;
    }
    asset_t asset = assets[id];

    int width = grid->tile_num_x;
    int height = grid->tile_num_y;
    // check for underflow?
    if (x > width) {
        log_error("Error: x = %d exceeds scene width: %d\n", x, width);
        return ERR_ARGS;
    }
    if (y > height) {
        log_error("Error: y = %d exceeds scene height: %d\n", y, height);
        return ERR_ARGS;
    }
    grid->tiles[x][y] = asset.img;

    return 0;
}
