#include <SDL3/SDL.h>
#include "entity.h"
#include "log.h"
#include "errors.h"

#define MAX_ENTITIES_NUM 1000

typedef struct entity {
    int x;
    int y;
    int depth;
    enum e_behaviour beh;
} entity_t;

static entity_t entities[MAX_ENTITIES_NUM];

static int curr_entities_num;

int RE_add_entity(int x, int y, enum e_behaviour beh) {
    entity_t ent = {.x=x, .y=y, .depth=1, .beh=beh};

    if (curr_entities_num >= MAX_ENTITIES_NUM) {
        log_error("Failed to add entity: limit %d is reached.\n", MAX_ENTITIES_NUM);
    }
    if (beh == PLAYER) {
        for (int i = 0; i < curr_entities_num; i++) {
            if (entities[i].beh == 0) {
                log_error("Cannot add more than one player.\n");
                return ERR_ARGS;
            }
        }
    }
    entities[curr_entities_num++] = ent;

    log_debug("Added entity with behaviour %d on (%d, %d)\n", beh, x, y);

    return 0;
}
