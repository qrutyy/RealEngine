#ifndef WORLD_H
#define WORLD_H

#include "types.h"

// to add
typedef struct world {
    m_vec3_t pl_pos;
	m_vec3_t cam_pos;
    float cam_zoom;
} world_t;

world_t* world_create(void);
void world_destroy(world_t* world);

#endif
