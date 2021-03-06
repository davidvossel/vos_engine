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

class player: public vos_map_bound_object {
	private:
		unsigned int hitid;
	public:
		player(int x, int y, struct vos_map_object_data *data) : vos_map_bound_object(x, y, data) {
			hitid = c_engine->register_rect(vos_map_object_collision_cb, COLLISION_CAT_PLAYER1, this, x, y, 100, 100);
		}
		~player() {
			c_engine->unregister_rect(hitid);
		}

		int update() {
			int dist = calc_dist(ticks_diff, 200);
			int disty = 0;
			int distx = 0;
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_DOWN)) {
				disty += dist;
			}
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_UP)) {
				disty -= dist;
			}
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_LEFT)) {
				distx -= dist;
			}
			if (controllers->is_button_active(PLAYER1_CONTROLLER, VOS_CON_RIGHT)) {
				distx  += dist;
			}

			update_coordinates(distx, disty);

			return 0;
		}
		int render() {
			map->center_camera_on(get_cam_x(), get_cam_y());
			m_engine->draw_image(BLUE_DOT, get_converted_x(), get_converted_y());
			c_engine->update_rect_coordinates(hitid, get_hit_x(), get_hit_y());

			return 0;
		}
};

class thing: public vos_map_object {
	private:
		int hitid;
	public:
		thing(int x, int y, struct vos_map_object_data *data) : vos_map_object(x, y, data) {
		hitid = c_engine->register_rect(vos_map_object_collision_cb, COLLISION_CAT_THING, this, x, y, 3, 3);
		}
		~thing() {
			c_engine->unregister_rect(hitid);
		}

		int update() {
			if (am_i_hit_by(hitid, COLLISION_CAT_PLAYER1)) {
				ready_for_deletion = 1;
			}
			return 0;
		}
		int render() {
			int camx = map->map2cam_x(x);
			int camy = map->map2cam_y(y);
			m_engine->draw_image(RED_DOT, camx, camy);
			c_engine->update_rect_coordinates(hitid, camx, camy);

			return 0;
		}
};

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

int main(int argc, char* args[])
{
	SDL_Event event;
	vos_media_engine *m_engine = new vos_media_engine(NULL, X_RES, Y_RES);
	vos_collision_engine *c_engine = new vos_collision_engine(X_RES, Y_RES);
	vos_controller_engine *controllers = new vos_controller_engine();
	vos_map *map = new vos_map(X_MAP, Y_MAP, X_RES, Y_RES);
	struct vos_map_object_data data;
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

	map->add_object(new player(200, 200, &data));

	for (i = 0; i < 100; i++) {
		map->add_object(new thing(i*40, 100, &data));
	}

	map->add_object(new vos_map_boundary(GREEN_BLOCK, 0, 0, &data));
	map->add_object(new vos_map_boundary(GREEN_BLOCK, 39,0, &data));
	map->add_object(new vos_map_boundary(GREEN_BLOCK, 39,39, &data));
	map->add_object(new vos_map_boundary(GREEN_BLOCK, 79,39, &data));
	map->add_object(new vos_map_boundary(GREEN_BLOCK, 119,39, &data));
	map->add_object(new vos_map_boundary(GREEN_BLOCK, 159,39, &data));

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
