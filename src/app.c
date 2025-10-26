#include "app.h"
#include "SDL3/SDL_events.h"
#include "render.h"
#include "log.h"
#include "scene.h"
#include "errors.h"
#include "camera.h"
#include "entity.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static void process_input(app_hlpr_t* app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            app->is_running = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
            app->is_running = false;
        }
		if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			if (event.key.key <= SDLK_UP && event.key.key >= SDLK_RIGHT) {
				app->key_event = event.key;	
				cam_process_key_event(app->key_event, &app->cam, &app->entities[app->player_ent_id], event.type);
			}
		}
    }
}

app_hlpr_t* app_create(void) {
    app_hlpr_t* app = calloc(1, sizeof(struct app_hlpr));
    if (!app) {
        log_error("Failed to allocate app\n");
        return NULL;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        log_error("SDL_Init Error: %s\n", SDL_GetError());
		goto err_ex;
    }

    app->window = SDL_CreateWindow(WINDOW_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!app->window) {
        log_error("sdl_createwindow error: %s\n", SDL_GetError());
		goto err_ex;
    }

    app->is_running = true;
    return app;

err_ex:
    free(app);
    return NULL;
}

static void destroy_grid(grid_t *grid) {
    if (!grid) {
        log_debug("Grid pointer is NULL.\n");
        return;
    }

    if (!grid->tiles) {
        log_debug("Tiles pointer is NULL.\n");
        return;
    }

    for (int i = 0; i < grid->tile_num_x; i++) {
        if (grid->tiles[i]) {
            free(grid->tiles[i]);
            grid->tiles[i] = NULL;
        }
    }

    free(grid->tiles);
    grid->tiles = NULL;

    log_debug("Destroyed grid.\n");
}

void app_destroy(app_hlpr_t *app) {
    if (!app) return;
    destroy_grid(&app->grid);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
    free(app);
}

void act_entity(entity_t *ent, entity_t player) {
    if (!ent) return;

    if (ent->beh == PLAYER) {
        // printf("a player acts like a player.\n");
        // printf("player entity is on x, y: %d, %d\n", ent->x, ent->y);
    } else if (ent->beh == NPC) {
        int rand = SDL_rand(4);
        switch (rand) {
            case 0:
                break;
            case 1:
                ent->x++;
                ent->y++;
                break;
            case 2:
                ent->x--;
                ent->y++;
                break;
            case 3:
                ent->x++;
                ent->y--;
                break;
            case 4:
                ent->x--;
                ent->y--;
                break;
        }
    } else if (ent->beh == FOLLOW) {
        int dist = 2;
        if (player.x - ent->x > dist)  {
            ent->x++;
        } else if (ent->x - player.x  > dist) {
            ent->x--;
        }
        if (player.y - ent->y > dist) {
            ent->y++;
        } else if (ent->y - player.y > dist) {
            ent->y--;
        }
        // log_debug("follow entity is on %d, %d", ent->x, ent->y);
    }
}

static inline int get_depth(entity_t *entity) {
    return entity->x + entity->y + 1;
}

static void update_state(app_hlpr_t *app) {
    SDL_Window *window = app->window;
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    int num = app->entities_num;
    entity_t *entities = app->entities;

    layer_entities_t *layers = app->lentities;

    for (int i = 0; i < app->layers_num; i++) {
        app->lentities[i].num_entities = 0;
    }

    for (int i = 0; i < num; i++) {
        // change entities placement somehow
        entity_t *entity = &entities[i];

        act_entity(entity, app->entities[app->player_ent_id]);

        int depth = get_depth(entity);
        if (depth < 1) {
            depth = 1;
        }

        int max_layers_num = app->grid.tile_num_x + app->grid.tile_num_y + 1;
        max_layers_num *= 10;
        if (depth > max_layers_num) {
            depth = max_layers_num;
        }

        entity->depth = depth;

        // this updates location in memory

        int num_entities = app->lentities[depth].num_entities++;
        app->lentities[depth].entities[num_entities] = *entity;

        // log_debug("entity %d x,y: %d, %d, depth: %d", i, entity.x, entity.y, depth);
    }

    // shadows, etc
    // for ()
}

void app_run(app_hlpr_t *app) {
    while (app->is_running) {
        process_input(app);
        update_state(app);
        render_scene(app);
        // SDL_Delay(16);
    }
}

static int init_layers(app_hlpr_t *app) {
    int max_layers_num = app->grid.tile_num_x + app->grid.tile_num_y + 1;

    max_layers_num *= 10; // to avoid errors for now, when out of bound is possible
    // better to allocate more dynamically when needed

    layer_entities_t *layers = malloc(sizeof(layer_entities_t) * max_layers_num);
    if (!layers) {
        log_debug("Failed to init layers\n");
        return ERR_NOMEM;
    }

    for (int l = 0; l < max_layers_num; l++) {
        entity_t *entities = malloc(sizeof(entity_t) * MAX_ENTITIES_PER_LAYER);
        layers[l].num_entities = 0;
        layers[l].entities = entities;
        if (!entities) {
            log_error("Failed to allocate memory for entities on layer %d.\n", l);

            for (int j = 0; j < l; j++) {
                free(layers[j].entities);
            }
            free(layers);
            return ERR_NOMEM;
        }
    }

    for (int i = 0; i < app->entities_num; i++) {
        entity_t entity = app->entities[i];
        int depth = get_depth(&entity);
        layer_entities_t *layer = &layers[depth];

        layer->entities[layer->num_entities++] = entity;
        // log_debug("put entity %d to depth %d", i, depth);
    }

    app->lentities = layers;
    app->layers_num = max_layers_num;
    // log_debug("initialized %d layers.", max_layers_num);

    return 0;
}

static void add_entities(app_hlpr_t *app) {
    app->entities = get_entities();
    app->entities_num = get_entities_num();
    init_layers(app);
}

static int setup_player(app_hlpr_t *app) {
    for (int i = 0; i < app->entities_num; i++) {
        entity_t ent = app->entities[i];
        if (ent.beh == PLAYER) {
            app->player_ent_id = i;
            app->cam.x = ent.x;
            app->cam.y = ent.y;
            return 0;
        }
    }

    log_debug("Error: player entity was not set.\n");
    return 1;
}

int app_setup(app_hlpr_t *app) {
    add_entities(app);

    int err = setup_player(app);
    return err;
}

int RE_init_grid(grid_t *grid, int tile_num_x, int tile_num_y, int tile_width, int tile_height) {
    SDL_Surface ***tiles;

    if (!grid) {
        log_debug("Failed to init grid: a grid pointer should not be NULL pointer.\n");
        return ERR_ARGS;
    }

    tiles = malloc(sizeof(SDL_Surface **) * tile_num_x);
    if (!tiles) {
        log_error("Failed to allocate memory for tiles.\n");
        return ERR_NOMEM;
    }

    for (int k = 0; k < tile_num_x; k++) {
        tiles[k] = malloc(sizeof(SDL_Surface *) * tile_num_y);
        if (!tiles[k]) {
            log_error("Failed to allocate memory for tiles[%d].\n", k);
            for (int f = 0; f < k; f++) {
                free(tiles[f]);
            }
            free(tiles);
            return ERR_NOMEM;
        }
        for (int j = 0; j < tile_num_y; j++) {
            tiles[k][j] = NULL;
        }
    }

    grid->tiles = tiles;
    grid->tile_num_x = tile_num_x;
    grid->tile_num_y = tile_num_y;
    grid->tile_width = tile_width;
    grid->tile_height = tile_height;
    log_debug("Initialized grid %d x %d", tile_num_x, tile_num_y);

    return 0;
}
