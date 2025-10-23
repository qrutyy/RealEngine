#include "app.h"
#include "scene.h"
#include "asset_manager.h"
#include "user_defined.h"
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

    user_function(app);

    app_run(app);

    app_destroy(app);

    return EXIT_SUCCESS;
}
