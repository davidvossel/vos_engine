#ifndef VOS_COLLISION
#define VOS_COLLISION

#include <map>

using namespace std;

// MUST BE mutliple of 4
#define HIT_BOXES 16

/*!
 * \brief Some collisions catagories are predefined just
 *        so some generic objects can be created.  Any of these
 *        collision catagories can be overridden no problem
 */
enum vos_collision_cat {
	VOS_COLLIDE_BOUNDARY_CAT = 1,
};

struct vos_collision_engine_cb_data {
	unsigned int myid;
	unsigned int mycat;
	unsigned int hitid;
	unsigned int hitcat;
	/* distance to go to escape collision */
	int dist_up;
	int dist_down;
	int dist_left;
	int dist_right;

	/* shortest path out of collision.
	 * just add these two values to the current
	 * position and the collision will be avoided. */
	int shortest_x;
	int shortest_y;
	
	void *userdata;
};

typedef int (*vos_collision_engine_cb)(struct vos_collision_engine_cb_data *data);

struct vos_collision_engine_rect {
	unsigned int id;
	int cat;
	int x;
	int y;
	int w;
	int h;
	int in_maps[HIT_BOXES];
	void *userdata;
	vos_collision_engine_cb cb;
};

class vos_collision_engine {
	private:
		int x;
		int y;
		unsigned int gen_id;
		map <unsigned int, struct vos_collision_engine_rect *> boxes[HIT_BOXES];
		map <unsigned int, struct vos_collision_engine_rect *> rects;
		map <unsigned int, struct vos_collision_engine_rect *> collisions_helper;
		map <unsigned int, vos_collision_engine_rect *>::iterator ii;
		map <unsigned int, vos_collision_engine_rect *>::iterator xx;
		map <unsigned int, vos_collision_engine_rect *>::iterator kk;
		struct vos_collision_engine_rect sections[HIT_BOXES];
		/* returns true if collision exists, stores
		 * required information in data struct when present. */
		int detect_collision(struct vos_collision_engine_rect *rect_a, struct vos_collision_engine_rect *rect_b, vos_collision_engine_cb_data *data);
		int detect_collision_create_cb_data(struct vos_collision_engine_rect *rect_a, struct vos_collision_engine_rect *rect_b, struct vos_collision_engine_cb_data *data);
		int update_rect_map(struct vos_collision_engine_rect *rect);

	public:
		vos_collision_engine(int x = 640, int y = 480);
		~vos_collision_engine();

		unsigned int register_rect(vos_collision_engine_cb cb, int hit_cat, void *userdata, int x  = 1, int y = 1, int w = 1, int h = 1);
		int update_rect_coordinates(unsigned int id, int x, int y);
		int update_rect_size(unsigned int id, int w, int h);
		int unregister_rect(unsigned int id);
		void run_collisions();
};

#endif

