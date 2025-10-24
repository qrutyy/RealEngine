#ifndef TYPES_H
#define TYPES_H 

#include <stdint.h>
#include <SDL3/SDL.h>

typedef struct grid {
    SDL_Surface ***tiles; // a matrix of pointers to assets
    int tile_num_x;
    int tile_num_y;
    int tile_width; // tile width in pixels
    int tile_height; // tile height in pixels
} grid_t;

typedef struct m_vec2{
    int x;
    int y;
} m_vec2_t;

typedef struct m_vec3 {
    int x;
    int y;
    int z;
} m_vec3_t;

enum g_object_type {
    GAME_OBJECT_NONE,
    GAME_OBJECT_WHATEVER,
};

struct g_object {
    enum g_object_type type;
    struct m_vec3 position;
};

struct rgb_pix {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

#endif
