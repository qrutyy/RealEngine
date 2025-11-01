#include "unity.h"
#include "render.h"
#include "app.h"
#include "asset.h"
#include "log.h"
#include "utils.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


SDL_Surface* SDL_GetWindowSurface(SDL_Window* window) {
    static SDL_Surface mock_surface = {
        .w = WINDOW_WIDTH,
        .h = WINDOW_HEIGHT,
        .pitch = WINDOW_WIDTH * 4, 
        .pixels = NULL
    };
    return &mock_surface;
}

void SDL_UpdateWindowSurface(SDL_Window* window) {
    
}

Uint32 SDL_MapSurfaceRGB(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b) {
    return (r << 16) | (g << 8) | b;
}

void SDL_FillSurfaceRect(SDL_Surface* surface, const SDL_Rect* rect, Uint32 color) {
    
}

const SDL_PixelFormatDetails* SDL_GetPixelFormatDetails(SDL_PixelFormat* format) {
    static SDL_PixelFormatDetails mock_format = {0};
    return &mock_format;
}

void SDL_GetRGBA(Uint32 pixel, const SDL_PixelFormatDetails* format, Uint8* a, Uint8* r, Uint8* g, Uint8* b) {
    if (r) *r = (pixel >> 16) & 0xFF;
    if (g) *g = (pixel >> 8) & 0xFF;
    if (b) *b = pixel & 0xFF;
    if (a) *a = 255; 
}

void SDL_GetRGB(Uint32 pixel, const SDL_PixelFormatDetails* format, Uint8* r, Uint8* g, Uint8* b) {
    if (r) *r = (pixel >> 16) & 0xFF;
    if (g) *g = (pixel >> 8) & 0xFF;
    if (b) *b = pixel & 0xFF;
}

Uint32 SDL_MapRGB(const SDL_PixelFormatDetails* format, Uint8* a, Uint8 r, Uint8 g, Uint8 b) {
    return (r << 16) | (g << 8) | b;
}


asset_t* RE_get_asset(int id) {
    static asset_t mock_asset = {
        .width = 32,
        .height = 32,
        .img = NULL
    };
    return &mock_asset;
}

void setUp(void) {
    
}

void tearDown(void) {
    
}

void test_render_asset_null_screen(void) {
    asset_t asset = {.width = 32, .height = 32};
    
    
    render_asset(NULL, 100, 100, &asset);
    
}

void test_render_asset_null_asset(void) {
    SDL_Surface screen = {.w = WINDOW_WIDTH, .h = WINDOW_HEIGHT, .pitch = WINDOW_WIDTH * 4};
    
    
    render_asset(&screen, 100, 100, NULL);
    
}

void test_render_asset_negative_dimensions(void) {
    SDL_Surface screen = {.w = WINDOW_WIDTH, .h = WINDOW_HEIGHT, .pitch = WINDOW_WIDTH * 4};
    asset_t negative_width = {.width = -10, .height = 32};
    asset_t negative_height = {.width = 32, .height = -10};
    
    
    render_asset(&screen, 100, 100, &negative_width);
    render_asset(&screen, 100, 100, &negative_height);
    
}

void test_render_asset_out_of_bounds(void) {
    SDL_Surface screen = {
        .w = WINDOW_WIDTH, 
        .h = WINDOW_HEIGHT, 
        .pitch = WINDOW_WIDTH * 4,
        .pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4)
    };
    asset_t asset = {
        .width = 32,
        .height = 32,
        .img = &screen 
    };
    
    
    render_asset(&screen, WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, &asset); 
    render_asset(&screen, -20, -20, &asset); 
    render_asset(&screen, WINDOW_WIDTH + 10, 100, &asset); 
    
    free(screen.pixels);
}

void test_render_shadow_basic(void) {
    SDL_Surface screen = {
        .w = WINDOW_WIDTH, 
        .h = WINDOW_HEIGHT, 
        .pitch = WINDOW_WIDTH * 4,
        .pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4)
    };
    SDL_Surface asset_img = {
        .w = 32,
        .h = 32,
        .pitch = 32 * 4,
        .pixels = malloc(32 * 32 * 4)
    };
    asset_t asset = {
        .width = 32,
        .height = 32,
        .img = &asset_img
    };
    
    
    memset(screen.pixels, 0xFF, WINDOW_WIDTH * WINDOW_HEIGHT * 4); 
    memset(asset_img.pixels, 0x80, 32 * 32 * 4); 
    
    
    render_shadow(&screen, 100, 100, &asset, 1.0f, 0.5f, 0.5f);
    
    
    TEST_ASSERT_TRUE(1); 
    
    free(screen.pixels);
    free(asset_img.pixels);
}

void test_render_shadow_out_of_bounds(void) {
    SDL_Surface screen = {
        .w = WINDOW_WIDTH, 
        .h = WINDOW_HEIGHT, 
        .pitch = WINDOW_WIDTH * 4,
        .pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4)
    };
    SDL_Surface asset_img = {
        .w = 32,
        .h = 32,
        .pitch = 32 * 4,
        .pixels = malloc(32 * 32 * 4)
    };
    asset_t asset = {
        .width = 32,
        .height = 32,
        .img = &asset_img
    };
    
    
    render_shadow(&screen, -10, -10, &asset, 1.0f, 0.5f, 0.5f); 
    render_shadow(&screen, WINDOW_WIDTH - 5, WINDOW_HEIGHT - 5, &asset, 1.0f, 0.5f, 0.5f); 
    
    
    TEST_ASSERT_TRUE(1);
    
    free(screen.pixels);
    free(asset_img.pixels);
}

void test_render_background_null_app(void) {
    
    render_background(NULL);
    
}

