#ifndef VOS_SOUND
#define VOS_SOUND
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

class vos_sound {
	private:
		Mix_Chunk *sound;
		Mix_Music *music;
	public:
		vos_sound(const char *filename, int is_music = 0);
		~vos_sound();
		int play_sound();
		int stop_sound();
		int pause_sound();
		int resume_sound();
		int is_paused();
};

#endif
