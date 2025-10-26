#include "app.h"
#include "scene.h"
#include "asset.h"
#include "include.h"
#include "log.h"
#include <stdlib.h>


int RE_main(grid_t *grid);

int main(int argc, char *argv[]) {
	int err;
    (void)argc;
    (void)argv;

    app_hlpr_t* app = app_create();
    if (!app)
        return EXIT_FAILURE;

    // AFTER creating an app or BEFORE?
    // call user functions which create entities and link
    // them with assets
    // load static assets (world, objects)
    err = RE_main(&app->grid);
	if (err) {
		log_error("User side error\n");
		return EXIT_FAILURE;
	}

    err = app_setup(app);
    if (err) {
        app_destroy(app);
        return err;
    }
        
    app_run(app);

    app_destroy(app);

    return EXIT_SUCCESS;
}
