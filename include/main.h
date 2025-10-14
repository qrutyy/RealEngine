#ifndef MAIN_H
#define MAIN_H


struct rend_hndl {
	struct SDL_Renderer *renderer;
	struct SDL_Window *window;
	
	struct window_dim {
		int width;
		int height;
	} dim;

	char debug_string[32];
};


#endif
