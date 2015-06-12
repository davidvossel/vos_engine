#ifndef VOS_FONT
#define VOS_FONT
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "vos_screen.h"

class vos_font {
	private:
		TTF_Font *font;
	public:
		vos_font(const char *font_name, int size);
		~vos_font();
		TTF_Font *get_font();
};
#endif
