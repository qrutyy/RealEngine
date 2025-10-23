#include "app.h"
#include "asset_manager.h"
#include "log.h"
#include "err.h"
#include <stdlib.h>
#include <SDL3/SDL.h>

#define HASHTABLE_MAX_SIZE 1000
#define MAX_KEY_LEN 64

static struct asset {
    char *key;
    SDL_Surface *img;
};
typedef struct asset asset_h;

static asset_h *create_asset(char *key, SDL_Surface *img) {
    asset_h *asset = malloc(sizeof(asset_h));

    asset->img = img;
    asset->key = key;

    return asset;
}

static void destroy_asset(asset_h *asset) {
    if (asset != NULL) free(asset);
}

static asset_h *ht_assets[HASHTABLE_MAX_SIZE];

// Hash function invented by Daniel J. Bernstein
static unsigned long hash_djb2(char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

static asset_h *hashtable_get(char *key) {
    unsigned int hash = hash_djb2(key);

    asset_h *asset = ht_assets[hash % HASHTABLE_MAX_SIZE];

    return asset;
}

static asset_h *hashtable_add(char *key, SDL_Surface *img) {
    asset_h *asset = hashtable_get(key);
    if (asset) {
        return asset;
    }

    unsigned int hash = hash_djb2(key);

    asset = create_asset(key, img);
    if (!asset) {
        log_error("Failed to create asset for image with key %s\n", key);
        return NULL;
    }
    ht_assets[hash % HASHTABLE_MAX_SIZE] = asset;

    return asset;
}

char *RE_load_asset(char *filename, int src_x, int src_y, int width, int height) {
    char asset_keya[MAX_KEY_LEN];
    snprintf(asset_keya, MAX_KEY_LEN, "%s,%d,%d,%d,%d", filename, src_x, src_y, width, height);

    asset_h *asset = hashtable_get(asset_keya);
    if (asset) {
        return asset->key;
    }

    asset_h *full = hashtable_get(filename);
    if (!full) {
        SDL_Surface *full_image = SDL_LoadPNG(filename);
        if (!full_image) {
            log_error("Asset %s not found.\n", filename);
            return NULL;
        }
        full = hashtable_add(filename, full_image);
        if (!full) {
            return NULL;
        }
    }

    char *asset_key = malloc(MAX_KEY_LEN);
        if (!asset_key) {
        log_error("Failed to allocate memory for asset %s key.\n", filename);
        return NULL;
    }

    SDL_Surface *image;
    image = SDL_CreateSurface(width, height, full->img->format);
    if (!image) {
        log_error("Failed to create asset of width %d and height %d.\n", width, height);
        free(asset_key);
        return NULL;
    }

    const SDL_Rect rect = {src_x, src_y, width, height};
    int err = SDL_BlitSurface(full->img, &rect, image, NULL);
    if (err) {
        log_error("Failed to extract asset from image %s with specified parameters.\n", filename);
        SDL_DestroySurface(image);
        free(asset_key);
        return NULL;
    }

    asset = hashtable_add(asset_key, image);

    return asset->key;
}

SDL_Surface *RE_get_asset(char *key) {
    asset_h *asset = hashtable_get(key);
    if (asset) return asset->img;
    return NULL;
}
