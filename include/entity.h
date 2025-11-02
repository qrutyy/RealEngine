#ifndef ENTITY_H
#define ENTITY_H

#include "animation.h"
#include "asset.h"

#define MAX_ENTITIES_NUM 1000
#define MAX_ENTITIES_PER_LAYER 100

/*
    A number specifying entity's behaviour.
*/
enum e_behaviour {
    PLAYER = 0,
    NPC = 1,
    FOLLOW = 2,
    // these should be more precise, to be added
};

/*
    A structure representing an entity.
*/
typedef struct entity {
    int x;
    int y;
    int depth;
    int asset_id;
    enum e_behaviour beh;
    animation_t animations[MAX_ANIMATIONS_NUM];
    int animations_num;
    int curr_animation;
    asset_t *asset;
} entity_t;

/*
    A structure representing all entities in the layer i.
*/
typedef struct layer_entities {
    entity_t *entities;
    int num_entities;
} layer_entities_t;

int RE_add_entity(int x, int y, enum e_behaviour beh);
entity_t *get_entities(void);
int get_entities_num(void);
void reset_entities(void);

#endif // ENTITY_H
