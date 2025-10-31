#include "asset.h"
#include "entity.h"
#include "include.h"
#include "scene.h"
#include "log.h"
#include "cfg_parser.h"
#include <mach/exception_types.h>

asset_cfg_t *find_asset_by_shortcut(map_layout_cfg_t *config, const char shortcut) {
	for (int i = 0; i < MAX_ASSETS; i ++) {
		if (config->assets[i].shortcut == shortcut)
			return &config->assets[i];
	}
	return NULL;
}

void assign_layers(grid_t *grid, map_layout_cfg_t *config) {
	int parsed_entity_count = 0;
	asset_cfg_t *asset;
	entity_cfg_t *curr_entity_cfg;

	for (int x = 0; x < MAX_MAP_WIDTH; x ++) {
		for (int y = 0; y < MAX_MAP_HEIGHT; y ++) {
			asset = find_asset_by_shortcut(config, config->layout[0][y][x]);
			RE_assign_asset_static(grid, asset->engine_asset_key, 0, asset->pos_x, asset->pos_y);	
		}
	}

	for (int i = 0; i < config->layer_count; i ++) {
		for (int x = 0; x < MAX_MAP_WIDTH; x ++) {
			for (int y = 0; y < MAX_MAP_HEIGHT; y ++) {
				if (config->layout[i][y][x] != '-') {
					curr_entity_cfg = &config->entities[i];
					RE_add_entity(x, y, curr_entity_cfg->type);
					parsed_entity_count++;
				}
			}
		}
	}

	if (config->entity_count != parsed_entity_count) {
		log_error("were fucked up, by 'we' i mean my parser\n");
	}
	
}

int load_cfg(grid_t *grid, map_layout_cfg_t *config) {
	int i;
	asset_cfg_t *curr_asset_cfg;

	RE_init_grid(grid, config->grid_width, config->grid_height, config->tile_width, config->tile_height);

	for (i = 0; i < config->asset_count; i ++) {
		curr_asset_cfg = &config->assets[i];
		curr_asset_cfg->engine_asset_key = RE_load_asset(curr_asset_cfg->filename, curr_asset_cfg->pos_x, curr_asset_cfg->pos_y, curr_asset_cfg->dim_w, curr_asset_cfg->dim_h);
	}

	assign_layers(grid, config);
//	assign_animations(); TODO: in parser as well
}


