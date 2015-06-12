#ifndef VOS_GRAPHICS
#define VOS_GRAPHICS
#include <map>
#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "vos_screen.h"
#include "vos_image.h"
#include "vos_text.h"
#include "vos_font.h"
#include "vos_sound.h"

using namespace std;

class vos_media_engine {
	private:
		map <int, vos_image *> images;
		map <int, vos_text *> texts;
		map <int, vos_font *> fonts;
		map <int, vos_sound *> sounds;
		map <int, vos_sound *> music;
		vos_screen *screen;
		int id_gen;

		int music_helper(int id,
			int play,
			int stop,
			int pause,
			int resume);
	public:
		vos_media_engine(const char *caption = NULL,
			int w = 640,
			int h = 480,
			int b = 24,
			int sample_rate = 22050);
		~vos_media_engine();
		int update_caption(const char *text);
		int flip();

		int make_full_screen();
		int make_windowed();
		int resize_screen(int x, int y);

		int add_font(const char *font_name,
			int size,
			int id = 0);

		int add_image(const char *file_name, int id = 0);
		int draw_image(int id, int x, int y);
		int image_height(int id);
		int image_width(int id);

		int create_text(const char *text,
			int font_id,
			int red = 255,
			int green = 255,
			int blue = 255);
		int free_text(int font_id);
		int draw_text(int id, int x, int y);

		int add_sound(const char *sound_file, int id = 0);
		int play_sound(int id);

		int add_music(const char *music_file, int id = 0);
		int play_music(int id);
		int stop_music(int id);
		int pause_music(int id);
		int resume_music(int id);
};

#endif
