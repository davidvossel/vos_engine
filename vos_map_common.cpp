#include "vos_map_common.h"

int vos_map_block::render(int camera_x, int camera_y, int ticks)
{
	m_engine->draw_image(image_id, map2cam_x(x, camera_x), map2cam_y(y, camera_y));

	return 0;
}


