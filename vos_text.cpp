#include "vos_text.h"
#include "SDL/SDL_rotozoom.h"
#include "SDL/SDL_image.h"

vos_text::vos_text(vos_font *font, const char *_text, int red, int green, int blue)
{
	SDL_Color color = { red, green, blue };
	text = TTF_RenderText_Solid(font->get_font(), _text, color);
	scaled_text = NULL;
}

vos_text::~vos_text()
{
	if (text) {
		SDL_FreeSurface(text);
	}
	if (scaled_text) {
		SDL_FreeSurface(scaled_text);
	}
}

int vos_text::draw(int x, int y, vos_screen *dst, int width_scale_ratio, int height_scale_ratio)
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
	w = text->w * dst->width_scale_ratio() * width_scale_ratio;
	h = text->h * dst->height_scale_ratio() * height_scale_ratio;

	if ((w == text->w) && (h == text->h)) {
		return SDL_BlitSurface(text, NULL, sdl_dst, &offset);
	}

	if (scaled_text && scaled_text->w == w && scaled_text->h == h) {
		return SDL_BlitSurface(scaled_text, NULL, sdl_dst, &offset);
	}

	// great, we have to make a scaled text
	if (scaled_text) {
		SDL_FreeSurface(scaled_text);
	}

	if (!(scaled_text = zoomSurface(text, dst->width_scale_ratio(), dst->height_scale_ratio(), SMOOTHING_OFF))) {
		return -1;
	}

	return SDL_BlitSurface(scaled_text, NULL, sdl_dst, &offset);
}
