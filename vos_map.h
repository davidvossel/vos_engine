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

#ifndef VOS_MAP
#define VOS_MAP
#include "vos_media_engine.h"
#include "vos_collision_engine.h"
#include "vos_controller_engine.h"
#include <iostream>
#include <map>
#include <list>

using namespace std;

#define MAX_X_CHUNKS 1000
#define MAX_Y_CHUNKS 1000

/*! max of 30 collision reports per frame on a single object */
#define COLLISION_LIST_SIZE 30

struct vos_map_object_data;
class vos_map;

/* use this callback to register collision rects
 * on a vos map object */
int vos_map_object_collision_cb(struct vos_collision_engine_cb_data *data);

class vos_map_object {
	protected:
		vos_media_engine *m_engine;
		vos_collision_engine *c_engine;
		vos_controller_engine *controllers;
		vos_map *map;
		int ready_for_deletion;
		int x;
		int y;
		int id;
		/* current ticks */
		int ticks;
		/* difference since last update */
		int ticks_diff;

		/* structure used for generic collision
		 * callback reporting on object */
		struct vos_map_object_collisions {
			vos_collision_engine_cb_data data;
		} collisions_list[COLLISION_LIST_SIZE];
		int num_collisions;

	public:
		vos_map_object(int x,
			int y,
			struct vos_map_object_data *data);
		virtual ~vos_map_object() {};

		int update_ticks(int ticks);
		/* get x_coordinate */
		int get_x();
		/* get y_coordinate */
		int get_y();
		/* calculate distance to travel for a given number
		 * of ticks and the rate (pixels per second) you
		 * wish to travel */
		int calc_dist(int ticks, int pps);
		/* update means update your AI logic or whatever */
		virtual int update() = 0;
		/* Render means that you're actually getting drawn on the screen */
		virtual int render() = 0;
		/* when this returns true, the object gets removed from chunk */
		int delete_me();
		/* did a collision rect i registered collid with anything
		 * in a specific hit catagory ? */
		int am_i_hit_by(unsigned int myid, unsigned int hitcat);
		/* returns id of object in map structure */
		int get_id();

		/* DON'T USE THE FOLLOWING OUTSIDE OF MAP CLASS */
		/* do not call directly, used by map */
		int set_id(int id);
		/* do not call directly, used by generic collision callback */
		int add_collision(struct vos_collision_engine_cb_data *data);
		/* do not call directly, called by map after rendering */
		void remove_collisions();
};

class vos_map {
	private:
		int w;
		int h;

		int camera_w;
		int camera_h;

		/* used when updating camera location
		 * actual camera change does not happen until
		 * the next rendering.  This keeps things from being
		 * weird */
		int new_camera_x;
		int new_camera_y;
		int camera_x;
		int camera_y;
		unsigned int gen_id;

		/* dealing with at most 4 chunks at a time. out of those 4
		 * render call render on all the objects in them. */
		map <unsigned int, vos_map_object *> chunk_map[MAX_X_CHUNKS][MAX_Y_CHUNKS];
		map <unsigned int, vos_map_object *> obj_map;
		map <unsigned int, vos_map_object *>::iterator map_ii;

		list <vos_map_object *> update_location_list;
		list <vos_map_object *> delete_list;
		list <vos_map_object *>::iterator list_ii;

		int render_chunk(int x_index, int y_index);
		int put_object_in_chunk(vos_map_object *obj);
		int remove_object_from_chunk(vos_map_object *obj);

		int x_to_x_chunk_index(int x);
		int y_to_y_chunk_index(int y);

		int remove_and_delete_object(vos_map_object *obj);
	public:
		vos_map(int w, int h, int camera_w, int camera_h);
		~vos_map();

		/* converts map coordinates to current camera
		 * coordinates. returns coordinate to render on
		 * graphics engine */
		int map2cam_x(int map_x);
		int map2cam_y(int map_y);

		int center_camera_on(int x, int y);
		int update_camera(int x, int y);
		int get_camera_x() { return camera_x; }
		int get_camera_y() { return camera_y; }
		int add_object(vos_map_object *obj);
		int render();
};

struct vos_map_object_data {
	vos_media_engine *m_engine;
	vos_collision_engine *c_engine;
	vos_controller_engine *controllers;
	vos_map *map;
};



#endif

