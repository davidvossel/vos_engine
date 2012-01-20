#ifndef VOS_MAP
#define VOS_MAP
#include "vos_media_engine.h"
#include "vos_collision_engine.h"
#include "vos_controller_engine.h"
#include <iostream>
#include <map>
#include <list>

using namespace std;

#define MAX_X_CHUNKS 1000
#define MAX_Y_CHUNKS 1000

struct vos_map_object_data;

class vos_map;

class vos_map_object {
	protected:
		vos_media_engine *m_engine;
		vos_collision_engine *c_engine;
		vos_controller_engine *controllers;
		vos_map *map;

		int ready_for_deletion;
		int x;
		int y;
		int id;
		int map2cam_x(int map_x, int camera_x);
		int map2cam_y(int map_y, int camera_y);
	public:
		vos_map_object(int x,
			int y,
			struct vos_map_object_data *data);
		virtual ~vos_map_object() {};

		/* get x_coordinate */
		int get_x();
		/* get y_coordinate */
		int get_y();

		int calc_dist(int ticks, int pps) {
			if (ticks) {
				return (((float)ticks / 1000) * pps);
			} else {
				return 0;
			}
		}

		/* update means update your AI logic or whatever */
		virtual int update() = 0;
		/* Render means that you're actually getting drawn on the screen */
		virtual int render(int camera_x, int camera_y, int ticks) = 0;
		/* when this returns true, the object gets removed from chunk */
		int delete_me();

		int get_id();
		int set_id(int id);
};

class vos_map {
	private:
		int w;
		int h;

		int camera_w;
		int camera_h;

		int camera_x;
		int camera_y;
		unsigned int gen_id;

		/* dealing with at most 4 chunks at a time. out of those 4
		 * render call render on all the objects in them. */
		map <unsigned int, vos_map_object *> chunk_map[MAX_X_CHUNKS][MAX_Y_CHUNKS];
		map <unsigned int, vos_map_object *> obj_map;
		map <unsigned int, vos_map_object *>::iterator map_ii;

		list <vos_map_object *> update_location_list;
		list <vos_map_object *>::iterator list_ii;

		int render_chunk(int x_index, int y_index);
		int put_object_in_chunk(vos_map_object *obj);
		int remove_object_from_chunk(vos_map_object *obj);

		int x_to_x_chunk_index(int x);
		int y_to_y_chunk_index(int y);

	public:
		vos_map(int w, int h, int camera_w, int camera_h);
		~vos_map();

		int center_camera_on(int x, int y);
		int update_camera(int x, int y);
		int add_object(vos_map_object *obj);
		int remove_object(int id);
		int render();
};

struct vos_map_object_data {
	vos_media_engine *m_engine;
	vos_collision_engine *c_engine;
	vos_controller_engine *controllers;
	vos_map *map;
};



#endif

