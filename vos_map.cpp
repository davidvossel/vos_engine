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

#include "vos_map.h"
#include "SDL/SDL.h"

vos_map_object::vos_map_object(int _x, int _y, struct vos_map_object_data *data)
{
	ticks_diff = ticks = 0;
	m_engine = data->m_engine;
	c_engine = data->c_engine;
	map = data->map;
	controllers = data->controllers;
	x = _x;
	y = _y;
	ready_for_deletion = 0;
	id = 0;
	memset(&collisions_list, 0, sizeof(collisions_list));
	num_collisions = 0;
}

int vos_map_object::get_x()
{
	return x;
}
int vos_map_object::get_y()
{
	return y;
}

int vos_map_object::update_ticks(int _ticks)
{
	if (ticks == 0) {
		ticks_diff = 0;
		ticks = _ticks;
	} else {
		ticks_diff = _ticks - ticks;
		ticks = _ticks;
	}

	/* if the time is lagged half a second, resync. */
	if (ticks_diff > 500) {
		ticks_diff = 0;
	}
	return 0;
}

int vos_map_object::delete_me()
{
	return ready_for_deletion;
}
int vos_map_object::set_id(int _id)
{
	id = _id;
	return id;
}
int vos_map_object::get_id()
{
	return id;
}

int vos_map_object::am_i_hit_by(unsigned int myid, unsigned int hitcat) {
	int i = 0;
	if (!num_collisions) {
		return 0;
	}
	for (i = 0; i < num_collisions; i++) {
		if ((collisions_list[i].data.myid == myid) && (collisions_list[i].data.hitcat == hitcat)) {
			return 1;
		}
	}
	return 0;
}

int vos_map_object::add_collision(struct vos_collision_engine_cb_data *data) {
	if (num_collisions < COLLISION_LIST_SIZE) {
		// TODO this is bad, I am fixing the symtom of a problem
		for (int i = 0; i < num_collisions; i++) {
			if (collisions_list[i].data.hitid == data->hitid) {
				return 0;
			}
		}

		memcpy(&collisions_list[num_collisions].data, data, sizeof(collisions_list[num_collisions].data));
		num_collisions++;
	}
	return 0;
}
void vos_map_object::remove_collisions() {
	if (num_collisions) {
		memset(&collisions_list, 0, sizeof(collisions_list));
		num_collisions = 0;
	}
}

int vos_map_object::calc_dist(int ticks, int pps) {
	if (ticks) {
		return (((float)ticks / 1000) * pps);
	} else {
		return 0;
	}
}

int vos_map_object_collision_cb(struct vos_collision_engine_cb_data *data)
{
	vos_map_object *obj = (vos_map_object *) data->userdata;
	obj->add_collision(data);
	return 0;
}

vos_map::vos_map(int _w, int _h, int _camera_w, int _camera_h)
{
	w = _w;
	h = _h;

	camera_w = _camera_w;
	camera_h = _camera_h;

	new_camera_x = camera_x = 0;
	new_camera_y = camera_y = 0;

	gen_id = 0;
}

vos_map::~vos_map()
{

}
int vos_map::map2cam_x(int map_x)
{
	return map_x-camera_x;
}

int vos_map::map2cam_y(int map_y)
{
	return map_y-camera_y;
}

int vos_map::center_camera_on(int x, int y)
{
	new_camera_x = x - (camera_w/2);
	new_camera_y = y - (camera_h/2);
	return 0;
}

int vos_map::update_camera(int x, int y)
{
	new_camera_x = x;
	new_camera_y = y;
	return 0;
}

int vos_map::x_to_x_chunk_index(int x)
{
	if (x >= camera_w) {
		return x / camera_w;
	}
	return 0;
}

int vos_map::y_to_y_chunk_index(int y)
{
	if (y >= camera_h) {
		return y / camera_h;
	}
	return 0;
}

int vos_map::put_object_in_chunk(vos_map_object *obj)
{
	int x_index = x_to_x_chunk_index(obj->get_x());
	int y_index = y_to_y_chunk_index(obj->get_y());

	if (obj->get_y() >= camera_h) {
		y_index = obj->get_y() / camera_h;
	}

	if (x_index >= MAX_X_CHUNKS) {
		return -1;
	}
	if (y_index >= MAX_Y_CHUNKS) {
		return -1;
	}

	chunk_map[x_index][y_index].insert(pair <unsigned int, vos_map_object *> (obj->get_id(), obj));

	return 0;
}

int vos_map::remove_object_from_chunk(vos_map_object *obj)
{
	int x_index = x_to_x_chunk_index(obj->get_x());
	int y_index = y_to_y_chunk_index(obj->get_y());

	if (x_index >= MAX_X_CHUNKS) {
		return -1;
	}
	if (y_index >= MAX_Y_CHUNKS) {
		return -1;
	}

	chunk_map[x_index][y_index].erase(obj->get_id());

	return 0;
}

int vos_map::remove_and_delete_object(vos_map_object *obj)
{
	remove_object_from_chunk(obj);
	obj_map.erase(obj->get_id());
	delete obj;
	return 0;
}

int vos_map::add_object(vos_map_object *obj)
{
	gen_id++;

	obj->set_id(gen_id);
	obj_map[gen_id] = obj;

	put_object_in_chunk(obj);
	return 0;
}

int vos_map::render_chunk(int x_index, int y_index)
{
	map <unsigned int, vos_map_object *> *tmp;
	vos_map_object *obj;
	int ticks = SDL_GetTicks();

	if (x_index >= MAX_X_CHUNKS) {
		return -1;
	}
	if (y_index >= MAX_Y_CHUNKS) {
		return -1;
	}
	tmp = &chunk_map[x_index][y_index];

	for (map_ii = tmp->begin(); map_ii!=tmp->end(); ++map_ii) {
		obj = (*map_ii).second;

		if (obj->delete_me()) {
			delete_list.push_back(obj);
			continue;
		}

		obj->update_ticks(ticks);
		obj->update();
		obj->render();
		obj->remove_collisions();
		if ((x_index != x_to_x_chunk_index(obj->get_x())) || (y_index != y_to_y_chunk_index(obj->get_y()))) {
			tmp->erase(obj->get_id());
			update_location_list.push_back(obj);
		}
	}

	return 0;
}

int vos_map::render()
{
	int x_index = x_to_x_chunk_index(camera_x);
	int y_index = y_to_y_chunk_index(camera_y);

	/*
	 * We are dealing with 4 chunks here
	 * (x, y), (x+1, y), (x, y+1), (x+1, y+1)
	 */

	camera_x = new_camera_x;
	camera_y = new_camera_y;

	update_location_list.clear();
	delete_list.clear();

	render_chunk(x_index, y_index);
	render_chunk(x_index+1, y_index);
	render_chunk(x_index, y_index+1);
	render_chunk(x_index+1 , y_index+1);

	for (list_ii = update_location_list.begin();
		list_ii != update_location_list.end();
		++list_ii) {
		put_object_in_chunk(*list_ii);
	}

	for (list_ii = delete_list.begin();
		list_ii != delete_list.end();
		++list_ii) {
		remove_and_delete_object(*list_ii);
	}



	return 0;
}
