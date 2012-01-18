#include "vos_collision_engine.h"
#include "stdlib.h"
#include <iostream>

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
		free((*ii).second);
	}
}

unsigned int vos_collision_engine::register_rect(vos_collision_engine_cb cb,
	int hit_cat,
	void *userdata,
	int x, int y, int w, int h)
{
	struct vos_collision_engine_rect *rect;
	map <unsigned int, vos_collision_engine_rect *>::iterator ii;


	if (!(rect = (struct vos_collision_engine_rect *) calloc(1, sizeof(*rect)))) {
		return -1;
	}

	// TODO find an ID that hasn't already been given...
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
	free(rect);

	return 0;
}


int vos_collision_engine::update_rect_map(struct vos_collision_engine_rect *rect)
{
	int i;

	for (i = 0; i < HIT_BOXES; i++) {
		if (detect_collision(&sections[i], rect)) {
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

int vos_collision_engine::detect_collision(struct vos_collision_engine_rect *rect_a, struct vos_collision_engine_rect *rect_b)
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
	}

	if (a_xr <= b_xl) {
		return 0;
	}

	if (a_yt >= b_yb) {
		return 0;
	}

	if (a_yb <= b_yt) {
		return 0;
	}

	return 1;
}

void vos_collision_engine::run_collisions()
{
	struct vos_collision_engine_rect *rect_a;
	struct vos_collision_engine_rect *rect_b;
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
				if (detect_collision(rect_a, rect_b)) {
					kk = collisions_helper.find(rect_b->id);
					if (kk != collisions_helper.end()) {
						continue;
					}
					collisions_helper[rect_b->id] = rect_b;
					rect_a->cb(rect_a->id, rect_a->cat, rect_b->id, rect_b->cat, rect_a->userdata);
				}
			}
		}
	}

}

