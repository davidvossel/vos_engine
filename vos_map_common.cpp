#include "vos_map_common.h"

int vos_map_block::render()
{
	m_engine->draw_image(image_id,
		map->map2cam_x(x),
		map->map2cam_y(y));

	return 0;
}


