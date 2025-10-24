#ifndef ENTITY_H
#define ENTITY_H

#define MAX_ENTITIES_NUM 1000

/*
    A number specifying entity's behaviour.
*/
enum e_behaviour {
    PLAYER = 0,
    NPC = 1
    // these should be more precise, to be added
};

/*
    A structure representing an entity.
*/
typedef struct entity {
    int x;
    int y;
    int depth;
    enum e_behaviour beh;
} entity_t;

int RE_add_entity(int x, int y, enum e_behaviour beh);
entity_t *get_entities(void);
int get_entities_num(void);

#endif // ENTITY_H