// The MIT License (MIT)
//
// Copyright (c) 2015 David Vossel
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
