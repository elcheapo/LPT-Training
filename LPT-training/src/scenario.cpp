/*
 * scenario.cpp
 *
 *  Created on: Dec 31, 2018
 *      Author: francois
 */

#include <avr/pgmspace.h>
#include "scenario.h"

scenario::scenario(void (*_init)(void), uint32_t (*_action)(uint8_t &)) {
	enabled = false;
	delay_time = 0;
	cycle = 0;
	action = _action;
	init = _init;
}

scenario::~scenario() {
	// TODO Auto-generated destructor stub
}

void scenario::enable(void) {
	enabled = true;
	cycle = 1;
}

void scenario::disable(void) {
	enabled = false;
	cycle = 0;
	init();
}

void scenario::run() {
	if (!enabled) return;
	if (delay_time != 0) {
		if (millis() < delay_time)
			return;
	}
	delay_time = millis() + action(cycle);
}

