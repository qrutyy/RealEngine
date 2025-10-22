#include "app.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    // call user functions which create entities and link
    // them with assets
    // load static assets (world, objects)

    app_hlpr_t* app = app_create();
    if (!app) {
        return EXIT_FAILURE;
    }

    app_run(app);

    app_destroy(app);

    return EXIT_SUCCESS;
}
