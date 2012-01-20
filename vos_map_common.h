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

    int update(int ticks) { return 0; };
    int render(int ticks);
};

#endif
