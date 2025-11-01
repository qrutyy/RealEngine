#include "entity.h"
#include "asset.h"
#include "errors.h"
#include "log.h"
#include <SDL3/SDL.h>

static entity_t entities[MAX_ENTITIES_NUM];

static int curr_entities_num;

int RE_add_entity(int x, int y, enum e_behaviour beh) {
	entity_t ent = {.x = x, .y = y, .depth = 1, .beh = beh};

	if (curr_entities_num >= MAX_ENTITIES_NUM) {
		log_error("Failed to add entity: limit %d is reached.\n", MAX_ENTITIES_NUM);
	}
	if (beh == PLAYER) {
		for (int i = 0; i < curr_entities_num; i++) {
			if (entities[i].beh == 0) {
				log_error("Cannot add more than one player.\n");
				return -1;
			}
		}
	}

	ent.asset_id = 0;
	int id = curr_entities_num++;
	entities[id] = ent;

	char *ent_name;
	if (beh == PLAYER)
		ent_name = "player";
	else
		ent_name = "unknown";
	// log_debug("Added entity with behaviour %s on (%d, %d) with id %d\n", ent_name, x, y, id);

	return id;
}

entity_t *get_entities(void) { return (entity_t *)&entities; }

int get_entities_num(void) { return curr_entities_num; }
