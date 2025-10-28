#ifndef RENDER_H
#define RENDER_H

#include "app.h"
#include <SDL3/SDL.h>

// your custom height and width are here
#include "scene.h"

#define WORLD_BORDER_Y (GRID_HEIGHT)
#define WORLD_BORDER_X (GRID_WIDTH)

#define OFFSET_X (WINDOW_WIDTH / 2)
#define OFFSET_Y 0

void render_draw_grid(app_hlpr_t *app);
void render_scene(app_hlpr_t *app);

#endif // RENDERER_H
