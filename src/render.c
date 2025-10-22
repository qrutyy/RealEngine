#include "render.h"
#include "app.h"
#include <stdint.h>

// Draw grid (with zoom/camera offset adjustment)
void render_draw_grid(app_hlpr_t *app) {
    world_t *world = app->world;
    float zoom = world->cam_zoom; // TODO: fix zoom feature
    int cam_x = world->cam_pos.x;
    int cam_y = world->cam_pos.y;

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int grid_x = x - cam_x;
            int grid_y = y - cam_y;
            int sx = (grid_x - grid_y) * TILE_WIDTH_HALF * zoom + OFFSET_X;
            int sy = (grid_x + grid_y) * TILE_HEIGHT_HALF * zoom + OFFSET_Y;
            SDL_FPoint points[5] = {
                { (float)sx, (float)(sy + TILE_HEIGHT_HALF * zoom) },
                { (float)(sx + TILE_WIDTH_HALF * zoom), (float)sy },
                { (float)sx, (float)(sy - TILE_HEIGHT_HALF * zoom) },
                { (float)(sx - TILE_WIDTH_HALF * zoom), (float)sy },
                { (float)sx, (float)(sy + TILE_HEIGHT_HALF * zoom) }
            };
            SDL_SetRenderDrawColor(app->renderer, 100, 120, 150, 255);
            SDL_RenderLines(app->renderer, points, 5);
        }
    }
}

void render_fps_bar(app_hlpr_t *app, uint64_t now) {
    static uint64_t last_update = 0;
    static int frame_count = 0;
    static int current_fps = 0;
    char debug_string[32];
    float prevScaleX, prevScaleY;

    ++frame_count;
    if (now - last_update > 1000000000) { // 1 second in nanoseconds
        current_fps = frame_count;
        frame_count = 0;
        last_update = now;
    }
    // Make FPS display bigger and offset from the corner
    
    SDL_GetRenderScale(app->renderer, &prevScaleX, &prevScaleY);
    SDL_SetRenderScale(app->renderer, 2.5f, 2.5f); 
    SDL_snprintf(debug_string, sizeof(debug_string), "%d fps", current_fps);
    SDL_RenderDebugText(app->renderer, 12, 14, debug_string); // offset from top-left
    SDL_SetRenderScale(app->renderer, prevScaleX, prevScaleY); // restore scale
}

void render_scene(app_hlpr_t* app) {
    Uint64 now = SDL_GetTicksNS();

    SDL_SetRenderDrawColor(app->renderer, 20, 20, 30, 255); 
    SDL_RenderClear(app->renderer);

	render_draw_grid(app);

	render_fps_bar(app, now);

    SDL_RenderPresent(app->renderer);
}


