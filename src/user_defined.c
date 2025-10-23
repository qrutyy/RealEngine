#include "user_defined.h"

int user_function(const app_hlpr_t *app) {
    char *key = RE_load_asset(
        "assets/tiles/ScreamingBrainStudios/Isometric_Floor_Tiles_Small_128x64/Small_128x64/Exterior/Grass/Floor_Grass_02-128x64p.png",
        256, 64, 128, 64
    );

    char *key2 = RE_load_asset(
        "assets/tiles/ScreamingBrainStudios/Isometric_Floor_Tiles_Small_128x64/Small_128x64/Exterior/Elements/Floor_Elements-01-128x64p.png",
        128, 128, 128, 64
    );

    for (int x = 0; x < 50; x++) {
        for (int y = 0; y < 50; y++) {
            if (x == 0  && y == 0) continue;
            RE_assign_asset_static(app, key, 0, x, y);
    
            if (x >= 5 && x <= 6) {
                RE_assign_asset_static(app, key2, 0, x, y);
            }
            if (y >= 10 && y <= 11) {
                RE_assign_asset_static(app, key2, 0, x, y);
            }
        }
    }

    return 0;
}