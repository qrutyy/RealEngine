#include "engine.h"
#include "scene.h"
#include "entity.h"
#include "tilemap.h"
#include "input.h"
#include "event.h"
#include "camera.h"
#include "assetmanager.h"

typedef struct PlayerEntity {
    Entity base;
    int tile_x;
    int tile_y;
} PlayerEntity;

void player_update(Entity *self, float dt) {
    // Можно добавить анимацию или движение
}

void player_render(Entity *self) {
    PlayerEntity *p = (PlayerEntity*)self;
    printf("[Render] Player at tile (%d, %d)\n", p->tile_x, p->tile_y);
}

void player_on_event(Entity *self, const Event *e) {
    PlayerEntity *p = (PlayerEntity*)self;

    if (e->type == EVENT_CLICK) {
        p->tile_x = e->data.click.x;
        p->tile_y = e->data.click.y;
        printf("[Event] Player moved to (%d, %d)\n", p->tile_x, p->tile_y);
    }
}

EntityVTable PLAYER_VTABLE = {
    .update = player_update,
    .render = player_render,
    .on_event = player_on_event
};

PlayerEntity *player_create(int x, int y) {
    PlayerEntity *p = malloc(sizeof(PlayerEntity));
    p->base.vtable = &PLAYER_VTABLE;
	p->base.position = (Vec2){x, y}; 
	p->base.sprite =
	p->base.physics = RE_set_physics(bitmap that will be filled by conjunction of enums);
	p->base.events = 
    p->tile_x = x;
    p->tile_y = y;
    return p;
}

void create_map() {
	tilemap = RE_create_tile_map(num_of_layers, ..);
	layer = tilemap.init_tile_layer();

	layer.fill(tile, ..);
	layer.set_tile(position, new_tile, specs);
	return tilemap;
} 

int main(void) {
    Engine engine;
    Scene scene;
    InputManager input;
    Camera camera;
    AssetManager assets;

    RE_scene_init(&scene, NULL, entities?, layers_meta);
	scene.map = create_map();

    RE_input_init(&input);
	input.add_event();

    RE_assetmanager_init(&assets); ???

    RE_engine_init(&engine, &scene, &input, &camera, &assets);

	RE_init_camera()


    PlayerEntity *player = player_create() // мы позволяем реализовывать доп обертку поверх entity для добавления функциональности и допустим хранения значений.
    scene.add_entity(&scene, (Entity*)player);

    // запуск главного цикла
    engine_run(&engine);

    return 0;
}

