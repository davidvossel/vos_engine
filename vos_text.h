#ifndef VOS_TEXT
#define VOS_TEXT
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "vos_screen.h"
#include "vos_font.h"

class vos_text {
	private:
		SDL_Surface *text;
		SDL_Surface *scaled_text;
	public:
		vos_text(vos_font *font, const char *text, int red = 255, int green = 255, int blue = 255);
		~vos_text();
		int draw(int x, int y, vos_screen *dst, int width_scale_ratio = 1, int height_scale_ratio = 1);
};

#endif
