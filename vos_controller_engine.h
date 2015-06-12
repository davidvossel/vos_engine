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
