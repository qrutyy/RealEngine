#include "user_defined.h"

const int WIDTH = 20;
const int HEIGHT = 20;

const int TILE_WIDTH = 128;
const int TILE_HEIGHT = 64;

// load assets and link them here
int user_function(grid_t *grid) {
    RE_init_grid(grid, WIDTH, HEIGHT, TILE_WIDTH, TILE_HEIGHT);

    char *key = RE_load_asset(
        "assets/tiles/ScreamingBrainStudios/Isometric_Floor_Tiles_Small_128x64/Small_128x64/Exterior/Grass/Floor_Grass_02-128x64p.png",
        256, 64, 128, 64
    );

    char *key2 = RE_load_asset(
        "assets/tiles/ScreamingBrainStudios/Isometric_Floor_Tiles_Small_128x64/Small_128x64/Exterior/Elements/Floor_Elements-01-128x64p.png",
        128, 128, 128, 64
    );

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            RE_assign_asset_static(grid, key, 0, x, y);
    
            if (x >= 5 && x <= 6) {
                RE_assign_asset_static(grid, key2, 0, x, y);
            }
            if (y >= 10 && y <= 11) {
                RE_assign_asset_static(grid, key2, 0, x, y);
            }
        }
    }

    RE_add_entity(0, 1, 0);
    // RE_add_entity(0, 1, 0);

    return 0;
}