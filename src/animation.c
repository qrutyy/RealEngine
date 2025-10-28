#include "asset.h"
#include "log.h"
#include "errors.h"
#include "entity.h"
#include "animation.h"
#include <stdlib.h>
#include <SDL3/SDL.h>

static animation_t animations[MAX_ANIMATIONS_NUM];

static int curr_animations_num;

int RE_create_animation(int *assets_ids, int assets_num) {
    if (assets_num <= 0) {
        log_debug("Assets_num cannot be negative.");
        return -1;
    }
    asset_t *assets = malloc(sizeof(asset_t) * assets_num);
    if (!assets) {
        log_debug("Failed to allocate assets for animation.");
        return -1;
    }

    for (int i = 0; i < assets_num; i++) {
        asset_t *asset = RE_get_asset(assets_ids[i]);
        assets[i] = *asset;
    }

    int index = curr_animations_num++;
    animation_t anim = {
        .assets = assets,
        .assets_num = assets_num,
        .curr_asset = 0,
    };
    animations[index] = anim;

    return index;
}

int RE_add_animation(int animation_id, int entity_id) {
    if (animation_id < 0) {
        log_debug("Cannot add animation with negative id: %d", animation_id);
        return ERR_ARGS;
    }
    if (entity_id < 0) {
        log_debug("Cannot add to entity with negative id: %d", entity_id);
        return ERR_ARGS;
    }

    entity_t *entities = get_entities();

    // printf("anim num: %d\n", anim_num);

    int anim_num = (&entities[entity_id])->animations_num++;
    (&entities[entity_id])->animations[anim_num] = animations[animation_id];

    log_debug("add animation %d to entity %d\n", animation_id, entity_id);

    return 0;
}
