#include "vos_controller_engine.h"

vos_controller_engine::~vos_controller_engine()
{
	struct vos_controller_data *c_data;
	int i;
	for (ii = controllers.begin(); ii!=controllers.end(); ++ii) {
		c_data = (*ii).second;
		for (i = 0; i < VOS_CON_BUTTONS_LEN; i++) {
			c_data->buttons[i].clear();
		}
		controllers.erase(ii);
		delete c_data;
	}
}

unsigned int vos_controller_engine::new_controller()
{
	struct vos_controller_data *c_data = new vos_controller_data();

	id_gen++;
	controllers[id_gen] = c_data;
	return id_gen;
}

int vos_controller_engine::set_button_value(unsigned int id, enum vos_controller_buttons button, int event_key)
{
	struct vos_controller_data *c_data;
	int i;

	ii = controllers.find(id);
	if (ii == controllers.end()) {
		return -1;
	}
	c_data = (*ii).second;

	// if the event exists for another button, remove it.
	for (i = 0; i < VOS_CON_BUTTONS_LEN; i++) {
		c_data->buttons[i].erase(event_key);
	}
	c_data->buttons[button].insert(pair<int, int> (event_key, 0));

	return 0;
}

int vos_controller_engine::is_button_active(unsigned int id, enum vos_controller_buttons button)
{
	struct vos_controller_data *c_data;

	ii = controllers.find(id);
	if (ii == controllers.end()) {
		return 0;
	}
	c_data = (*ii).second;

	return c_data->active[button];
}

int vos_controller_engine::process_event_key(int event_key, int active)
{
	struct vos_controller_data *c_data;
	int i;
	for (ii = controllers.begin(); ii!=controllers.end(); ++ii) {
		c_data = (*ii).second;
		for (i = 0; i < VOS_CON_BUTTONS_LEN; i++) {
			c_data->ii = c_data->buttons[i].find(event_key);
			if (c_data->ii == c_data->buttons[i].end()) {
				continue;
			}
			c_data->active[i] = active;
		}
	}

	return 0;
}



