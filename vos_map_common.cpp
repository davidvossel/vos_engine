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

