#include "vos_sound.h"

vos_sound::vos_sound(const char *filename, int is_music)
{
	sound = NULL;
	music = NULL;
	if (is_music) {
		music = Mix_LoadMUS(filename);
	} else {
		sound = Mix_LoadWAV(filename);
	}

}

vos_sound::~vos_sound()
{
	if (music) {
		Mix_FreeMusic(music);
	}
	if (sound) {
		Mix_FreeChunk(sound);
	}
}

int vos_sound::play_sound()
{
	if (music) {
		if (Mix_PlayingMusic()) {
			Mix_HaltMusic();
		}
		Mix_PlayMusic(music, -1);
	} else {
		Mix_PlayChannel(-1, sound, 0);
	}
	return 0;
}

int vos_sound::stop_sound()
{
	if (music) {
		if (Mix_PlayingMusic()) {
			Mix_HaltMusic();
		}
	}
	return 0;
}

int vos_sound::pause_sound()
{
	if (music) {
		if (Mix_PausedMusic() == 0) {
			Mix_PauseMusic();
		}
	}
	return 0;
}

int vos_sound::resume_sound()
{
	if (music) {
		if (Mix_PausedMusic() == 1) {
			Mix_ResumeMusic();
		}
	}
	return 0;
}

int vos_sound::is_paused()
{
	if (music) {
		return Mix_PausedMusic();
	}
	return 0;
}