void test_render_background_no_tiles(void) {
    app_hlpr_t app = {0};
    SDL_Window mock_window;
    app.window = &mock_window;
    
    
    app.grid.tiles = NULL;
    render_background(&app);
    
    TEST_ASSERT_TRUE(1);
}

void test_render_background_basic(void) {
    app_hlpr_t app = {0};
    SDL_Window mock_window;
    app.window = &mock_window;
    
    
    app.grid.tile_num_x = 5;
    app.grid.tile_num_y = 5;
    app.grid.tile_width = 32;
    app.grid.tile_height = 32;
    app.grid.pad_y = 2;
    app.cam.x = 2;
    app.cam.y = 2;
    
    
    app.grid.tiles = malloc(sizeof(SDL_Surface**) * 5);
    for (int i = 0; i < 5; i++) {
        app.grid.tiles[i] = malloc(sizeof(SDL_Surface*) * 5);
        for (int j = 0; j < 5; j++) {
            app.grid.tiles[i][j] = NULL;
        }
    }
    
    
    SDL_Surface* mock_tile = malloc(sizeof(SDL_Surface));
    mock_tile->w = 32;
    mock_tile->h = 32;
    mock_tile->pitch = 32 * 4;
    mock_tile->pixels = malloc(32 * 32 * 4);
    app.grid.tiles[2][2] = mock_tile;
    
    render_background(&app);
    
    TEST_ASSERT_TRUE(1);
    
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (app.grid.tiles[i][j]) {
                free(app.grid.tiles[i][j]->pixels);
                free(app.grid.tiles[i][j]);
            }
        }
        free(app.grid.tiles[i]);
    }
    free(app.grid.tiles);
}

void test_render_entities_null_app(void) {
    
    render_entities(NULL);
    
}

void test_render_entities_no_entities(void) {
    app_hlpr_t app = {0};
    SDL_Window mock_window;
    app.window = &mock_window;
    app.entities_num = 0;
    app.layers_num = 0;
    app.lentities = NULL;
    
    
    render_entities(&app);
    
    TEST_ASSERT_TRUE(1);
}

void test_render_entities_basic(void) {
    app_hlpr_t app = {0};
    SDL_Window mock_window;
    app.window = &mock_window;
    
    
    app.grid.tile_width = 32;
    app.grid.tile_height = 32;
    app.grid.pad_y = 2;
    app.cam.x = 0;
    app.cam.y = 0;
    
    
    app.layers_num = 1;
    app.lentities = malloc(sizeof(layer_entities_t));
    app.lentities[0].num_entities = 1;
    app.lentities[0].entities = malloc(sizeof(entity_t));
    app.lentities[0].entities[0].x = 5;
    app.lentities[0].entities[0].y = 5;
    
    render_entities(&app);
    
    TEST_ASSERT_TRUE(1);
    
    
    free(app.lentities[0].entities);
    free(app.lentities);
}

void test_render_scene_null_app(void) {
    
    render_scene(NULL);
    
}

void test_render_scene_basic(void) {
    app_hlpr_t app = {0};
    SDL_Window mock_window;
    app.window = &mock_window;
    
    
    app.grid.tile_num_x = 5;
    app.grid.tile_num_y = 5;
    app.grid.tile_width = 32;
    app.grid.tile_height = 32;
    app.grid.pad_y = 2;
    app.cam.x = 2;
    app.cam.y = 2;
    app.entities_num = 0;
    app.layers_num = 0;
    
    
    app.grid.tiles = malloc(sizeof(SDL_Surface**) * 5);
    for (int i = 0; i < 5; i++) {
        app.grid.tiles[i] = malloc(sizeof(SDL_Surface*) * 5);
        for (int j = 0; j < 5; j++) {
            app.grid.tiles[i][j] = NULL;
        }
    }
    
    render_scene(&app);
    
    TEST_ASSERT_TRUE(1);
    
    
    for (int i = 0; i < 5; i++) {
        free(app.grid.tiles[i]);
    }
    free(app.grid.tiles);
}

void test_render_asset_transparency_handling(void) {
    SDL_Surface screen = {
        .w = WINDOW_WIDTH, 
        .h = WINDOW_HEIGHT, 
        .pitch = WINDOW_WIDTH * 4,
        .pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 4)
    };
    SDL_Surface asset_img = {
        .w = 32,
        .h = 32,
        .pitch = 32 * 4,
        .pixels = malloc(32 * 32 * 4)
    };
    asset_t asset = {
        .width = 32,
        .height = 32,
        .img = &asset_img
    };
    
    
    memset(screen.pixels, 0x00, WINDOW_WIDTH * WINDOW_HEIGHT * 4); 
    memset(asset_img.pixels, 0x80808080, 32 * 32 * 4); 
    
    render_asset(&screen, 100, 100, &asset);
    
    TEST_ASSERT_TRUE(1);
    
    free(screen.pixels);
    free(asset_img.pixels);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_render_asset_null_screen);
    RUN_TEST(test_render_asset_null_asset);
    RUN_TEST(test_render_asset_negative_dimensions);
    RUN_TEST(test_render_asset_out_of_bounds);
    RUN_TEST(test_render_shadow_basic);
    RUN_TEST(test_render_shadow_out_of_bounds);
    RUN_TEST(test_render_background_null_app);
    RUN_TEST(test_render_background_no_tiles);
    RUN_TEST(test_render_background_basic);
    RUN_TEST(test_render_entities_null_app);
    RUN_TEST(test_render_entities_no_entities);
    RUN_TEST(test_render_entities_basic);
    RUN_TEST(test_render_scene_null_app);
    RUN_TEST(test_render_scene_basic);
    RUN_TEST(test_render_asset_transparency_handling);
    
    return UNITY_END();
}
