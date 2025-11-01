#include "unity.h"
#include "scene.h"
#include "asset.h"
#include "cfg_parser.h"
#include "entity.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int RE_load_asset(char *filename, int src_x, int src_y, int width, int height) {
    return 0; 
}

int RE_add_entity(int x, int y, enum e_behaviour beh) {
    return 0; 
}

int RE_init_grid(grid_t *grid, int width, int height, int tile_w, int tile_h, int pad_y) {
    if (!grid) return -1;
    grid->tile_num_x = width;
    grid->tile_num_y = height;
    grid->tile_width = tile_w;
    grid->tile_height = tile_h;
    grid->pad_y = pad_y;
    return 0;
}

int RE_assign_asset_static(grid_t *grid, int id, int layer, int x, int y) {
    if (!grid) return -1;
    if (x >= grid->tile_num_x || y >= grid->tile_num_y) return -1;
    return 0; 
}

void setUp(void) {
    
}

void tearDown(void) {
    
}

void test_find_asset_by_shortcut_found(void) {
    map_layout_cfg_t config = {0};
    
    
    config.assets[0].shortcut = 'T';
    config.assets[0].engine_asset_key = 1;
    config.assets[1].shortcut = 'G';
    config.assets[1].engine_asset_key = 2;
    config.asset_count = 2;
    
    asset_cfg_t* result = find_asset_by_shortcut(&config, 'T');
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL('T', result->shortcut);
    TEST_ASSERT_EQUAL(1, result->engine_asset_key);
    
    result = find_asset_by_shortcut(&config, 'G');
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL('G', result->shortcut);
}

void test_find_asset_by_shortcut_not_found(void) {
    map_layout_cfg_t config = {0};
    
    
    config.assets[0].shortcut = 'T';
    config.assets[0].engine_asset_key = 1;
    config.asset_count = 1;
    
    asset_cfg_t* result = find_asset_by_shortcut(&config, 'X');
    TEST_ASSERT_NULL(result);
    
    result = find_asset_by_shortcut(&config, '\0');
    TEST_ASSERT_NULL(result);
}

void test_find_asset_by_shortcut_empty_config(void) {
    map_layout_cfg_t config = {0};
    config.asset_count = 0;
    
    asset_cfg_t* result = find_asset_by_shortcut(&config, 'T');
    TEST_ASSERT_NULL(result);
}

void test_get_entity_with_type_found(void) {
    map_layout_cfg_t config = {0};
    
    
    config.entities[0].shortcut = 'P';
    config.entities[0].type = PLAYER;
    config.entities[1].shortcut = 'E';
    config.entities[1].type = NPC;
    config.entity_count = 2;
    
    entity_cfg_t* result = get_entity_with_type(&config, 'P');
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL('P', result->shortcut);
    TEST_ASSERT_EQUAL(PLAYER, result->type);
    
    result = get_entity_with_type(&config, 'E');
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL('E', result->shortcut);
    TEST_ASSERT_EQUAL(NPC, result->type);
}

void test_get_entity_with_type_not_found(void) {
    map_layout_cfg_t config = {0};
    
    config.entities[0].shortcut = 'P';
    config.entities[0].type = PLAYER;
    config.entity_count = 1;
    
    entity_cfg_t* result = get_entity_with_type(&config, 'X');
    TEST_ASSERT_NULL(result);
    
    result = get_entity_with_type(&config, '\0');
    TEST_ASSERT_NULL(result);
}

void test_get_entity_with_type_empty_config(void) {
    map_layout_cfg_t config = {0};
    config.entity_count = 0;
    
    entity_cfg_t* result = get_entity_with_type(&config, 'P');
    TEST_ASSERT_NULL(result);
}

