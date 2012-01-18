#ifndef VOS_SCREEN
#define VOS_SCREEN
#include "SDL/SDL.h"

class vos_screen {
	private:
		SDL_Surface *screen;
		int windowed;
		int width;
		int height;
		int bits;
	public:
		vos_screen(const char *caption = NULL, int w = 640, int h = 480, int b = 24);
		~vos_screen();
		int flip();
		void handle_event(SDL_Event *event);
		SDL_Surface *get_sdl_screen();
		float width_scale_ratio();
		float height_scale_ratio();
		int update_caption(const char *text);
};

#endif
