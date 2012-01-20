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

typedef int (*vos_collision_engine_cb)(unsigned int myid, int mycat, unsigned int hitid, int hitcat, void *userdata);

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

		int detect_collision(struct vos_collision_engine_rect *rect_a, struct vos_collision_engine_rect *rect_b);
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

