#ifndef ASSET_H
#define ASSET_H

#include <SDL3/SDL.h>
#include "types.h"

/*
    Load asset from memory and store it into map.

    @param filename a name of image inside 'assets' folder.
    @param src_x x of top-left pixel of asset inside image.
    @param src_y y of top-left pixel of asset inside image.
    @param width width of an asset.
    @param height height of an asset.

    @returns a key to asset on success, NULL on error.
*/
char *RE_load_asset(char *filename, int src_x, int src_y, int width, int height);

/*
    Get asset via associated key.

    @param key a key returned by `RE_load_asset`.

    @returns a pointer to asset if key is associated with some asset, NULL otherwise.
*/
SDL_Surface *RE_get_asset(char *key);

// works only for tiles?
/*
    Assign asset to a static object.

    @param grid a grid handle.
    @param key a key associated with some asset.
    @param layer TODO
    @param x x of a tile on which to add static object.
    @param y y of a tile on which to add static object.

    @returns 0 on success, error code otherwise.
*/
int RE_assign_asset_static(grid_t *grid, char *key, int layer, int x, int y);

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