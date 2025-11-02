#include "unity.h"
#include "errors.h"
#include "app.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_app_create_destroy(void) {
    app_hlpr_t* app = app_create();
    // TEST_ASSERT_NOT_NULL(app);
    TEST_ASSERT_FALSE(app->is_running); 
    
    app_destroy(app);
}

void test_cam_process_key_event(void) {
    app_hlpr_t app = {0};
    entity_t player = {0};
    SDL_KeyboardEvent event;
    
    
    app.cam.x = 5;
    app.cam.y = 5;
    
    
    event.key = SDLK_UP;
    cam_process_key_event(event, &app, &player, SDL_EVENT_KEY_DOWN);
    TEST_ASSERT_EQUAL(4, app.cam.x);
    TEST_ASSERT_EQUAL(4, app.cam.y);
    
    
    app.cam.x = 0;
    app.cam.y = 0;
    event.key = SDLK_LEFT;
    cam_process_key_event(event, &app, &player, SDL_EVENT_KEY_DOWN);
    TEST_ASSERT_EQUAL(0, app.cam.x); 
}

void test_act_entity_boundaries(void) {
    app_hlpr_t app = {0};
    entity_t ent = {0};
    
    
    app.grid.tile_num_x = 10;
    app.grid.tile_num_y = 10;
    app.entities = &ent;
    app.player_ent_id = 0;
    
    
    ent.x = -1;
    ent.y = 11;
    ent.beh = PLAYER;
    
    act_entity(&app, &ent);
    TEST_ASSERT_EQUAL(0, ent.x);
    TEST_ASSERT_EQUAL(9, ent.y); 
}

void test_get_depth(void) {
    entity_t ent = {.x = 3, .y = 4};
    TEST_ASSERT_EQUAL(8, get_depth(&ent)); 
    
    ent.x = 0;
    ent.y = 0;
    TEST_ASSERT_EQUAL(1, get_depth(&ent));
}

void test_RE_init_grid(void) {
    grid_t grid = {0};
    int result = RE_init_grid(&grid, 5, 5, 32, 32, 0);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(5, grid.tile_num_x);
    TEST_ASSERT_EQUAL(5, grid.tile_num_y);

    destroy_grid(&grid);
}

void test_RE_init_grid_null_pointer(void) {
    int result = RE_init_grid(NULL, 5, 5, 32, 32, 0);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
}

void run_app_tests(void) {
    RUN_TEST(test_app_create_destroy);
    RUN_TEST(test_cam_process_key_event);
    RUN_TEST(test_act_entity_boundaries);
    RUN_TEST(test_get_depth);
    RUN_TEST(test_RE_init_grid);
    RUN_TEST(test_RE_init_grid_null_pointer);
}