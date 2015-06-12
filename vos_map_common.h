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

class  vos_map_bound_object: public vos_map_object {
	protected:
		int next_x;
		int next_y;
	public:
		vos_map_bound_object(int x, int y, struct vos_map_object_data * data): vos_map_object(x, y, data)
		{
			next_x = x;
			next_y = y;
		}
		virtual ~vos_map_bound_object() {};
		int update_coordinates(int distx, int disty);
		/* must be called to update position, never use x and y
		 * directly when using this class */
		/* coordinates to render next image on */
		int get_converted_x() { return map->map2cam_x(x); }
		int get_converted_y() { return map->map2cam_y(y); }
		int get_cam_x() { return x; }
		int get_cam_y() { return y; }
		/* coordinates to base next hit box on */
		int get_hit_x() { return map->map2cam_x(next_x); }
		int get_hit_y() { return map->map2cam_y(next_y); }
};
#endif
