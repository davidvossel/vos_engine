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



