#include "vos_map_common.h"

int vos_map_block::render(int ticks)
{
	m_engine->draw_image(image_id,
		map2cam_x(x, map->get_camera_x()),
		map2cam_y(y, map->get_camera_y()));

	return 0;
}


