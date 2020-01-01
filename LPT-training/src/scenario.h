/*
 * scenario.h
 *
 *  Created on: Dec 31, 2018
 *      Author: francois
 */

#ifndef SRC_SCENARIO_H_
#define SRC_SCENARIO_H_
#include "Arduino.h"

class scenario {
public:
	scenario(void (*_init)(void), uint32_t (*_action)(uint8_t &));
	virtual ~scenario();

	void run(void);
	void enable(void);
	void disable(void);
	boolean enabled;
	uint32_t delay_time;
	void (*init)(void);
	uint8_t cycle;
	uint32_t (*action)(uint8_t &cycle); // returns delay
};


#endif /* SRC_SCENARIO_H_ */
