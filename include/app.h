#ifndef APP_H
#define APP_H

#include "entity.h"
#include "scene.h"
#include "types.h"
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define WINDOW_NAME "RealEngine"

typedef struct cam {
	int x;
	int y;
	int width;
	int height;
} cam_t;

typedef struct app_hlpr {
	SDL_KeyboardEvent key_event;
	SDL_Window *window;
	grid_t grid;
	layer_entities_t *lentities;
	int layers_num;
	entity_t *entities;
	int entities_num;
	int player_ent_id;
	cam_t cam;
	bool is_running;
} app_hlpr_t;

app_hlpr_t *app_create(void);
void app_destroy(app_hlpr_t *app);
void app_run(app_hlpr_t *app);
int app_setup(app_hlpr_t *app);

void cam_process_key_event(SDL_KeyboardEvent kb_event, app_hlpr_t *app, uint32_t sdl_kb_event_type);
void process_input(app_hlpr_t *app);
void destroy_grid(grid_t *grid);
void destroy_layers(layer_entities_t *layers, int layers_num);
int get_depth(entity_t *entity);
void act_entity(app_hlpr_t *app, entity_t *ent);
void update_state(app_hlpr_t *app);
int init_layers(app_hlpr_t *app);
void add_entities(app_hlpr_t *app);
int setup_player(app_hlpr_t *app);

#endif // APP_H
