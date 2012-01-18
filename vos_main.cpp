#include "SDL/SDL.h"
#include <string>
#include "vos_media_engine.h"
#include "vos_collision_engine.h"
#include "vos_image.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int RED_DOT;
int BLUE_DOT;
int FONT_ID;
int HI_SOUND;
int MED_SOUND;
int LOW_SOUND;

int BEAT_MUSIC;

#define X_RES 640
#define Y_RES 480

//The dot
class Dot
{
    private:
    float x, y;
	unsigned int hitid;
	int hit_timeout;
	int x_pix_sec;
	int y_pix_sec;
	int x_dir;
	int y_dir;

	vos_collision_engine *c_engine;
    public:
	int ishit;
    Dot(vos_collision_engine *_c_engine);

    void render(vos_media_engine *m_engine, int time_lapse);
};

int collision_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata)
{
	Dot *dot = (Dot *) userdata;
	dot->ishit = 1;

	return 0;
}

Dot::Dot(vos_collision_engine *_c_engine)
{
	x = 1;//rand() % X_RES;
	y = 1;//rand() % Y_RES;
	ishit = 0;
	x_pix_sec = (rand() % 132) + 51;
	y_pix_sec = (rand() % 127) + 49;
	x_dir = 1;
	y_dir = 1;
	c_engine = _c_engine;
	hit_timeout = 0;
	hitid = c_engine->register_rect(collision_cb, 1, this, x, y, 3, 3);
}

void Dot::render(vos_media_engine *m_engine, int time_lapse)
{
	float x_dist = 0;
	float y_dist = 0;

	if (time_lapse != 0) {
		x_dist = ((float)time_lapse / 1000) * x_pix_sec * x_dir;
		y_dist = ((float)time_lapse / 1000) * y_pix_sec * y_dir;
	}
	if (ishit) {
//		m_engine->play_sound(HI_SOUND);
		m_engine->play_sound(LOW_SOUND);
		hit_timeout = 100;
		ishit=0;
	}

	x += x_dist;
	y += y_dist;

	if (y > Y_RES) {
		y = Y_RES;
		y_dir *= -1;
	}
	if (y < 0) {
		y = 0;
		y_dir *= -1;
	}

	if (x > X_RES) {
		x = X_RES;
		x_dir *= -1;
	}
	if (x < 0) {
		x = 0;
		x_dir *= -1;
	}

	if (hit_timeout) {
		m_engine->draw_image(RED_DOT, x, y);
		hit_timeout -= time_lapse;
		if (hit_timeout < 0) {
			hit_timeout = 0;
		}
	} else {
		m_engine->draw_image(BLUE_DOT, x, y);
	}
	c_engine->update_rect_coordinates(hitid, x, y);
}

int handle_quit_events(SDL_Event *event)
{
	if ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_ESCAPE)) {
		return 1;
	}
	if (event->type == SDL_QUIT ) {
		return 1;
	}

	return 0;
}

#define NUM_DOTS 100

int main(int argc, char* args[])
{
	SDL_Event event;
	vos_media_engine *m_engine;
	vos_collision_engine c_engine(X_RES, Y_RES);
	Dot *dots[NUM_DOTS];
	int res = 1;
	int quit = 0;
	int last;
	int now;
	int diff;
	int i;

	int frames = 0;
	int frame_counter;
	int text_id = 0;

	srand(1231);
	for (i = 0; i < NUM_DOTS; i++) {
		dots[i] = new Dot(&c_engine);
	}


	if (!(m_engine = new vos_media_engine(NULL, X_RES, Y_RES))) {
		goto the_end;
	}

	RED_DOT = m_engine->add_image("red_dot.bmp");
	BLUE_DOT = m_engine->add_image("blue_dot.bmp");
	FONT_ID = m_engine->add_font("lazy.ttf", 28);
	HI_SOUND = m_engine->add_sound("high.wav");
	MED_SOUND = m_engine->add_sound("medium.wav");
	LOW_SOUND = m_engine->add_sound("low.wav");

	BEAT_MUSIC = m_engine->add_music("beat.wav");


	m_engine->play_music(BEAT_MUSIC);
	now = SDL_GetTicks();
	last = now;
	diff = 0;
	frame_counter = diff;
	// game loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			quit = handle_quit_events(&event);
			m_engine->handle_event(&event);
		}
		now = SDL_GetTicks();
		diff = now-last;
		frame_counter += diff;
		last = now;
		c_engine.run_collisions();

		for (i = 0; i < NUM_DOTS; i++) {
			dots[i]->render(m_engine, diff);
		}

		m_engine->flip();
		//Cap the frame rate
//		if (diff < (1000 / 60)) {
//			SDL_Delay((1000 / 60 ) - diff);
//		}
		frames++;
		if (frame_counter > 1000) {
			char buf[100] = { 0, };
			frame_counter = 0;
			sprintf(buf, "%d", frames);
			m_engine->free_text(text_id);
			text_id = m_engine->create_text(buf, FONT_ID);
			frames = 0;
		}
		m_engine->draw_text(text_id, 100, 100);
	}


	res = 0;
the_end:
	delete m_engine;
	return res;
}
