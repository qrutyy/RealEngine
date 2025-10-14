#include "renderer.h"
#include "app.h"


void renderer_draw_grid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 100, 120, 150, 255); // Цвет линий

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int sx = (x - y) * TILE_WIDTH_HALF;
            int sy = (x + y) * TILE_HEIGHT_HALF;
            sx += OFFSET_X;
            sy += OFFSET_Y;

            SDL_FPoint points[5] = {
                { (float)sx, (float)(sy + TILE_HEIGHT_HALF) }, // lower point
                { (float)(sx + TILE_WIDTH_HALF), (float)sy },  // right point
                { (float)sx, (float)(sy - TILE_HEIGHT_HALF) }, // upper point
                { (float)(sx - TILE_WIDTH_HALF), (float)sy },  // left point
                { (float)sx, (float)(sy + TILE_HEIGHT_HALF) } 
            };
            SDL_RenderLines(renderer, points, 5);
        }
    }
}
