#include "vos_map.h"
#include "SDL/SDL.h"

vos_map_object::vos_map_object(int _x, int _y, vos_media_engine *_m_engine, vos_collision_engine *_c_engine)
{
	m_engine = _m_engine;
	c_engine = _c_engine;
	x = _x;
	y = _y;
	ready_for_deletion = 0;
	id = 0;
}

int vos_map_object::get_x()
{
	return x;
}
int vos_map_object::get_y()
{
	return y;
}

int vos_map_object::map2cam_x(int map_x, int camera_x)
{
	return map_x-camera_x;
}

int vos_map_object::map2cam_y(int map_y, int camera_y)
{
	return map_y-camera_y;
}

int vos_map_object::delete_me()
{
	return ready_for_deletion;
}
int vos_map_object::set_id(int _id)
{
	id = _id;
	return id;
}
int vos_map_object::get_id()
{
	return id;
}

vos_map::vos_map(int _w, int _h, int _camera_w, int _camera_h)
{
	w = _w;
	h = _h;

	camera_w = _camera_w;
	camera_h = _camera_h;

	camera_x = 0;
	camera_y = 0;

	gen_id = 0;
}

vos_map::~vos_map()
{

}

int vos_map::center_camera_on(int x, int y)
{
	camera_x = x - (camera_w/2);
	camera_y = y - (camera_h/2);
	return 0;
}

int vos_map::update_camera(int x, int y)
{
	camera_x = x;
	camera_y = y;
	return 0;
}

int vos_map::x_to_x_chunk_index(int x)
{
	if (x >= camera_w) {
		return x / camera_w;
	}
	return 0;
}

int vos_map::y_to_y_chunk_index(int y)
{
	if (y >= camera_h) {
		return y / camera_h;
	}
	return 0;
}

int vos_map::put_object_in_chunk(vos_map_object *obj)
{
	int x_index = x_to_x_chunk_index(obj->get_x());
	int y_index = y_to_y_chunk_index(obj->get_y());

	if (obj->get_y() >= camera_h) {
		y_index = obj->get_y() / camera_h;
	}

	if (x_index >= MAX_X_CHUNKS) {
		return -1;
	}
	if (y_index >= MAX_Y_CHUNKS) {
		return -1;
	}

	chunk_map[x_index][y_index].insert(pair <unsigned int, vos_map_object *> (obj->get_id(), obj));

	return 0;
}

int vos_map::remove_object_from_chunk(vos_map_object *obj)
{
	int x_index = x_to_x_chunk_index(obj->get_x());
	int y_index = y_to_y_chunk_index(obj->get_y());

	if (x_index >= MAX_X_CHUNKS) {
		return -1;
	}
	if (y_index >= MAX_Y_CHUNKS) {
		return -1;
	}

	chunk_map[x_index][y_index].erase(obj->get_id());

	return 0;
}

int vos_map::remove_object(int id)
{
	vos_map_object *obj = NULL;
	map_ii = obj_map.find(id);

	if (map_ii == obj_map.end()) {
		return -1;
	}

	obj = (*map_ii).second;
	remove_object_from_chunk(obj);

	delete obj;

	return 0;
}

int vos_map::add_object(vos_map_object *obj)
{
	gen_id++;

	obj->set_id(gen_id);
	obj_map[gen_id] = obj;

	put_object_in_chunk(obj);
	return 0;
}

int vos_map::render_chunk(int x_index, int y_index)
{
	map <unsigned int, vos_map_object *> *tmp;
	vos_map_object *obj;
	int ticks = SDL_GetTicks();

	if (x_index >= MAX_X_CHUNKS) {
		return -1;
	}
	if (y_index >= MAX_Y_CHUNKS) {
		return -1;
	}
	tmp = &chunk_map[x_index][y_index];

	for (map_ii = tmp->begin(); map_ii!=tmp->end(); ++map_ii) {
		obj = (*map_ii).second;

		obj->update();
		obj->render(camera_x, camera_y, ticks);

		if (obj->delete_me()) {
			tmp->erase(obj->get_id());
			remove_object(obj->get_id());
		} else if ((x_index != x_to_x_chunk_index(obj->get_x())) || (y_index != y_to_y_chunk_index(obj->get_y()))) {
			tmp->erase(obj->get_id());
			update_location_list.push_back(obj);
		}
	}

	return 0;
}

int vos_map::render()
{
	int x_index = x_to_x_chunk_index(camera_x);
	int y_index = y_to_y_chunk_index(camera_y);

	/*
	 * We are dealing with 4 chunks here
	 * (x, y), (x+1, y), (x, y+1), (x+1, y+1)
	 */

	update_location_list.clear();

	render_chunk(x_index, y_index);
	render_chunk(x_index+1, y_index);
	render_chunk(x_index, y_index+1);
	render_chunk(x_index+1 , y_index+1);

	for (list_ii = update_location_list.begin();
		list_ii != update_location_list.end();
		++list_ii) {
		put_object_in_chunk(*list_ii);
	}

	return 0;
}
