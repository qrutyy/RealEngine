#include "app.h"
#include "scene.h"
#include "asset.h"
#include "include.h"
#include "cfg_loader.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

	map_layout_cfg_t config;

    app_hlpr_t* app = app_create();
    if (!app) {
        return EXIT_FAILURE;
    }
	
	printf("Parsing config file 'scene.cfg'...\n");
    parse_config("scene.cfg", &config);

	printf("Initializing scene from config...\n");
    load_cfg(&app->grid, &config);

    int err = app_setup(app);
    if (err) {
        app_destroy(app);
        return err;
    }
        
    app_run(app);

    app_destroy(app);

    return EXIT_SUCCESS;
}
