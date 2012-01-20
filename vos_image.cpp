#include "vos_image.h"
#include "SDL/SDL_rotozoom.h"
#include <iostream>

vos_image::vos_image(const char *file_name) {
	SDL_Surface* loaded_img = IMG_Load(file_name);

	scaled_image = NULL;

	//Create an optimized surface
	image = SDL_DisplayFormat(loaded_img);

	colorkey = SDL_MapRGB(image->format, 0xFF, 0xFF, 0xFF);

	SDL_SetColorKey(image, SDL_SRCCOLORKEY, colorkey);

	//Free the old surface
	SDL_FreeSurface(loaded_img);

	strncpy(file, file_name, sizeof(file));
}

vos_image::~vos_image() {
	if (image) {
		SDL_FreeSurface(image);
	}
	if (scaled_image) {
		SDL_FreeSurface(scaled_image);
	}

}

SDL_Surface *vos_image::get_image()
{
	return image;
}

const char *vos_image::get_name()
{
	return file;
}

int vos_image::get_height()
{
	return image->h;
}

int vos_image::get_width()
{
	return image->w;
}

int vos_image::draw(int x, int y, vos_screen *dst, int width_scale_ratio, int height_scale_ratio)
{
    SDL_Rect offset = { 0, };
	SDL_Surface *sdl_dst = dst->get_sdl_screen();
	int w;
	int h;

	if (!sdl_dst) {
		return -1;
	}

    offset.x = x * dst->width_scale_ratio();
    offset.y = y * dst->height_scale_ratio();
	w = image->w * dst->width_scale_ratio() * width_scale_ratio;
	h = image->h * dst->height_scale_ratio() * height_scale_ratio;

	if ((w == image->w) && (h == image->h)) {
		return SDL_BlitSurface(image, NULL, sdl_dst, &offset);
	}

	if (scaled_image && scaled_image->w == w && scaled_image->h == h) {
		return SDL_BlitSurface(scaled_image, NULL, sdl_dst, &offset);
	}

	// great, we have to make a scaled image
	if (scaled_image) {
		SDL_FreeSurface(scaled_image);
	}
	if (!(scaled_image = zoomSurface(image, dst->width_scale_ratio(), dst->height_scale_ratio(), SMOOTHING_OFF))) {
		return -1;
	}
	SDL_SetColorKey(scaled_image, SDL_SRCCOLORKEY, colorkey);

	return SDL_BlitSurface(scaled_image, NULL, sdl_dst, &offset);

//    return SDL_SoftStretch(image, NULL, sdl_dst, &offset);
}
