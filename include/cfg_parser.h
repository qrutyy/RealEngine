#include "entity.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entity.h"

#define MAX_ASSETS 10
#define MAX_ENTITIES 10
#define MAX_LINE_LENGTH 256
#define MAX_MAP_WIDTH 50
#define MAX_MAP_HEIGHT 50
#define MAX_MAP_LAYERS 5

typedef struct asset_cfg {
    int id;
    char shortcut;
    char name[MAX_LINE_LENGTH];
    char filename[MAX_LINE_LENGTH];
    int pos_x, pos_y;
    int dim_w, dim_h;

    int engine_asset_key;
} asset_cfg_t;

typedef struct entity_cfg {
    int id;
    char shortcut;
	enum e_behaviour type; 
    int asset_id;
} entity_cfg_t;

typedef struct map_layout_cfg {
    int grid_width, grid_height;
    int tile_width, tile_height;

    asset_cfg_t assets[MAX_ASSETS];
    int asset_count;

    entity_cfg_t entities[MAX_ENTITIES];
    int entity_count;

    char layout[MAX_MAP_LAYERS][MAX_MAP_HEIGHT][MAX_MAP_WIDTH + 1];
	int layer_count;
} map_layout_cfg_t;

void parse_config(const char *filename, map_layout_cfg_t *config);
