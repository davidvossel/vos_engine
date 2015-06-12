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
		int get_height();
		int get_width();
		int draw(int x, int y, vos_screen *dst, int width_scale_ratio = 1, int height_scale_ratio = 1);
};

#endif
