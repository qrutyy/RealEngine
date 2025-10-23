
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL3/SDL.h>
#define SHORT_SIZE 32

/*
    A number specifying entity's behaviour.

    PLAYER = 0
    NPC = 1
*/
enum e_behaviour {
    PLAYER = 0,
    NPC = 1
    // these should be more precise, to be added
};

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

/*
    Create a game entity.

    @param filename a filename of loaded asset.
    @param behaviour which behaviour entity follows.
    @param x x of a tile on which entity is created.
    @param y y of a tile on which enity is created.
    
    @return 0 on success, error code otherwise.
*/
int RE_create_entity(char *filename, enum e_behaviour behaviour, int x, int y);

// if filename is not loaded, return error
int RE_create_static(char *filename, int layer, int x, int y);

#endif // ASSET_MANAGER_H