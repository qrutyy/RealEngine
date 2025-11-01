#include "unity.h"
#include "asset.h"
#include "errors.h"
#include "log.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External declarations for the module under test
extern asset_t assets[MAX_ASSETS_NUM];
extern int curr_assets_num;

// Mock SDL functions
SDL_Surface* SDL_LoadPNG(const char* file) {
    // Return a mock surface for testing
    static SDL_Surface mock_surface = {.w = 64, .h = 64};
    return &mock_surface;
}

SDL_Surface* SDL_CreateSurface(int width, int height, SDL_PixelFormat* format) {
    static SDL_Surface mock_surface;
    mock_surface.w = width;
    mock_surface.h = height;
    return &mock_surface;
}

int SDL_BlitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect) {
    return 0; // Success
}

void SDL_DestroySurface(SDL_Surface* surface) {
    // Do nothing for tests
}

void setUp(void) {
    // Reset the asset system before each test
    curr_assets_num = 0;
    memset(assets, 0, sizeof(assets));
}

void tearDown(void) {
    // Clean up after each test
}

void test_RE_load_asset_basic(void) {
    int id = RE_load_asset("test.png", 0, 0, 32, 32);
    
    TEST_ASSERT_EQUAL(0, id);
    TEST_ASSERT_EQUAL(1, curr_assets_num);
    TEST_ASSERT_NOT_NULL(assets[0].img);
    TEST_ASSERT_EQUAL(32, assets[0].width);
    TEST_ASSERT_EQUAL(32, assets[0].height);
}

void test_RE_load_asset_multiple_assets(void) {
    int id1 = RE_load_asset("test1.png", 0, 0, 32, 32);
    int id2 = RE_load_asset("test2.png", 32, 0, 64, 64);
    int id3 = RE_load_asset("test3.png", 0, 32, 16, 16);
    
    TEST_ASSERT_EQUAL(0, id1);
    TEST_ASSERT_EQUAL(1, id2);
    TEST_ASSERT_EQUAL(2, id3);
    TEST_ASSERT_EQUAL(3, curr_assets_num);
    
    TEST_ASSERT_EQUAL(32, assets[0].width);
    TEST_ASSERT_EQUAL(64, assets[1].width);
    TEST_ASSERT_EQUAL(16, assets[2].width);
}

void test_RE_load_asset_max_limit(void) {
    // Fill up to maximum
    for (int i = 0; i < MAX_ASSETS_NUM; i++) {
        int id = RE_load_asset("test.png", 0, 0, 32, 32);
        TEST_ASSERT_EQUAL(i, id);
    }
    
    TEST_ASSERT_EQUAL(MAX_ASSETS_NUM, curr_assets_num);
    
    // Try to add one more (should fail)
    int result = RE_load_asset("overflow.png", 0, 0, 32, 32);
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(MAX_ASSETS_NUM, curr_assets_num);
}

void test_RE_get_asset_valid_id(void) {
    RE_load_asset("test.png", 0, 0, 32, 32);
    RE_load_asset("test2.png", 0, 0, 64, 64);
    
    asset_t* asset1 = RE_get_asset(0);
    asset_t* asset2 = RE_get_asset(1);
    
    TEST_ASSERT_NOT_NULL(asset1);
    TEST_ASSERT_NOT_NULL(asset2);
    TEST_ASSERT_EQUAL(32, asset1->width);
    TEST_ASSERT_EQUAL(64, asset2->width);
    TEST_ASSERT_EQUAL_PTR(&assets[0], asset1);
    TEST_ASSERT_EQUAL_PTR(&assets[1], asset2);
}

void test_RE_get_asset_invalid_id(void) {
    RE_load_asset("test.png", 0, 0, 32, 32);
    
    // Test with ID beyond current count
    asset_t* asset = RE_get_asset(5);
    TEST_ASSERT_NULL(asset);
    
    // Test with ID beyond maximum
    asset = RE_get_asset(MAX_ASSETS_NUM + 1);
    TEST_ASSERT_NULL(asset);
}

void test_RE_get_asset_empty(void) {
    // Test getting asset when no assets are loaded
    asset_t* asset = RE_get_asset(0);
    TEST_ASSERT_NULL(asset);
}

void test_RE_assign_asset_static_valid(void) {
    // Create a test grid
    grid_t grid = {0};
    grid.tile_num_x = 10;
    grid.tile_num_y = 10;
    grid.tiles = malloc(sizeof(SDL_Surface**) * 10);
    for (int i = 0; i < 10; i++) {
        grid.tiles[i] = malloc(sizeof(SDL_Surface*) * 10);
        for (int j = 0; j < 10; j++) {
            grid.tiles[i][j] = NULL;
        }
    }
    
    // Load an asset and assign it
    int asset_id = RE_load_asset("test.png", 0, 0, 32, 32);
    TEST_ASSERT_EQUAL(0, asset_id);
    
    int result = RE_assign_asset_static(&grid, 0, 1, 5, 5);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_NOT_NULL(grid.tiles[5][5]);
    
    // Cleanup
    for (int i = 0; i < 10; i++) {
        free(grid.tiles[i]);
    }
    free(grid.tiles);
}

