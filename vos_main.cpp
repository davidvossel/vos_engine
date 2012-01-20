#include "SDL/SDL.h"
#include <string>
#include "vos_media_engine.h"
#include "vos_collision_engine.h"
#include "vos_map.h"
#include "vos_map_common.h"
#include "vos_image.h"
#include <cstdlib>
#include <iostream>

using namespace std;

#define X_MAP 2000
#define Y_MAP 2000

#define X_RES 720
#define Y_RES 480


int RED_DOT;
int BLUE_DOT;
int GREEN_BLOCK;

int FONT_ID;
int HI_SOUND;
int MED_SOUND;
int LOW_SOUND;

int BEAT_MUSIC;

int _camera_x = 0;
int _camera_y = 0;

int collision_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata);

class Dot: public vos_map_object {
    private:
	unsigned int hitid;
	int hit_timeout;
	int x_pix_sec;
	int y_pix_sec;
	int x_dir;
	int y_dir;
	int last_ticks;

    public:

		int ishit;
		Dot(int x, int y, vos_media_engine *m_engine, vos_collision_engine *c_engine) :
			vos_map_object(x, y, m_engine, c_engine)
	{
		ishit = 0;
		x_pix_sec = ((unsigned int) rand() % 50) + 111;
		y_pix_sec = ((unsigned int) rand() % 50) + 111;
		x_dir = 1;
		y_dir = 1;
		hit_timeout = 0;
		last_ticks = 0;
		hitid = c_engine->register_rect(collision_cb, 1, this, -1, -1, 3, 3);
	}

    int update();
    int render(int camera_x, int camera_y, int ticks);
};

int collision_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata)
{
	Dot *dot = (Dot *) userdata;
	dot->ishit = 1;

	return 0;
}

int Dot::update()
{
	return 0;
}

int Dot::render(int camera_x, int camera_y, int ticks)
{
	float x_dist = 0;
	float y_dist = 0;
	int time_lapse = ticks- last_ticks;
	int camx,camy;

	if (last_ticks == 0) {
		last_ticks = ticks;
		return 0;
	}
	last_ticks = ticks;


	x_dist = calc_dist(time_lapse, x_pix_sec*x_dir);
	y_dist = calc_dist(time_lapse, y_pix_sec*y_dir);

	if (ishit) {
		m_engine->play_sound(LOW_SOUND);
		hit_timeout = 100;
		ishit=0;
	}

	x += x_dist;
	y += y_dist;

	if (y > Y_MAP) {
		y = Y_MAP;
		y_dir *= -1;
	}
	if (y < 0) {
		y = 0;
		y_dir *= -1;
	}

	if (x > X_MAP) {
		x = X_MAP;
		x_dir *= -1;
	}
	if (x < 0) {
		x = 0;
		x_dir *= -1;
	}

	camx = map2cam_x(x, camera_x);
	camy = map2cam_y(y, camera_y);

	if (hit_timeout) {
		m_engine->draw_image(RED_DOT, camx, camy);
		hit_timeout -= time_lapse;
		if (hit_timeout < 0) {
			hit_timeout = 0;
		}
	} else {
		m_engine->draw_image(BLUE_DOT, camx, camy);
	}
	c_engine->update_rect_coordinates(hitid, camx, camy);


	return 0;
}

int handle_events(SDL_Event *event)
{
	if ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_ESCAPE)) {
		return 1;
	}
	if (event->type == SDL_QUIT ) {
		return 1;
	}

	if ( event->type == SDL_KEYDOWN ) {
		switch( (int) event->key.keysym.sym ) {
		case SDLK_UP:
			_camera_y -= 50;
			break;
		case SDLK_DOWN:
			_camera_y += 50;
			break;
		case SDLK_LEFT:
			_camera_x -= 50;
			break;
		case SDLK_RIGHT:
			_camera_x += 50;
			break;
		}
		cout << "cam x: " << _camera_x << " cam y " << _camera_y << "\n";
	}

	return 0;
}

#define NUM_DOTS 500

int main(int argc, char* args[])
{
	SDL_Event event;
	vos_media_engine *m_engine = new vos_media_engine(NULL, X_RES, Y_RES);
	vos_collision_engine c_engine(X_RES, Y_RES);
	vos_map *map;
	Dot *dot;
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

	RED_DOT = m_engine->add_image("red_dot.bmp");
	BLUE_DOT = m_engine->add_image("blue_dot.bmp");
	GREEN_BLOCK = m_engine->add_image("green_block.bmp");
	FONT_ID = m_engine->add_font("lazy.ttf", 28);
	HI_SOUND = m_engine->add_sound("high.wav");
	MED_SOUND = m_engine->add_sound("medium.wav");
	LOW_SOUND = m_engine->add_sound("low.wav");
	BEAT_MUSIC = m_engine->add_music("beat.wav");

	map = new vos_map(X_MAP, Y_MAP, X_RES, Y_RES);

	for (i = 0; i < NUM_DOTS; i++) {
		dot = new Dot(1, 1, m_engine, &c_engine);
		map->add_object(dot);
	}
	for (i = 0; i < X_MAP; i+=40) {
		map->add_object(new vos_map_block(GREEN_BLOCK, i, 0, m_engine, &c_engine));
	}

	now = SDL_GetTicks();
	last = now;
	diff = 0;
	frame_counter = diff;
	// game loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			quit = handle_events(&event);
		    if (event.type == SDL_VIDEORESIZE) {
				m_engine->resize_screen(event.resize.w, event.resize.h);
		    }
		}

		now = SDL_GetTicks();
		diff = now-last;
		frame_counter += diff;
		last = now;
		c_engine.run_collisions();

		map->update_camera(_camera_x, _camera_y);
		map->render();
		m_engine->flip();
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

	delete map;
	res = 0;
	return res;
}