void test_assign_layers_basic(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    
    config.grid_width = 3;
    config.grid_height = 3;
    config.layer_count = 2;
    config.entity_count = 1;
    
    
    config.assets[0].shortcut = 'T';
    config.assets[0].engine_asset_key = 1;
    config.asset_count = 1;
    
    
    config.entities[0].shortcut = 'P';
    config.entities[0].type = PLAYER;
    config.entity_count = 1;
    
    
    config.layout[0] = malloc(sizeof(char*) * 3);
    for (int y = 0; y < 3; y++) {
        config.layout[0][y] = malloc(sizeof(char) * 4); 
        strcpy(config.layout[0][y], "T--");
    }
    config.layout[0][1][1] = 'T'; 
    
    
    config.layout[1] = malloc(sizeof(char*) * 3);
    for (int y = 0; y < 3; y++) {
        config.layout[1][y] = malloc(sizeof(char) * 4);
        strcpy(config.layout[1][y], "---");
    }
    config.layout[1][1][1] = 'P'; 
    
    
    RE_init_grid(&grid, 3, 3, 32, 32, 2);
    
    assign_layers(&grid, &config);
    
    
    TEST_ASSERT_TRUE(1);
    
    
    for (int y = 0; y < 3; y++) {
        free(config.layout[0][y]);
        free(config.layout[1][y]);
    }
    free(config.layout[0]);
    free(config.layout[1]);
}

void test_assign_layers_null_terminator_handling(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    config.grid_width = 2;
    config.grid_height = 2;
    config.layer_count = 1;
    config.asset_count = 0;
    config.entity_count = 0;
    
    
    config.layout[0] = malloc(sizeof(char*) * 2);
    for (int y = 0; y < 2; y++) {
        config.layout[0][y] = malloc(sizeof(char) * 3);
        strcpy(config.layout[0][y], "\0-");
    }
    
    RE_init_grid(&grid, 2, 2, 32, 32, 2);
    assign_layers(&grid, &config);
    
    
    TEST_ASSERT_TRUE(1);
    
    
    for (int y = 0; y < 2; y++) {
        free(config.layout[0][y]);
    }
    free(config.layout[0]);
}

void test_assign_layers_entity_count_mismatch(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    config.grid_width = 2;
    config.grid_height = 2;
    config.layer_count = 2;
    
    
    config.entity_count = 2;
    config.entities[0].shortcut = 'P';
    config.entities[0].type = PLAYER;
    config.entities[1].shortcut = 'E';
    config.entities[1].type = NPC;
    
    
    config.layout[0] = malloc(sizeof(char*) * 2);
    for (int y = 0; y < 2; y++) {
        config.layout[0][y] = malloc(sizeof(char) * 3);
        strcpy(config.layout[0][y], "--");
    }
    
    
    config.layout[1] = malloc(sizeof(char*) * 2);
    for (int y = 0; y < 2; y++) {
        config.layout[1][y] = malloc(sizeof(char) * 3);
        strcpy(config.layout[1][y], "--");
    }
    config.layout[1][0][0] = 'P'; 
    
    RE_init_grid(&grid, 2, 2, 32, 32, 2);
    assign_layers(&grid, &config);
    
    
    TEST_ASSERT_TRUE(1);
    
    
    for (int y = 0; y < 2; y++) {
        free(config.layout[0][y]);
        free(config.layout[1][y]);
    }
    free(config.layout[0]);
    free(config.layout[1]);
}

void test_assign_layers_dash_entity_handling(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    config.grid_width = 2;
    config.grid_height = 2;
    config.layer_count = 2;
    config.entity_count = 1;
    config.entities[0].shortcut = 'P';
    config.entities[0].type = PLAYER;
    
    
    config.layout[0] = malloc(sizeof(char*) * 2);
    for (int y = 0; y < 2; y++) {
        config.layout[0][y] = malloc(sizeof(char) * 3);
        strcpy(config.layout[0][y], "--");
    }
    
    config.layout[1] = malloc(sizeof(char*) * 2);
    for (int y = 0; y < 2; y++) {
        config.layout[1][y] = malloc(sizeof(char) * 3);
        strcpy(config.layout[1][y], "-P");
    }
    
    RE_init_grid(&grid, 2, 2, 32, 32, 2);
    assign_layers(&grid, &config);
    
    
    TEST_ASSERT_TRUE(1);
    
    
    for (int y = 0; y < 2; y++) {
        free(config.layout[0][y]);
        free(config.layout[1][y]);
    }
    free(config.layout[0]);
    free(config.layout[1]);
}

