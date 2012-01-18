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

void vos_media_engine::handle_event(SDL_Event *event)
{
	screen->handle_event(event);
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