void test_RE_assign_asset_static_invalid_asset_id(void) {
    grid_t grid = {0};
    grid.tile_num_x = 10;
    grid.tile_num_y = 10;
    
    // Try to assign non-existent asset
    int result = RE_assign_asset_static(&grid, 5, 1, 5, 5);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
}

void test_RE_assign_asset_static_out_of_bounds_x(void) {
    grid_t grid = {0};
    grid.tile_num_x = 10;
    grid.tile_num_y = 10;
    
    RE_load_asset("test.png", 0, 0, 32, 32);
    
    int result = RE_assign_asset_static(&grid, 0, 1, 15, 5);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
}

void test_RE_assign_asset_static_out_of_bounds_y(void) {
    grid_t grid = {0};
    grid.tile_num_x = 10;
    grid.tile_num_y = 10;
    
    RE_load_asset("test.png", 0, 0, 32, 32);
    
    int result = RE_assign_asset_static(&grid, 0, 1, 5, 15);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
}

void test_RE_assign_asset_static_boundary_conditions(void) {
    grid_t grid = {0};
    grid.tile_num_x = 10;
    grid.tile_num_y = 10;
    grid.tiles = malloc(sizeof(SDL_Surface**) * 10);
    for (int i = 0; i < 10; i++) {
        grid.tiles[i] = malloc(sizeof(SDL_Surface*) * 10);
        for (int j = 0; j < 10; j++) {
            grid.tiles[i][j] = NULL;
        }
    }
    
    RE_load_asset("test.png", 0, 0, 32, 32);
    
    // Test minimum bounds
    int result = RE_assign_asset_static(&grid, 0, 1, 0, 0);
    TEST_ASSERT_EQUAL(0, result);
    
    // Test maximum bounds
    result = RE_assign_asset_static(&grid, 0, 1, 9, 9);
    TEST_ASSERT_EQUAL(0, result);
    
    // Cleanup
    for (int i = 0; i < 10; i++) {
        free(grid.tiles[i]);
    }
    free(grid.tiles);
}

void test_RE_assign_asset_static_null_grid(void) {
    RE_load_asset("test.png", 0, 0, 32, 32);
    
    int result = RE_assign_asset_static(NULL, 0, 1, 5, 5);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
}

void test_RE_load_asset_different_sizes(void) {
    int id1 = RE_load_asset("small.png", 0, 0, 16, 16);
    int id2 = RE_load_asset("medium.png", 0, 0, 32, 32);
    int id3 = RE_load_asset("large.png", 0, 0, 64, 64);
    
    TEST_ASSERT_EQUAL(0, id1);
    TEST_ASSERT_EQUAL(1, id2);
    TEST_ASSERT_EQUAL(2, id3);
    
    TEST_ASSERT_EQUAL(16, assets[0].width);
    TEST_ASSERT_EQUAL(16, assets[0].height);
    TEST_ASSERT_EQUAL(32, assets[1].width);
    TEST_ASSERT_EQUAL(32, assets[1].height);
    TEST_ASSERT_EQUAL(64, assets[2].width);
    TEST_ASSERT_EQUAL(64, assets[2].height);
}

void test_RE_load_asset_src_coordinates(void) {
    // Test loading from different source coordinates
    int id = RE_load_asset("spritesheet.png", 32, 64, 32, 32);
    
    TEST_ASSERT_EQUAL(0, id);
    TEST_ASSERT_EQUAL(32, assets[0].width);
    TEST_ASSERT_EQUAL(32, assets[0].height);
}

void test_asset_structure_initialization(void) {
    int id = RE_load_asset("test.png", 0, 0, 32, 32);
    
    TEST_ASSERT_EQUAL(0, id);
    TEST_ASSERT_NOT_NULL(assets[0].img);
    TEST_ASSERT_EQUAL(32, assets[0].width);
    TEST_ASSERT_EQUAL(32, assets[0].height);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_RE_load_asset_basic);
    RUN_TEST(test_RE_load_asset_multiple_assets);
    RUN_TEST(test_RE_load_asset_max_limit);
    RUN_TEST(test_RE_get_asset_valid_id);
    RUN_TEST(test_RE_get_asset_invalid_id);
    RUN_TEST(test_RE_get_asset_empty);
    RUN_TEST(test_RE_assign_asset_static_valid);
    RUN_TEST(test_RE_assign_asset_static_invalid_asset_id);
    RUN_TEST(test_RE_assign_asset_static_out_of_bounds_x);
    RUN_TEST(test_RE_assign_asset_static_out_of_bounds_y);
    RUN_TEST(test_RE_assign_asset_static_boundary_conditions);
    RUN_TEST(test_RE_assign_asset_static_null_grid);
    RUN_TEST(test_RE_load_asset_different_sizes);
    RUN_TEST(test_RE_load_asset_src_coordinates);
    RUN_TEST(test_asset_structure_initialization);
    
    return UNITY_END();
}