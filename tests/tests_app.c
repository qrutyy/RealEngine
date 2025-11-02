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
    TEST_ASSERT_NOT_NULL(app);
    TEST_ASSERT_FALSE(app->is_running); 
    
    app_destroy(app);
}

void test_cam_process_key_event(void) {
    app_hlpr_t *app = app_create();
    SDL_KeyboardEvent event;

    RE_init_grid(&app->grid, 10, 10, 0, 0, 0);
    
    app->cam.x = 5;
    app->cam.y = 5;
    
    
    event.key = SDLK_UP;
    cam_process_key_event(event, app, SDL_EVENT_KEY_DOWN);
    TEST_ASSERT_EQUAL(4, app->cam.x);
    TEST_ASSERT_EQUAL(4, app->cam.y);
    
    
    app->cam.x = 0;
    app->cam.y = 0;
    event.key = SDLK_LEFT;
    cam_process_key_event(event, app, SDL_EVENT_KEY_DOWN);
    TEST_ASSERT_EQUAL(0, app->cam.x); 

    app_destroy(app);
}

void test_cam_process_key_event_boundary(void) {
    app_hlpr_t *app = app_create();
    SDL_KeyboardEvent event;
    RE_init_grid(&app->grid, 10, 10, 0, 0, 0);

    app->cam.x = 0;
    app->cam.y = 0;

    event.key = SDLK_UP;
    cam_process_key_event(event, app, SDL_EVENT_KEY_DOWN);
    TEST_ASSERT_EQUAL(0, app->cam.x);
    TEST_ASSERT_EQUAL(0, app->cam.y);

    app_destroy(app);
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
    TEST_ASSERT_EQUAL(32, grid.tile_width);
    TEST_ASSERT_EQUAL(32, grid.tile_height);
    TEST_ASSERT_EQUAL(0, grid.pad_y);

    destroy_grid(&grid);
}

void test_RE_init_grid_null_pointer(void) {
    int result = RE_init_grid(NULL, 5, 5, 32, 32, 0);
    TEST_ASSERT_EQUAL(ERR_ARGS, result);
}

void run_app_tests(void) {
    RUN_TEST(test_app_create_destroy);
    RUN_TEST(test_cam_process_key_event);
    RUN_TEST(test_cam_process_key_event_boundary);
    RUN_TEST(test_get_depth);
    RUN_TEST(test_RE_init_grid);
    RUN_TEST(test_RE_init_grid_null_pointer);
}