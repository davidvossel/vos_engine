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

#include "vos_collision_engine.h"
#include <iostream>
#include <stdlib.h>

vos_collision_engine::vos_collision_engine(int _x, int _y)
{
	int i;
	int k;

	int x_cur = 0;
	int y_cur = 0;

	int x_rows = HIT_BOXES / 4;
	int y_rows = 4;

	int w_box_len = (_x / x_rows) + 1;
	int h_box_len = (_y / 4) + 1;

	int section = 0;

	gen_id = 0;
	x = _x;
	y = _y;

	for (i = 0; i < x_rows; i++) {
		y_cur = 0;
		for (k = 0; k < y_rows; k++) {
			sections[section].x = x_cur;
			sections[section].y = y_cur;
			sections[section].w = w_box_len;
			sections[section].h = h_box_len;

			y_cur += h_box_len;
			section++;
		}
		x_cur += w_box_len;
	}
}

vos_collision_engine::~vos_collision_engine()
{
	for (map <unsigned int, struct vos_collision_engine_rect *>::iterator ii=rects.begin(); ii!=rects.end(); ++ii) {
		delete (*ii).second;
	}
}

unsigned int vos_collision_engine::register_rect(vos_collision_engine_cb cb,
	int hit_cat,
	void *userdata,
	int x, int y, int w, int h)
{
	struct vos_collision_engine_rect *rect;
	map <unsigned int, vos_collision_engine_rect *>::iterator ii;


	if (!(rect = new vos_collision_engine_rect())) {
		return -1;
	}

	gen_id++;
	ii = rects.find(gen_id);
	while (ii != rects.end()) {
		gen_id++;
		ii = rects.find(gen_id);
	}
	rect->id = gen_id;
	rect->cat = hit_cat;
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
	rect->cb = cb;
	rect->userdata = userdata;

	rects[rect->id] = rect;

	update_rect_map(rect);

	return gen_id;
}
int vos_collision_engine::update_rect_coordinates(unsigned int id, int x, int y)
{
	struct vos_collision_engine_rect *rect;
	map <unsigned int, vos_collision_engine_rect *>::iterator ii;

	ii = rects.find(id);
	if (ii == rects.end()) {
		return -1;
	}

	rect = (*ii).second;
	rect->x = x;
	rect->y = y;

	update_rect_map(rect);
	return 0;
}
int vos_collision_engine::update_rect_size(unsigned int id, int w, int h)
{
	struct vos_collision_engine_rect *rect;
	map <unsigned int, vos_collision_engine_rect *>::iterator ii;

	ii = rects.find(id);
	if (ii == rects.end()) {
		return -1;
	}

	rect = (*ii).second;
	rect->w = w;
	rect->h = h;

	update_rect_map(rect);
	return 0;
}
int vos_collision_engine::unregister_rect(unsigned int id)
{
	map <unsigned int, vos_collision_engine_rect *>::iterator ii;
	struct vos_collision_engine_rect *rect = NULL;

	ii = rects.find(id);

	if (ii == rects.end()) {
		return -1;
	}

	rect = (*ii).second;
	rects.erase(ii);

	for (int i = 0; i < HIT_BOXES; i++) {
		if (rect->in_maps[i]) {
			boxes[i].erase(rect->id);
		}
	}
	delete rect;

	return 0;
}

int vos_collision_engine::update_rect_map(struct vos_collision_engine_rect *rect)
{
	int i;

	for (i = 0; i < HIT_BOXES; i++) {
		if (detect_collision(&sections[i], rect, NULL)) {
			if (!rect->in_maps[i]) {
				boxes[i].insert(pair<unsigned int, struct vos_collision_engine_rect *> (rect->id, rect));
				rect->in_maps[i] = 1;
			}
		} else {
			if (rect->in_maps[i]) {
				boxes[i].erase(rect->id);
				rect->in_maps[i] = 0;
			}
		}
	}
	return 0;
}

int vos_collision_engine::detect_collision(
	struct vos_collision_engine_rect *rect_a,
	struct vos_collision_engine_rect *rect_b,
	struct vos_collision_engine_cb_data *data)
{
	int a_xl = rect_a->x;
	int a_xr = rect_a->x + rect_a->w;
	int a_yt = rect_a->y;
	int a_yb = rect_a->y + rect_a->h;

	int b_xl = rect_b->x;
	int b_xr = rect_b->x + rect_b->w;
	int b_yt = rect_b->y;
	int b_yb = rect_b->y + rect_b->h;

	if (a_xl >= b_xr) {
		return 0;
	} else if (data) {
		data->dist_right = b_xr - a_xl;
	}

	if (a_xr <= b_xl) {
		return 0;
	} else if (data) {
		data->dist_left = b_xl - a_xr; /* will be negative */
	}
	
	if (a_yt >= b_yb) {
		return 0;
	} else if (data) {
		data->dist_down = b_yb - a_yt;
	}

	if (a_yb <= b_yt) {
		return 0;
	} else if (data) {
		data->dist_up = b_yt - a_yb; /* will be negative */
	}

	if (!data) {
		return 1;
	}
	data->shortest_x = abs(data->dist_left) > data->dist_right ? data->dist_right : data->dist_left;
	data->shortest_y = abs(data->dist_up) > data->dist_down ? data->dist_down : data->dist_up;

	data->myid = rect_a->id;
	data->mycat = rect_a->cat;
	data->hitid = rect_b->id;
	data->hitcat = rect_b->cat;
	data->userdata = rect_a->userdata;

	return 1;
}

void vos_collision_engine::run_collisions()
{
	struct vos_collision_engine_rect *rect_a;
	struct vos_collision_engine_rect *rect_b;
	struct vos_collision_engine_cb_data data;
	int i;

	for (i = 0; i < HIT_BOXES; i++) {
		for (ii=boxes[i].begin(); ii!=boxes[i].end(); ++ii) {
			rect_a = (*ii).second;
			collisions_helper.clear();

			for (xx=boxes[i].begin(); xx!=boxes[i].end(); ++xx) {
				rect_b = (*xx).second;
				if (rect_b->id == rect_a->id) {
					continue;
				}
				if (detect_collision(rect_a, rect_b, &data)) {
					kk = collisions_helper.find(rect_b->id);
					if (kk != collisions_helper.end()) {
						continue;
					}
					collisions_helper[rect_b->id] = rect_b;
					rect_a->cb(&data);
				}
			}
		}
	}
}
