#ifndef RENDER_H
#define RENDER_H

#include "app.h"
#include <SDL3/SDL.h>

#define TILE_WIDTH_HALF  16
#define TILE_HEIGHT_HALF 8

#define GRID_WIDTH  50
#define GRID_HEIGHT 50

#define WORLD_BORDER_Y (GRID_HEIGHT)
#define WORLD_BORDER_X (GRID_WIDTH)

#define OFFSET_X (WINDOW_WIDTH / 2)
#define OFFSET_Y 0 

void render_draw_grid(app_hlpr_t *app);
void render_scene(app_hlpr_t *app);

#endif // RENDERER_H
