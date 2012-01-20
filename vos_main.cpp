#include "SDL/SDL.h"
#include <string>
#include "vos_media_engine.h"
#include "vos_collision_engine.h"
#include "vos_controller_engine.h"
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

int COLLISION_CAT_PLAYER1 = 10;
int COLLISION_CAT_THING = 11;

unsigned int PLAYER1_CONTROLLER;

int _camera_x = 0;
int _camera_y = 0;

int player_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata);
class player: public vos_map_object {
	private:
		int hitid;
		int last_tick;
	public:
		int hit;
		player(int x, int y, struct vos_map_object_data *data) : vos_map_object(x, y, data) {
			hit = 0;
			hitid = c_engine->register_rect(player_cb, COLLISION_CAT_PLAYER1, this, x, y, 3, 3);
		}
		~player() {
			c_engine->unregister_rect(hitid);
		}

		int update(int ticks) {
			int time_lapse = ticks - last_tick;
			int dist = calc_dist(time_lapse, 200);
			last_tick = ticks;

			if (hit) {
				hit = 0;
			}

			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_DOWN)) {
				y += dist;
			}
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_UP)) {
				y -= dist;
			}
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_LEFT)) {
				x -= dist;
			}
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_RIGHT)) {
				x += dist;
			}

			map->center_camera_on(x, y);
			return 0;
		}
		int render(int ticks) {
			int camx = map2cam_x(x, map->get_camera_x());
			int camy = map2cam_y(y, map->get_camera_y());
			m_engine->draw_image(BLUE_DOT, camx, camy);
			c_engine->update_rect_coordinates(hitid, camx, camy);

			return 0;
		}
};
int player_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata)
{
	player *p = (player *) userdata;
	p->hit = 1;

	return 0;
}

int thing_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata);
class thing: public vos_map_object {
	private:
		int hitid;
	public:
		int hit;
		thing(int x, int y, struct vos_map_object_data *data) : vos_map_object(x, y, data) {
			hit = 0;
			hitid = c_engine->register_rect(thing_cb, 1, this, x, y, 3, 3);
		}
		~thing() {
			c_engine->unregister_rect(hitid);
		}

		int update(int ticks) {
			if (hit) {
				ready_for_deletion = 1;
			}
			return 0;
		}
		int render(int ticks) {
			int camx = map2cam_x(x, map->get_camera_x());
			int camy = map2cam_y(y, map->get_camera_y());
			m_engine->draw_image(RED_DOT, camx, camy);
			c_engine->update_rect_coordinates(hitid, camx, camy);

			return 0;
		}
};
int thing_cb(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata)
{
	thing *t = (thing *) userdata;
	t->hit = 1;

	return 0;
}


#if 0
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
		Dot(int x, int y, struct vos_map_object_data *data) :
			vos_map_object(x, y, data)
	{
		ishit = 0;
		x_pix_sec = ((unsigned int) rand() % 50) + 111;
		y_pix_sec = ((unsigned int) rand() % 50) + 111;
		x_dir = 1;
		y_dir = 1;
		hit_timeout = 0;
		last_ticks = 0;
		hitid = c_engine->register_rect(player_cb, 1, this, -1, -1, 3, 3);
	}

    int update(int ticks) { return 0; }
    int render(int camera_x, int camera_y, int ticks);
};



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
#endif

int handle_events(SDL_Event *event)
{
	if ((event->type == SDL_KEYDOWN) && (event->key.keysym.sym == SDLK_ESCAPE)) {
		return 1;
	}
	if (event->type == SDL_QUIT ) {
		return 1;
	}

	return 0;
}

#define NUM_DOTS 500

int main(int argc, char* args[])
{
	SDL_Event event;
	vos_media_engine *m_engine = new vos_media_engine(NULL, X_RES, Y_RES);
	vos_collision_engine *c_engine = new vos_collision_engine(X_RES, Y_RES);
	vos_controller_engine *controllers = new vos_controller_engine();
	vos_map *map = new vos_map(X_MAP, Y_MAP, X_RES, Y_RES);
	struct vos_map_object_data data;
//	Dot *dot;
	int res = 1;
	int quit = 0;
	int last;
	int now;
	int diff;
	int i;
	int frames = 0;
	int frame_counter;
	int text_id = 0;

	data.c_engine = c_engine;
	data.m_engine = m_engine;
	data.controllers = controllers;
	data.map = map;

	srand(1231);

	RED_DOT = m_engine->add_image("red_dot.bmp");
	BLUE_DOT = m_engine->add_image("blue_dot.bmp");
	GREEN_BLOCK = m_engine->add_image("green_block.bmp");
	FONT_ID = m_engine->add_font("lazy.ttf", 28);
	HI_SOUND = m_engine->add_sound("high.wav");
	MED_SOUND = m_engine->add_sound("medium.wav");
	LOW_SOUND = m_engine->add_sound("low.wav");
	BEAT_MUSIC = m_engine->add_music("beat.wav");

	PLAYER1_CONTROLLER = controllers->new_controller();
	controllers->set_button_value(PLAYER1_CONTROLLER, VOS_CON_UP, SDLK_UP);
	controllers->set_button_value(PLAYER1_CONTROLLER, VOS_CON_DOWN, SDLK_DOWN);
	controllers->set_button_value(PLAYER1_CONTROLLER, VOS_CON_LEFT, SDLK_LEFT);
	controllers->set_button_value(PLAYER1_CONTROLLER, VOS_CON_RIGHT, SDLK_RIGHT);

//	for (i = 0; i < NUM_DOTS; i++) {
//		dot = new Dot(1, 1, &data);
//		map->add_object(dot);
//	}

	map->add_object(new player(50, 50, &data));

	for (i = 0; i < 100; i++) {
		map->add_object(new thing(i*40, 0, &data));
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
			if (event.type == SDL_KEYDOWN) {
				controllers->process_event_key(event.key.keysym.sym, 1);
			}
			if (event.type == SDL_KEYUP) {
				controllers->process_event_key(event.key.keysym.sym, 0);
			}
		}

		now = SDL_GetTicks();
		diff = now-last;
		frame_counter += diff;
		last = now;
		c_engine->run_collisions();

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

	delete c_engine;
	delete m_engine;
	delete controllers;
	delete map;
	res = 0;
	return res;
}
