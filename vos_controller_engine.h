#ifndef VOS_CONTROLLER
#define VOS_CONTROLLER
#include <iostream>
#include <map>

using namespace std;

enum vos_controller_buttons {
	VOS_CON_UP = 0,
	VOS_CON_DOWN,
	VOS_CON_LEFT,
	VOS_CON_RIGHT,
	VOS_CON_BUTTONS_LEN,
};

struct vos_controller_data {
	map <int, int> buttons[VOS_CON_BUTTONS_LEN];
	map <int, int>::iterator ii;
	int active[VOS_CON_BUTTONS_LEN];
};

class vos_controller_engine {
	private:
		map < unsigned int, vos_controller_data * > controllers;
		map <unsigned int, vos_controller_data *>::iterator ii;
		unsigned int id_gen;
	public:
		vos_controller_engine() {id_gen = 0;}
		~vos_controller_engine();

		unsigned int new_controller();
		int set_button_value(unsigned int id, enum vos_controller_buttons button, int event_key);
		int is_button_active(unsigned int id, enum vos_controller_buttons button);
		int process_event_key(int event_key, int active);
};

#endif