void test_load_cfg_basic(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    
    config.grid_width = 5;
    config.grid_height = 5;
    config.tile_width = 32;
    config.tile_height = 32;
    config.pad_y = 2;
    config.layer_count = 1;
    
    
    config.asset_count = 1;
    config.assets[0].filename = "test.png";
    config.assets[0].pos_x = 0;
    config.assets[0].pos_y = 0;
    config.assets[0].dim_w = 32;
    config.assets[0].dim_h = 32;
    config.assets[0].shortcut = 'T';
    
    
    config.layout[0] = malloc(sizeof(char*) * 5);
    for (int y = 0; y < 5; y++) {
        config.layout[0][y] = malloc(sizeof(char) * 6);
        strcpy(config.layout[0][y], "-----");
    }
    
    int result = load_cfg(&grid, &config);
    
    
    TEST_ASSERT_EQUAL(5, grid.tile_num_x);
    TEST_ASSERT_EQUAL(5, grid.tile_num_y);
    TEST_ASSERT_EQUAL(32, grid.tile_width);
    TEST_ASSERT_EQUAL(32, grid.tile_height);
    TEST_ASSERT_EQUAL(2, grid.pad_y);
    
    
    for (int y = 0; y < 5; y++) {
        free(config.layout[0][y]);
    }
    free(config.layout[0]);
}

void test_load_cfg_multiple_assets(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    config.grid_width = 3;
    config.grid_height = 3;
    config.tile_width = 32;
    config.tile_height = 32;
    config.pad_y = 2;
    config.layer_count = 1;
    
    
    config.asset_count = 3;
    config.assets[0].filename = "grass.png";
    config.assets[0].pos_x = 0;
    config.assets[0].pos_y = 0;
    config.assets[0].dim_w = 32;
    config.assets[0].dim_h = 32;
    config.assets[0].shortcut = 'G';
    
    config.assets[1].filename = "tree.png";
    config.assets[1].pos_x = 32;
    config.assets[1].pos_y = 0;
    config.assets[1].dim_w = 32;
    config.assets[1].dim_h = 64;
    config.assets[1].shortcut = 'T';
    
    config.assets[2].filename = "water.png";
    config.assets[2].pos_x = 64;
    config.assets[2].pos_y = 0;
    config.assets[2].dim_w = 32;
    config.assets[2].dim_h = 32;
    config.assets[2].shortcut = 'W';
    
    
    config.layout[0] = malloc(sizeof(char*) * 3);
    for (int y = 0; y < 3; y++) {
        config.layout[0][y] = malloc(sizeof(char) * 4);
        strcpy(config.layout[0][y], "---");
    }
    
    int result = load_cfg(&grid, &config);
    
    
    TEST_ASSERT_EQUAL(3, grid.tile_num_x);
    TEST_ASSERT_EQUAL(3, grid.tile_num_y);
    
    
    for (int y = 0; y < 3; y++) {
        free(config.layout[0][y]);
    }
    free(config.layout[0]);
}

void test_load_cfg_null_inputs(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    
    int result = load_cfg(NULL, &config);
    
    
    result = load_cfg(&grid, NULL);
    
}

void test_assign_layers_null_inputs(void) {
    map_layout_cfg_t config = {0};
    grid_t grid = {0};
    
    
    assign_layers(NULL, &config);
    
    
    assign_layers(&grid, NULL);
    
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_find_asset_by_shortcut_found);
    RUN_TEST(test_find_asset_by_shortcut_not_found);
    RUN_TEST(test_find_asset_by_shortcut_empty_config);
    RUN_TEST(test_get_entity_with_type_found);
    RUN_TEST(test_get_entity_with_type_not_found);
    RUN_TEST(test_get_entity_with_type_empty_config);
    RUN_TEST(test_assign_layers_basic);
    RUN_TEST(test_assign_layers_null_terminator_handling);
    RUN_TEST(test_assign_layers_entity_count_mismatch);
    RUN_TEST(test_assign_layers_dash_entity_handling);
    RUN_TEST(test_load_cfg_basic);
    RUN_TEST(test_load_cfg_multiple_assets);
    RUN_TEST(test_load_cfg_null_inputs);
    RUN_TEST(test_assign_layers_null_inputs);
    
    return UNITY_END();
}