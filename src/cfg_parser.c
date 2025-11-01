#include "cfg_parser.h"
#include "entity.h"
#include <ctype.h>

static void trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

static enum e_behaviour get_entity_type_from_str(const char* str_name) {
	if (!strcmp(str_name, "PLAYER")) return PLAYER;
	if (!strcmp(str_name, "NPC")) return NPC;
	if (!strcmp(str_name, "FOLLOW")) return FOLLOW;
	return -1;
}

static void parser_dbg(map_layout_cfg_t *config) {
	printf("--- Настройки карты ---\n");
    printf("  Размер сетки: %d x %d\n", config->grid_width, config->grid_height);
    printf("  Размер тайла: %d x %d\n", config->tile_width, config->tile_height);

    printf("\n--- Ассеты (%d найдено) ---\n", config->asset_count);
    for (int i = 0; i < config->asset_count; i++) {
		asset_cfg_t *a = &config->assets[i];
        printf("  Ассет %d:\n", a->id);
        printf("    shortcut: '%c'\n", a->shortcut);
        printf("    name:     %s\n", a->name);
        printf("    filename: %s\n", a->filename);
        printf("    pos:      %d,%d\n", a->pos_x, a->pos_y);
        printf("    dim:      %d,%d\n", a->dim_w, a->dim_h);
    }
    
    printf("\n--- Типы сущностей (%d найдено) ---\n", config->entity_count);
    for (int i = 0; i < config->entity_count; i++) {
        entity_cfg_t *e = &config->entities[i];
        printf("  Сущность %d:\n", e->id);
        printf("    shortcut: '%c'\n", e->shortcut);
        printf("    type:     %d\n", e->type);
        printf("    asset_id: %d\n", e->asset_id);
    }
    
    printf("\n--- Слои карты (%d найдено) ---\n", config->layer_count);
    for (int l = 0; l < config->layer_count; l++) {
        printf("  Слой %d:\n", l);
        // Выведем первые 5 строк для проверки
        for (int y = 0; y < 5 && y < config->grid_height; y++) {
            printf("    %s\n", config->layout[l][y]);
        }
        if (config->grid_height > 5) printf("    ...\n");
    }

}

void parse_config(const char *filename, map_layout_cfg_t *config) {
	FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка: не удалось открыть файл конфигурации");
        return;
    }

    char line[MAX_LINE_LENGTH];
    char current_section[50] = "";
    int current_layer_index = -1;
    int layout_row = 0;

    memset(config, 0, sizeof(map_layout_cfg_t));

    while (fgets(line, sizeof(line), file)) {
        trim(line);
		// skip comments
        if (line[0] == ';' || line[0] == '\0') {
            continue;
        }

        int layer_index;
        if (sscanf(line, "[Map.Layout.Layer.%d]", &layer_index) == 1) {
            strcpy(current_section, "Map.Layout.Layer");
            current_layer_index = layer_index;
            layout_row = 0; 
            
            if (current_layer_index >= config->layer_count) {
                config->layer_count = current_layer_index + 1;
            }
            continue;
        }
        
        if (line[0] == '[') {
            sscanf(line, "[%[^]]]", current_section);
            current_layer_index = -1; 
            continue;
        }

        if (strcmp(current_section, "Map.Layout.Layer") == 0 && current_layer_index != -1) {
            if (config->grid_width == 0) {
                fprintf(stderr, "Внимание: grid.width/height должны быть определены перед [Map.Layout]\n");
            }
            if (layout_row < MAX_MAP_HEIGHT && current_layer_index < MAX_MAP_LAYERS) {
                strncpy(config->layout[current_layer_index][layout_row], line, config->grid_width);
                config->layout[current_layer_index][layout_row][config->grid_width] = '\0';
                layout_row++;
            }
            continue;
        }
        
        char *key = strtok(line, "=");
        char *value = strtok(NULL, "\n");
        if (!key || !value) continue;

        if (strcmp(current_section, "Assets") == 0) {
            int id;
            char prop[50];
            if (sscanf(key, "asset.%d.%s", &id, prop) == 2) {
                id--;
                if (id >= 0 && id < MAX_ASSETS) {
                    config->assets[id].id = id + 1;
                    if (strcmp(prop, "shortcut") == 0) config->assets[id].shortcut = value[0];
                    else if (strcmp(prop, "name") == 0) strncpy(config->assets[id].name, value, sizeof(config->assets[id].name) - 1);
                    else if (strcmp(prop, "filename") == 0) strncpy(config->assets[id].filename, value, sizeof(config->assets[id].filename) - 1);
                    else if (strcmp(prop, "pos") == 0) sscanf(value, "%d,%d", &config->assets[id].pos_x, &config->assets[id].pos_y);
                    else if (strcmp(prop, "dim") == 0) sscanf(value, "%d,%d", &config->assets[id].dim_w, &config->assets[id].dim_h);
                    
                    if (id >= config->asset_count) config->asset_count = id + 1;
                }
            }
        } else if (strcmp(current_section, "Map") == 0) {
            if (strcmp(key, "grid.dim") == 0) sscanf(value, "%d,%d", &config->grid_width, &config->grid_height);
            else if (strcmp(key, "tile.dim") == 0) sscanf(value, "%d,%d", &config->tile_width, &config->tile_height);
        } else if (strcmp(current_section, "Entities") == 0) {
            int id;
            char prop[50];
            if (sscanf(key, "entity.%d.%s", &id, prop) == 2) {
                id--;
                if (id >= 0 && id < MAX_ENTITIES) {
                    config->entities[id].id = id + 1;
                    if (strcmp(prop, "shortcut") == 0) config->entities[id].shortcut = value[0];
                    else if (strcmp(prop, "type") == 0) {
						enum e_behaviour e_type = get_entity_type_from_str(value);
						config->entities[id].type = e_type;
					}
                    else if (strcmp(prop, "asset") == 0) config->entities[id].asset_id = (value[0] == '?') ? -1 : atoi(value);

                    if (id >= config->entity_count) config->entity_count = id + 1;
                }
            }
        }
    }
    fclose(file);
	parser_dbg(config);
}
