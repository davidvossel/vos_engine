#include "vos_screen.h"
#include <iostream>

vos_screen::vos_screen(const char *caption, int w, int h, int b)
{
	windowed = 1;
	width = w;
	height = h;
	bits = b;
	screen = SDL_SetVideoMode(width,
		height,
		bits,
		SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);

	if (!screen) {
		std::cout << "SCREEN INIT FAILED.\n";
	}

	if (caption) {
		SDL_WM_SetCaption(caption, NULL);
	}
}

vos_screen::~vos_screen()
{
	SDL_FreeSurface(screen);
}

int vos_screen::flip()
{
	int res;
	if (!screen) {
		std::cout << "NO SCREEN FOUND TO FLIP!\n";
	}
	res = SDL_Flip(screen);

	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00));
	return res;
}

SDL_Surface *vos_screen::get_sdl_screen()
{
	return screen;
}

int vos_screen::make_full_screen()
{
	if (windowed) {
		windowed = 0;
		screen = SDL_SetVideoMode(width,
			height,
			bits,
			SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	}
	return 0;
}

int vos_screen::make_windowed()
{
	if (!windowed) {
		windowed = 1;
		screen = SDL_SetVideoMode(width,
			height,
			bits,
			SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	}
	return 0;
}

int vos_screen::resize_screen(int x, int y)
{
	screen = SDL_SetVideoMode(
		x,
		y,
		bits,
		SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
	return 0;
}

float vos_screen::width_scale_ratio()
{
	return (float) screen->w / width;
}

float vos_screen::height_scale_ratio()
{
	return (float) screen->h / height;
}

int vos_screen::update_caption(const char *text)
{
	if (text) {
		SDL_WM_SetCaption(text, NULL);
		return 0;
	}
	return -1;
}

