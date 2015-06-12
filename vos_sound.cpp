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
