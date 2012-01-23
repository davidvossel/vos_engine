#include "vos_map_common.h"

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
	for (int i = 0; i < num_collisions; i++) {
		if (collisions_list[i].data.hitcat == VOS_COLLIDE_BOUNDARY_CAT) {
			if (collisions_list[i].data.shortest_x) {
				next_x += collisions_list[i].data.shortest_x;
				distx = 0;
				break;
			}
			if (collisions_list[i].data.shortest_y) {
				next_y += collisions_list[i].data.shortest_y;
				disty = 0;
				break;
			}
		}
	}

	x = next_x;
	y = next_y;
	next_x += distx;
	next_y += disty;
	return 0;
}


