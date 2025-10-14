#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

#define TILE_WIDTH_HALF  16
#define TILE_HEIGHT_HALF 8

#define GRID_WIDTH  50
#define GRID_HEIGHT 50

#define OFFSET_X (WINDOW_WIDTH / 2)
#define OFFSET_Y 0 

// Основная функция, которая будет рисовать нашу сетку
void renderer_draw_grid(SDL_Renderer* renderer);

#endif // RENDERER_H
