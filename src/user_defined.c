#include "include.h"
#include "scene.h"

const int WIDTH = 30;
const int HEIGHT = 30;

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILE_THICKNESS = 8;

int user_function(grid_t *grid) {
    RE_init_grid(grid, WIDTH, HEIGHT, TILE_WIDTH, TILE_HEIGHT, TILE_THICKNESS);

    int a1 = RE_load_asset(
        "assets/tiles/philticular/isometric_jumpstart_v230311(1)/isometric_jumpstart_v230311/iso_tile_export.png",
        256, 64, 32, 32
    );

    int a2 = RE_load_asset(
        "assets/tiles/philticular/isometric_jumpstart_v230311(1)/isometric_jumpstart_v230311/iso_tile_export.png",
        128, 192, 32, 32
    );

    int pl_south = RE_load_asset(
        "assets/characters/scrabling/critters/boar/boar_NW_idle_0.png",
        0, 0, 46, 32
    );

    // int pl_south = RE_load_asset(MAIN_CHAR_IMG_S, MAIN_CHAR_SRC_X, MAIN_CHAR_SRC_Y, MAIN_CHAR_WIDTH, MAIN_CHAR_HEIGHT);
    // int pl_east = RE_load_asset(MAIN_CHAR_IMG_E, MAIN_CHAR_SRC_X, MAIN_CHAR_SRC_Y, MAIN_CHAR_WIDTH, MAIN_CHAR_HEIGHT);
    // int pl_north = RE_load_asset(MAIN_CHAR_IMG_N, MAIN_CHAR_SRC_X, MAIN_CHAR_SRC_Y, MAIN_CHAR_WIDTH, MAIN_CHAR_HEIGHT);
    // int pl_west = RE_load_asset(MAIN_CHAR_IMG_W, MAIN_CHAR_SRC_X, MAIN_CHAR_SRC_Y, MAIN_CHAR_WIDTH, MAIN_CHAR_HEIGHT);

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // if ((x + y) % 2 == 0) {
            if (x > 10 && x < 13) {
                RE_assign_asset_static(grid, a2, 0, x, y);
            } else {
                RE_assign_asset_static(grid, a1, 0, x, y);
            }
            // } else {
            //     RE_assign_asset_static(grid, key2, 0, x, y);
            // }
        }
    }

    int pl_south_arr[] = { pl_south };
    int pl_east_arr[] = { pl_south };
    int pl_north_arr[] = { pl_south };
    int pl_west_arr[] = { pl_south };

    int anim_south = RE_create_animation(pl_south_arr, 1);
    int anim_east = RE_create_animation(pl_east_arr, 1);
    int anim_north = RE_create_animation(pl_north_arr, 1);
    int anim_west = RE_create_animation(pl_west_arr, 1);


    int epl = RE_add_entity(0, 1, PLAYER);
    RE_add_animation(anim_south, epl);
    RE_add_animation(anim_west, epl);
    RE_add_animation(anim_north, epl);
    RE_add_animation(anim_east, epl);

    int npc_num = 3;
    for (int i = 0; i < npc_num; i++) {
        int e =RE_add_entity(2, 10, NPC);
        RE_add_animation(anim_south, e);
        RE_add_animation(anim_west, e);
        RE_add_animation(anim_north, e);
        RE_add_animation(anim_east, e);
    }

    int follow_num = 1;
    for (int i = 0; i < follow_num; i++) {
        int e =RE_add_entity(2, 10, FOLLOW);
        RE_add_animation(anim_south, e);
        RE_add_animation(anim_west, e);
        RE_add_animation(anim_north, e);
        RE_add_animation(anim_east, e);
    }

    return 0;
}