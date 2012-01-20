#ifndef VOS_MAP_COMMON
#define VOS_MAP_COMMON
#include "vos_map.h"

#define MAX_X_CHUNKS 1000
#define MAX_Y_CHUNKS 1000

class vos_map_block: public vos_map_object {
    private:
		int image_id;
    public:
		vos_map_block(int _image_id,
			int x,
			int y,
			struct vos_map_object_data *data) :
			vos_map_object(x, y, data)
		{
			image_id = _image_id;
		}

    int update() { return 0; };
    int render();
};

class vos_map_boundary: public vos_map_object {
    private:
		int image_id;
		int hitid;
    public:
		vos_map_boundary(int _image_id,
			int x,
			int y,
			struct vos_map_object_data *data) :
			vos_map_object(x, y, data)
		{
			image_id = _image_id;
			hitid = data->c_engine->register_rect(
				vos_map_object_collision_cb,
				VOS_COLLIDE_BOUNDARY_CAT,
				this,
				x,
				y,
				m_engine->image_height(_image_id),
				m_engine->image_width(_image_id));
		}
		~vos_map_boundary()
		{
			c_engine->unregister_rect(hitid);
		}

    int update() { return 0; };
    int render();
};

#endif
