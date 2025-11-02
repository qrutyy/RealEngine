#ifndef ANIMATION_H
#define ANIMATION_H

#include "asset.h"

#define MAX_ANIMATIONS_NUM 1000

/*
    A structure representing animations of an entity.
*/
typedef struct animation {
	asset_t *assets;
	int assets_num;
	int curr_asset;
} animation_t;

/*
    Create an animation of a sequence of assets.

    @param assets_ids an array of asset ids returned by RE_load_asset.
    @param assets_num a positive number specifying assets_ids size.

    @returns an id of created animation on success, -1 otherwise.
*/
int RE_create_animation(int *assets_ids, int assets_num);

/*
    TODO a way to be able to choose different animations.
    For now, only first four animations can be used.
    0 is south, 1 - west, 2 - north, 3 - east.

    Add an animation to an entity.

    @param animation_id an animation id returned by RE_create_animation.
    @param entity_id an entity id returned by RE_add_entity.

    @returns 0 on success, errno otherwise.
*/
int RE_add_animation(int animation_id, int entity_id);

#endif // ANIMATION_H