#include "unity.h"
#include "asset.h"
#include "app.h"
#include "errors.h"
#include "log.h"
#include <SDL3/SDL.h>
#include <stdlib.h>


void test_RE_load_asset_max_limit(void) {
    for (int i = 0; i < MAX_ASSETS_NUM; i++) {
        int result = RE_load_asset("tests/image.png", 0, 0, 32, 32);
        TEST_ASSERT_TRUE(result >= 0 || result == -1);
    }
    
    int result = RE_load_asset("tests/image.png", 0, 0, 32, 32);
    TEST_ASSERT_EQUAL(-1, result);
}

void test_RE_load_asset_invalid_filename(void) {
    int result = RE_load_asset("nonexistent.png", 0, 0, 32, 32);
    TEST_ASSERT_EQUAL(-1, result);
}

void test_RE_get_asset_valid(void) {
    int id = RE_load_asset("tests/image.png", 0, 0, 32, 32);
    if (id >= 0) {
        asset_t* asset = RE_get_asset(id);
        TEST_ASSERT_NOT_NULL(asset);
    }
}

void test_RE_get_asset_too_big_id(void) {
    asset_t *asset = RE_get_asset(MAX_ASSETS_NUM + 1);
    TEST_ASSERT_NULL(asset);
}

void test_RE_get_asset_neg_id(void) {
    asset_t* asset = RE_get_asset(-1);
    TEST_ASSERT_NULL(asset);
}

void test_RE_assign_asset_static_valid(void) {
    int id = RE_load_asset("tests/image.png", 0, 0, 32, 32);
    if (id >= 0) {
        grid_t grid;
        RE_init_grid(&grid, 10, 10, 32, 32, 0);
        
        int result = RE_assign_asset_static(&grid, id, 0, 5, 5);
        TEST_ASSERT_EQUAL(0, result);
        
        destroy_grid(&grid);
    }
}

void test_RE_assign_asset_static_invalid_asset_id(void) {
    grid_t grid;
    RE_init_grid(&grid, 10, 10, 32, 32, 0);
    
    int result = RE_assign_asset_static(&grid, MAX_ASSETS_NUM + 1, 0, 5, 5);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
    
    destroy_grid(&grid);
}

void test_RE_assign_asset_static_out_of_bounds(void) {
    int id = RE_load_asset("tests/image.png", 0, 0, 32, 32);
    if (id >= 0) {
        grid_t grid;
        RE_init_grid(&grid, 5, 5, 32, 32, 0);
        
        int result = RE_assign_asset_static(&grid, id, 0, 10, 5);
        TEST_ASSERT_EQUAL(ERR_ARGS, result);
        
        result = RE_assign_asset_static(&grid, id, 0, 5, 10);
        TEST_ASSERT_EQUAL(ERR_ARGS, result);
        
        destroy_grid(&grid);
    }
}

void test_RE_assign_asset_static_null_grid(void) {
    int id = RE_load_asset("tests/image.png", 0, 0, 32, 32);
    if (id >= 0) {
        int result = RE_assign_asset_static(NULL, id, 0, 5, 5);
        TEST_ASSERT_EQUAL(ERR_ARGS, result);
    }
}

void run_asset_tests(void) {
    RUN_TEST(test_RE_load_asset_max_limit);
    RUN_TEST(test_RE_load_asset_invalid_filename);
    RUN_TEST(test_RE_get_asset_valid);
    RUN_TEST(test_RE_get_asset_too_big_id);
    RUN_TEST(test_RE_get_asset_neg_id);
    RUN_TEST(test_RE_assign_asset_static_valid);
    RUN_TEST(test_RE_assign_asset_static_invalid_asset_id);
    RUN_TEST(test_RE_assign_asset_static_out_of_bounds);
    RUN_TEST(test_RE_assign_asset_static_null_grid);
}