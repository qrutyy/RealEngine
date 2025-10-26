#include "include.h"
#include "scene.h"
#include "log.h"

const int WIDTH = 20;
const int HEIGHT = 20;

const int TILE_WIDTH = 128;
const int TILE_HEIGHT = 64;

// TODO: move file to the

int RE_main(grid_t *grid) {

    RE_init_grid(grid, WIDTH, HEIGHT, TILE_WIDTH, TILE_HEIGHT);

    int key = RE_load_asset(
        "assets/tiles/ScreamingBrainStudios/Isometric_Floor_Tiles_Small_128x64/Small_128x64/Exterior/Grass/Floor_Grass_02-128x64p.png",
        0, 64, 128, 64
    );

    int key2 = RE_load_asset(
        "assets/tiles/ScreamingBrainStudios/Isometric_Floor_Tiles_Small_128x64/Small_128x64/Exterior/Elements/Floor_Elements-01-128x64p.png",
        128, 128, 128, 64
    );

    int key3 = RE_load_asset(MAIN_CHAR_IMG_S, MAIN_CHAR_SRC_X, MAIN_CHAR_SRC_Y, MAIN_CHAR_WIDTH, MAIN_CHAR_HEIGHT);


    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // if ((x + y) % 2 == 0) {
            RE_assign_asset_static(grid, key, 0, x, y);
            // } else {
            //     RE_assign_asset_static(grid, key2, 0, x, y);
            // }
        }
    }
    // RE_assign_asset_static(grid, key, 0, 0, 0);

    RE_add_entity(0, 0, PLAYER);
    // RE_add_entity(4, 5, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(2, 10, NPC);
    // RE_add_entity(0, 1, NPC);
    // RE_add_entity(0, 2, NPC);
    // RE_add_entity(0, 0, FOLLOW);
    // RE_add_entity(2, 10, FOLLOW);

    // RE_add_entity(0, 1, 0);

    return 0;
}
