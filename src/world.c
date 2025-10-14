#include "world.h"
#include "err.h"
#include "log.h"
#include <stdlib.h>

world_t *world_create() {
	world_t *world = calloc(1, sizeof(struct world));
	if (!world) {
		log_error("Failed to create world\n");
		return NULL;
	}
	world->cam_zoom = 1.0f;
	return world;
}
