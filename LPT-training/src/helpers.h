/*
 * helpers.h
 *
 *  Created on: Jan 3, 2019
 *      Author: francois
 */

#ifndef SRC_HELPERS_H_
#define SRC_HELPERS_H_

// I2C helpers
void I2C_digitalWrite(uint8_t pin, uint8_t value){
	uint8_t port = pin / 10;
	uint8_t pin_mask = pin % 10;
	if (value == HIGH){
		i2c_ports[port].set_mask(1<<pin_mask);
	} else {
		i2c_ports[port].clear_mask(1<<pin_mask);
	}
}
uint8_t I2C_digitalRead(uint8_t pin){
	uint8_t port = pin  / 10;
	uint8_t pin_mask = pin % 10;
	uint8_t data = i2c_ports[port].read();
#if 0
	Serial.print(F("I2C digital Read :"));
	Serial.print(port,16);
	Serial.print(F(" / "));
	Serial.print(pin_mask,16);
	Serial.print(F(" / "));
	Serial.println(data,16);
#endif
	data &= (1<<pin_mask);
#if 0
	Serial.print(F(" / "));
	Serial.println(data,16);
#endif
	if (data != 0) return HIGH; else return LOW;
}
void I2C_pinMode(uint8_t pin, uint8_t mode){
	uint8_t port = pin / 10;
	uint8_t pin_mask = pin % 10;
	if (mode != OUTPUT) {
		i2c_ports[port].set_input(1<<pin_mask);
	}
}

/*********************************************************************************/
// first LED

void init1 (void){
		leds1[0] = CRGB::Black;
}
uint32_t update_led1(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds1[0] = CRGB::White;
		cycle = 2;
		return 1000;
	default:
		leds1[0] = CRGB::Black;
		cycle = 1;
		return 1000;
		break;
	}
return 0;
}

scenario led_string1(&init1, &update_led1);

/*-----------------------------------------------------*/

/*********************************************************************************/
// 2nd LED

void init2 (void){
		leds1[1] = CRGB::Black;
}
uint32_t update_led2(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds1[1] = CRGB::Red;
		cycle = 2;
		return 500;
	default:
		leds1[1] = CRGB::Black;
		cycle = 1;
		return 500;
		break;
	}
return 0;
}

scenario led_string2(&init2, &update_led2);

/*-----------------------------------------------------*/

/*********************************************************************************/
// 3rd LED

void init3 (void){
		leds1[2] = CRGB::Black;
}
uint32_t update_led3(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds1[2] = CRGB::Orange;
		cycle = 2;
		return 200;
	default:
		leds1[2] = CRGB::Black;
		cycle = 1;
		return 200;
		break;
	}
return 0;
}

scenario led_string3(&init3, &update_led3);

/*-----------------------------------------------------*/
/*********************************************************************************/
// 4th LED

void init4 (void){
		leds1[3] = CRGB::Black;
}
uint32_t update_led4(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds1[3] = CRGB::Blue;
		cycle = 2;
		return 2000;
	default:
		leds1[3] = CRGB::Black;
		cycle = 1;
		return 2000;
		break;
	}
return 0;
}

scenario led_string4(&init4, &update_led4);

/*-----------------------------------------------------*/
/*********************************************************************************/
// 5th LED

void init5 (void){
		leds1[4] = CRGB::Black;
}
uint32_t update_led5(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds1[4] = CRGB::Green;
		cycle = 2;
		return 1500;
	default:
		leds1[4] = CRGB::Black;
		cycle = 1;
		return 1500;
		break;
	}
return 0;
}

scenario led_string5(&init5, &update_led5);

/*-----------------------------------------------------*/

void init6 (void){
	for(uint8_t i=0; i<NUM_LEDS1; i++) {
		leds1[i] = CRGB::Black;
	}
}
uint32_t update_led6(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	default:
		for(uint8_t i=0; i<NUM_LEDS1; i++) {
			leds1[i] = CHSV(cycle, 255-(30*i), 180 );
		}
		cycle ++;
		if (cycle == 0) cycle = 1;
		return 50;
		break;
	}
return 0;
}

scenario led_string6(&init6, &update_led6);

/*-----------------------------------------------------*/

void init_op1(void) {
}
#define BUTTON1 0

uint32_t run_op1_button(uint8_t &cycle) {
//	Serial.println(cycle);
	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(BUTTON1) == LOW) {

		 	Serial.println(F("Operator Button 1"));
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(BUTTON1) == HIGH) {
			// Button not pressed, restart
			cycle = 1;
		} else {
			led_string1.disable();
			led_string2.disable();
			led_string3.disable();
			led_string4.disable();
			led_string5.disable();
			cycle = 3;
		}
		return 200;
		break;
	case 3:
	default:
		if (I2C_digitalRead(BUTTON1) == LOW) {
			// Button still pressed
			return 200;
		}
		cycle = 1;
		return 200;
		break;
	}
	return 0;
}

scenario op_button1(&init_op1, &run_op1_button);

/*-----------------------------------------------------*/


void init_op2(void) {
}
#define BUTTON2 1

uint32_t run_op2_button(uint8_t &cycle) {

	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(BUTTON2) == LOW) {

		 	Serial.println(F("Operator Button 2"));
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(BUTTON2) == HIGH) {
			// Button not pressed, restart
			cycle = 1;
		} else {
			led_string6.disable();
			led_string1.enable();
			led_string2.enable();
			led_string3.enable();
			led_string4.enable();
			led_string5.enable();
			cycle = 3;
		}
		return 200;
		break;
	case 3:
	default:
		if (I2C_digitalRead(BUTTON2) == LOW) {
			// Button still pressed
			return 200;
		}
		cycle = 1;
		return 200;
		break;
	}
	return 0;
}

scenario op_button2(&init_op2, &run_op2_button);
/*-----------------------------------------------------*/


void init_op3(void) {
}
#define BUTTON3 2

uint32_t run_op3_button(uint8_t &cycle) {
//	Serial.println(cycle);
	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(BUTTON3) == LOW) {

		 	Serial.println(F("Operator Button 3"));
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(BUTTON3) == HIGH) {
			// Button not pressed, restart
			cycle = 1;
		} else {
			led_string6.disable();
			cycle = 3;
		}
		return 200;
		break;
	case 3:
	default:
		if (I2C_digitalRead(BUTTON3) == LOW) {
			// Button still pressed
			return 200;
		}
		cycle = 1;
		return 200;
		break;
	}
	return 0;
}

scenario op_button3(&init_op3, &run_op3_button);
/*-----------------------------------------------------*/


void init_op4(void) {
}
#define BUTTON4 3

uint32_t run_op4_button(uint8_t &cycle) {

	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(BUTTON4) == LOW) {

		 	Serial.println(F("Operator Button 4"));
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(BUTTON4) == HIGH) {
			// Button not pressed, restart
			cycle = 1;
		} else {
			led_string1.disable();
			led_string2.disable();
			led_string3.disable();
			led_string4.disable();
			led_string5.disable();
			led_string6.init();
			led_string6.enable();
			cycle = 3;
		}
		return 200;
		break;
	case 3:
	default:
		if (I2C_digitalRead(BUTTON4) == LOW) {
			// Button still pressed
			return 200;
		}
		cycle = 1;
		return 200;
		break;
	}
	return 0;
}

scenario op_button4(&init_op4, &run_op4_button);


#endif /* SRC_HELPERS_H_ */
