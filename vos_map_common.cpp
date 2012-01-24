#include "vos_map_common.h"
#include <iostream>
using namespace std;

int vos_map_block::render()
{
	m_engine->draw_image(image_id,
		map->map2cam_x(x),
		map->map2cam_y(y));

	return 0;
}

int vos_map_boundary::render()
{
	m_engine->draw_image(image_id,
		map->map2cam_x(x),
		map->map2cam_y(y));

	c_engine->update_rect_coordinates(hitid, map->map2cam_x(x), map->map2cam_y(y));
	return 0;
}
int vos_map_bound_object::update_coordinates(int distx, int disty) {
	int shortest_x = 0;
	int shortest_y = 0;
	for (int i = 0; i < num_collisions; i++) {
		if (collisions_list[i].data.hitcat == VOS_COLLIDE_BOUNDARY_CAT) {
			if (!shortest_y) {
				shortest_y = collisions_list[i].data.shortest_y;
			} else if (collisions_list[i].data.shortest_y && (shortest_y != collisions_list[i].data.shortest_y)) {
				shortest_y = 0;
			}

			if (!shortest_x) {
				shortest_x = collisions_list[i].data.shortest_x;
			} else if (collisions_list[i].data.shortest_x && (shortest_x != collisions_list[i].data.shortest_x)) {
				shortest_x = 0;
			}
		}
	}

	if (shortest_x && shortest_y) {
		if (abs(shortest_x) < abs(shortest_y)) {
			distx = 0;
			next_x += shortest_x;
		} else {
			disty = 0;
			next_y += shortest_y;
		}
	} else if (shortest_x) {
		distx = 0;
		next_x += shortest_x;
	} else if (shortest_y) {
		disty = 0;
		next_y += shortest_y;
	}
	x = next_x;
	y = next_y;
	next_x += distx;
	next_y += disty;
	return 0;
}

