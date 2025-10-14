#include "renderer.h"
#include "app.h"

void renderer_draw_player(app_hlpr_t *app) {
    world_t *world = app->world;
    float zoom = world->cam_zoom;
    int cam_x = world->cam_pos.x;
    int cam_y = world->cam_pos.y;
    int tile_x = world->pl_pos.x - cam_x;
    int tile_y = world->pl_pos.y - cam_y;
    int sx = (tile_x - tile_y) * TILE_WIDTH_HALF * zoom + OFFSET_X;
    int sy = (tile_x + tile_y) * TILE_HEIGHT_HALF * zoom + OFFSET_Y;
    SDL_FPoint points[5] = {
        { (float)sx, (float)(sy + TILE_HEIGHT_HALF * zoom) },
        { (float)(sx + TILE_WIDTH_HALF * zoom), (float)sy },
        { (float)sx, (float)(sy - TILE_HEIGHT_HALF * zoom) },
        { (float)(sx - TILE_WIDTH_HALF * zoom), (float)sy },
        { (float)sx, (float)(sy + TILE_HEIGHT_HALF * zoom) }
    };
    SDL_SetRenderDrawColor(app->renderer, 255, 100, 30, 255);
    SDL_RenderLines(app->renderer, points, 5);
}


// Draw grid (with zoom/camera offset adjustment)
void renderer_draw_grid(app_hlpr_t *app) {
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

void scene_render(app_hlpr_t* app) {
    SDL_SetRenderDrawColor(app->renderer, 20, 20, 30, 255); 
    SDL_RenderClear(app->renderer);

	renderer_draw_grid(app);
	renderer_draw_player(app);

    SDL_RenderPresent(app->renderer);
}


