#ifndef VOS_IMAGE
#define VOS_IMAGE
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "vos_screen.h"

using namespace std;

class vos_image {
	private:
		SDL_Surface *image;
		SDL_Surface *scaled_image;
		int colorkey;
		char file[512];
	public:
		vos_image(const char *file_name);
		~vos_image();
		SDL_Surface *get_image();
		const char *get_name();
		int draw(int x, int y, vos_screen *dst, int width_scale_ratio = 1, int height_scale_ratio = 1);
};

#endif
