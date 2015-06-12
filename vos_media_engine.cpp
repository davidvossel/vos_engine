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

#include "vos_media_engine.h"

vos_media_engine::vos_media_engine(const char *caption, int w, int h, int b, int sample_rate)
{
	SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512);
	screen = new vos_screen(caption, w, h, b);
}

vos_media_engine::~vos_media_engine()
{
	if (screen) {
		delete screen;
	}
	for (map <int, vos_image *>::iterator ii=images.begin(); ii!=images.end(); ++ii) {
		delete (*ii).second;
	}
    Mix_CloseAudio();
    TTF_Quit();
	SDL_Quit();
}

int vos_media_engine::make_full_screen()
{
	return screen->make_full_screen();
}

int vos_media_engine::make_windowed()
{
	return screen->make_windowed();
}

int vos_media_engine::resize_screen(int x, int y)
{
	return screen->resize_screen(x, y);
}

int vos_media_engine::flip()
{
	return screen->flip();
}

int vos_media_engine::add_font(const char *font_name, int size, int id)
{
	vos_font *font = new vos_font(font_name, size);

	if (!font) {
		return -1;
	}

	if (!id) {
		id_gen++;
		id = id_gen;
	}
	fonts[id] = font;

	return id;
}

int vos_media_engine::add_image(const char *file_name, int id)
{
	vos_image *image = new vos_image(file_name);

	if (!image) {
		return -1;
	}

	if (!id) {
		id_gen++;
		id = id_gen;
	}
	images[id] = image;

	return id;
}

int vos_media_engine::add_sound(const char *file_name, int id)
{
	vos_sound *sound = new vos_sound(file_name);

	if (!sound) {
		return -1;
	}

	if (!id) {
		id_gen++;
		id = id_gen;
	}
	sounds[id] = sound;

	return id;
}

int vos_media_engine::play_sound(int id)
{
	vos_sound *sound = NULL;
	map <int, vos_sound *>::iterator ii = sounds.find(id);

	if (ii == sounds.end()) {
		return -1;
	}

	sound = (*ii).second;
	return sound->play_sound();
}

int vos_media_engine::add_music(const char *file_name, int id)
{
	vos_sound *sound = new vos_sound(file_name, 1);

	if (!sound) {
		return -1;
	}

	if (!id) {
		id_gen++;
		id = id_gen;
	}
	music[id] = sound;

	return id;
}

int vos_media_engine::music_helper(int id, int play, int stop, int pause, int resume)
{
	vos_sound *sound = NULL;
	map <int, vos_sound *>::iterator ii = music.find(id);

	if (ii == music.end()) {
		return -1;
	}

	sound = (*ii).second;
	if (play) {
		sound->play_sound();
	}
	if (stop) {
		sound->stop_sound();
	}
	if (pause) {
		sound->pause_sound();
	}
	if (resume) {
		sound->resume_sound();
	}

	return 0;
}

int vos_media_engine::play_music(int id)
{
	return music_helper(id, 1, 0, 0, 0);
}

int vos_media_engine::stop_music(int id)
{
	return music_helper(id, 0, 1, 0, 0);
}
int vos_media_engine::pause_music(int id)
{
	return music_helper(id, 0, 0, 1, 0);
}
int vos_media_engine::resume_music(int id)
{
	return music_helper(id, 0, 0, 0, 1);
}
int vos_media_engine::create_text(const char *_text, int font_id, int red, int green, int blue)
{
	vos_text *text = NULL;
	vos_font *font = NULL;
	map <int, vos_font *>::iterator ii = fonts.find(font_id);
	int id;

	if (ii == fonts.end()) {
		return -1;
	}

	font = (*ii).second;
	text = new vos_text(font, _text, red, green, blue);

	if (!text) {
		return -1;
	}

	if (!id) {
		id_gen++;
		id = id_gen;
	}
	texts[id] = text;

	return id;
}

int vos_media_engine::free_text(int id)
{
	vos_text *text;
	map <int, vos_text *>::iterator ii = texts.find(id);

	if (ii == texts.end()) {
		return -1;
	}

	text = (*ii).second;
	texts.erase(id);

	delete text;
	return 0;
}

int vos_media_engine::draw_text(int id, int x, int y)
{
	vos_text *text;
	map <int, vos_text *>::iterator ii = texts.find(id);

	if (ii == texts.end()) {
		return -1;
	}

	text = (*ii).second;
	if (text) {
		text->draw(x, y, screen);
	}

	return 0;
}

int vos_media_engine::image_width(int id)
{
	vos_image *image;
	map <int, vos_image *>::iterator ii = images.find(id);

	if (ii == images.end()) {
		return 0;
	}

	image = (*ii).second;
	return image->get_width();
}

int vos_media_engine::image_height(int id)
{
	vos_image *image;
	map <int, vos_image *>::iterator ii = images.find(id);

	if (ii == images.end()) {
		return 0;
	}

	image = (*ii).second;
	return image->get_height();
}

int vos_media_engine::draw_image(int id, int x, int y)
{
	vos_image *image;
	map <int, vos_image *>::iterator ii = images.find(id);

	if (ii == images.end()) {
		return -1;
	}

	image = (*ii).second;
	if (image) {
		image->draw(x, y, screen);
	}

	return 0;
}

int vos_media_engine::update_caption(const char *text)
{
	return screen->update_caption(text);
}
