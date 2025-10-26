#ifndef ASSET_H
#define ASSET_H

#include <SDL3/SDL.h>
#include "types.h"

#define MAX_ASSETS_NUM 1000

typedef struct asset {
    SDL_Surface *img;
    int width;
    int height;
} asset_t;

/*
    Load asset from memory and store it into map.

    @param filename a name of image inside 'assets' folder.
    @param src_x x of top-left pixel of asset inside image.
    @param src_y y of top-left pixel of asset inside image.
    @param width width of an asset.
    @param height height of an asset.

    @returns an id of loaded asset on success, -1 otherwise.
*/
int RE_load_asset(char *filename, int src_x, int src_y, int width, int height);

/*
    Get asset via id.

    @param id an id of loaded asset.

    @returns a pointer to asset if id is associated with some asset, NULL otherwise.
*/
asset_t *RE_get_asset(int id);

// works only for tiles?
/*
    Assign asset to a static object.

    @param grid a grid handle.
    @param if an id of loaded asset.
    @param layer TODO
    @param x x of a tile on which to add static object.
    @param y y of a tile on which to add static object.

    @returns 0 on success, error code otherwise.
*/
int RE_assign_asset_static(grid_t *grid, int id, int layer, int x, int y);

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


#endif // ASSET_H

