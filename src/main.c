#include "app.h"
#include "scene.h"
#include "asset.h"
#include "include.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    app_hlpr_t* app = app_create();
    if (!app) {
        return EXIT_FAILURE;
    }

    // AFTER creating an app or BEFORE?
    // call user functions which create entities and link
    // them with assets
    // load static assets (world, objects)
    user_function(&app->grid);

    int err = app_setup(app);
    if (err) app_destroy(app);

    app_run(app);

    app_destroy(app);

    return EXIT_SUCCESS;
}
