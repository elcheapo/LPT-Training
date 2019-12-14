/*
 * actions.h
 *
 *  Created on: Dec 31, 2018
 *      Author: francois
 */

#ifndef SRC_ACTIONS_H_
#define SRC_ACTIONS_H_

enum actions_t {
    A_END = 0x00,             ///< end of command string, restart from beginning
    A_START_SCRIPT = 0x01,    ///< + 1 byte(n), start script #n
    A_STOP_SCRIPT = 0x02,     ///< + 1 byte(n), stop script #n
    A_DELAY = 0x03, 		  ///< + 2 bytes, delay # of milliseconds
    A_I2C_PORT_IS_1 = 0x04,   ///< + 1 byte read i2c port #n, + 1 byte skip #m if bit is 0
    A_I2C_PORT_IS_0 = 0x05,   ///< + 1 byte read i2c port #n, + 1 byte skip #m if bit is 1
    A_SET_LED_STRING = 0x06,  ///< +1 byte LED string, + 4 bytes cRGB, set all LEDs in string to cRGB value
    A_SET_ONE_LED = 0x07,     ///< +1 byte LED string, + 1 byte LED index + 4 bytes cRGB, set LED[index] in string to cRGB value
	A_I2C_SET = 0x08,         ///< +1 byte I2C port #n : set I2C port #n
	A_I2C_CLEAR = 0x09        ///< +1 byte I2C port #n : clear I2C port #n
};

#endif /* SRC_ACTIONS_H_ */
