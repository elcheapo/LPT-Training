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
	Serial.print(data,16);
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
// Mas de cocagne 40 [0..39] / led1

#define NUM_LEDS11 40

void init11 (uint8_t &cycle){
	for (uint8_t i=0; i<NUM_LEDS11; i++) {
		leds1[i] = CRGB::Black;
	}
	cycle=0;
}
#if 0
uint32_t update_led11(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		for (uint8_t i=0; i<32; i++) {
			leds1[i] = CRGB::White;
		}
		for (uint8_t i=32; i<NUM_LEDS11; i++) {
			leds1[i] = CRGB::Cyan;
		}
		cycle = 2;
		return 900;
	default:
		for (uint8_t i=0; i<32; i++) {
			leds1[i] = CRGB::Cyan;
		}
		for (uint8_t i=32; i<NUM_LEDS11; i++) {
			leds1[i] = CRGB::White;
		}
		cycle = 1;
		return 900;
		break;
	}
return 0;
}
#endif
uint32_t update_led11(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		for (uint8_t i=0; i<NUM_LEDS11; i++) {
			leds1[i] = CRGB::White;
		}
		cycle = 2;
		return 1000;
	default:
		CRGB color;
		switch (TCNT0 & 0xf) {
		case 0 ... 4 : color = CRGB::Yellow; break;
		case 5 ... 10 : color = CRGB::Red; break;
		case 11 ... 14 : color = CRGB::Orange; break;
		default : color = CRGB::Blue; break;
		}
		for (uint8_t i=0; i<NUM_LEDS11; i++) {
			leds1[i] = color;
		}
		cycle = 1;
		return 1000;
		break;
	}
return 0;
}

scenario led_string11(&init11, &update_led11);

/*********************************************************************************/
// Mas de cocagne 12 [0..11] / led7

#define NUM_LEDS12 12
#define OFFSET12 0

void init12 (uint8_t &cycle){
	for (uint8_t i=OFFSET12; i<OFFSET12+NUM_LEDS12; i++) {
		leds7[i] = CRGB::Black;
	}
	cycle=0;
}

uint32_t update_led12(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		for (uint8_t i=OFFSET12; i<OFFSET12+NUM_LEDS12; i++) {
			leds7[i] = CRGB::Red;
		}
		cycle = 2;
		return 950;
	default:
		CRGB color;
		switch (TCNT0 & 0xf) {
		case 0 ... 10 : color = CRGB::Orange; break;
		case 11 ... 13 : color = CRGB::Yellow; break;
		case 14 : color = CRGB::Blue; break;
		default : color = CRGB::Green; break;
		}
		for (uint8_t i=OFFSET12; i<OFFSET12+NUM_LEDS12; i++) {
			leds7[i] = color;
		}
		cycle = 1;
		return 950;
		break;
	}
return 0;
}

scenario led_string12(&init12, &update_led12);



/************************************************************************************************/
// Scenario for string 21 - Palais des mirages 2 LED leds2[0], leds2[1]

#define DELAY21 500

void init21(uint8_t &cycle) {
	leds2[0] = CRGB::Black;
	leds2[1] = CRGB::Black;
	cycle=0;
}
uint32_t update_led21(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds2[0] = CRGB::Red;
		leds2[1] = CRGB::Yellow;
		cycle ++;
		return DELAY21;
		break;
	case 2:
		leds2[0] = CRGB::Yellow;
		leds2[1] = CRGB::Red;
		cycle ++;
		return DELAY21;
		break;
	case 3:
		leds2[0] = CRGB::Yellow;
		leds2[1] = CRGB::Green;
		cycle ++;
		return DELAY21;
		break;
	case 4:
		leds2[0] = CRGB::Green;
		leds2[1] = CRGB::Blue;
		cycle ++;
		return DELAY21;
		break;
	case 5:
		leds2[0] = CRGB::Blue;
		leds2[1] = CRGB::Cyan;
		cycle ++;
		return DELAY21;
		break;
	case 6:
		leds2[0] = CRGB::Cyan;
		leds2[1] = CRGB::OrangeRed;
		cycle ++;
		return DELAY21;
		break;
	case 7:
		leds2[0] = CRGB::OrangeRed;
		leds2[1] = CRGB::Orange;
		cycle ++;
		return DELAY21;
		break;
	case 8:
		leds2[0] = CRGB::Orange;
		leds2[1] = CRGB::Pink;
		cycle ++;
		return DELAY21;
		break;
	case 9:
		leds2[0] = CRGB::Pink;
		leds2[1] = CRGB::Azure;
		cycle ++;
		return DELAY21;
		break;
	default:
		leds2[0] = CRGB::Azure;
		leds2[1] = CRGB::Red;
		cycle = 1;
		return DELAY21;
		break;

	}
return 0;
}

scenario led_string21(&init21, &update_led21);

/************************************************************************************************/
// Scenario for string 2 - Palais des glaces 3

#define DELAY22 400

void init22(uint8_t &cycle) {
	leds2[2] = CRGB::Black;
	leds2[3] = CRGB::Black;
	leds2[4] = CRGB::Black;
	cycle=0;
}
uint32_t update_led22(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case 1:
		leds2[2] = CRGB::Blue;
		leds2[3] = CRGB::Blue;
		leds2[4] = CRGB::Blue;
		cycle ++;
		return DELAY22;
		break;
	case 2:
		leds2[2] = CRGB::Cyan;
		leds2[3] = CRGB::Cyan;
		leds2[4] = CRGB::Cyan;
		cycle ++;
		return DELAY22;
		break;
	case 3:
		leds2[2] = CRGB::AliceBlue;
		leds2[3] = CRGB::AliceBlue;
		leds2[4] = CRGB::AliceBlue;
		cycle ++;
		return DELAY22;
		break;
	case 4:
		leds2[2] = CRGB::Aqua;
		leds2[3] = CRGB::Aqua;
		leds2[4] = CRGB::Aqua;
		cycle ++;
		return DELAY22;
		break;
	case 5:
		leds2[2] = CRGB::CadetBlue;
		leds2[3] = CRGB::CadetBlue;
		leds2[4] = CRGB::CadetBlue;
		cycle ++;
		return DELAY22;
		break;
	case 6:
		leds2[2] = CRGB::Aquamarine;
		leds2[3] = CRGB::Aquamarine;
		leds2[4] = CRGB::Aquamarine;
		cycle ++;
		return DELAY22;
		break;
	case 7:
		leds2[2] = CRGB::DarkBlue;
		leds2[3] = CRGB::DarkBlue;
		leds2[4] = CRGB::DarkBlue;
		cycle ++;
		return DELAY22;
		break;
	case 8:
		leds2[2] = CRGB::DarkCyan;
		leds2[3] = CRGB::DarkCyan;
		leds2[4] = CRGB::DarkCyan;
		cycle ++;
		return DELAY22;
		break;
	case 9:
		leds2[2] = CRGB::BlueViolet;
		leds2[3] = CRGB::BlueViolet;
		leds2[4] = CRGB::BlueViolet;
		cycle ++;
		return DELAY22;
		break;
	case 10:
		leds2[2] = CRGB::DarkSlateBlue;
		leds2[3] = CRGB::DarkSlateBlue;
		leds2[4] = CRGB::DarkSlateBlue;
		cycle ++;
		return DELAY22;
		break;
	case 11:
		leds2[2] = CRGB::Lavender;
		leds2[3] = CRGB::Lavender;
		leds2[4] = CRGB::Lavender;
		cycle ++;
		return DELAY22;
		break;
	case 12:
		leds2[2] = CRGB::LavenderBlush;
		leds2[3] = CRGB::LavenderBlush;
		leds2[4] = CRGB::LavenderBlush;
		cycle ++;
		return DELAY22;
		break;
	default:
		leds2[2] = CRGB::LightCyan;
		leds2[3] = CRGB::LightCyan;
		leds2[4] = CRGB::LightCyan;
		cycle =1;
		return DELAY22;
		break;

	}
return 0;
}

scenario led_string22(&init22, &update_led22);


/*********************************************************************************/
// Grande roue Gauche led3/led4
// Grande roue Droite
// Courone sur led2 5--17

uint8_t hue34;

void init34 (uint8_t &cycle) {
	for (uint8_t i=0; i<NUM_LEDS3; i++) {
		leds3[i] = CRGB::Black;
		leds4[i] = CRGB::Black;
	}
	for (uint8_t i=0; i<NUM_LEDS2-5; i++) {
		leds2[i+5] = CRGB::Black;
	}
	cycle=0;
	hue34  = 0;
}
void up34_led( uint8_t start) {
	for  (uint8_t i = 0; i < (2*start); i++) {
		leds3[i]= leds4[i]=CHSV(hue34, 255, 180);
	}
	uint8_t j = 0;
	for (uint8_t i = (2*start); i < 26; i++) {
		j += 20;
		leds3[i]= leds4[i]= CHSV(hue34, 255-j, 180);
	}
	for (uint8_t i=0; i<NUM_LEDS2-5; i++) {
		leds2[i+5] = CHSV(hue34, 255-j, 180);
	}
}


uint32_t update_led34(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	default:
		up34_led(cycle);
		cycle ++;
		return 500;
		break;
	case NUM_LEDS3/2:
		up34_led(cycle);
		cycle = 1;
		hue34 += 8;
		return 500;
		break;
	}
	return 0;
}

scenario led_string34(&init34, &update_led34);

/*********************************************************************************/
// Petit Bandeau / led5
void init5(uint8_t &cycle) {
	for (uint8_t i=0; i<NUM_LEDS5; i++) {
		leds5[i] = CRGB::Black;
	}
}


#define HUE 150
void up5_led( uint8_t start) {
	uint8_t i,diff;
	for( i=0; i<NUM_LEDS5 ; i++) {
		diff = abs(i-start);
		leds5[i] = CHSV(HUE-diff,255,200);;
	}
}

uint32_t update_led5(uint8_t & cycle) {
	switch (cycle) {
	case 0:
		break;
	case NUM_LEDS7:
		up5_led(NUM_LEDS5-1);
		cycle = 1;
		return 300;
		break;
	default:
		up5_led(cycle-1);
		cycle++;
		return 300;
		break;
	}
return 0;
}

scenario led_string5(&init5, &update_led5);

/*********************************************************************************/
// Grand bandeau / led6
void init6 (uint8_t &cycle) {
	for (uint8_t i=0; i<NUM_LEDS6; i++) {
		leds6[i] = CRGB::Black;
	}
	cycle=0;
}

void up6_led( uint8_t start) {
	uint8_t i;
	for( i=0; i<NUM_LEDS6 ; i++) {
		leds6[i] = CRGB::White;
	}
	leds6[start] = CRGB::Red;
}

uint32_t update_led6(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		break;
	case NUM_LEDS6:
		up6_led(NUM_LEDS6-1);
		cycle = 1;
		return 300;
		break;
	default:
		up6_led(cycle-1);
		cycle++;
		return 300;
		break;
	}
return 0;
}

scenario led_string6(&init6, &update_led6);


/*********************************************************************************/
// Handling of the train

uint16_t speed;

void train_init(uint8_t &cycle) {
	speed = 0;
	timer3.begin();
	timer3.analog_set_speed_and_direction(speed,off);
	cycle=0;
}
uint32_t train(uint8_t &cycle) {
//	Serial.print('T');
	switch (cycle) {
	case 0:
		// Stop Train
		timer3.analog_set_speed_and_direction(speed,off);
		return 200;
		break;
	case 1:
		if (I2C_digitalRead(B_TRAIN) == LOW) {
			// button pressed, start the train
			Serial.print('S');
			speed = 350;
			timer3.analog_set_speed_and_direction(speed, backward);
			Serial.println(F("Train starts"));
			return 3000; // give it time to get out of detection zone
		}

		if (I2C_digitalRead(OP_BUTTON3) == LOW) {
			// button pressed, faster
			Serial.print('+');
			speed += 10;
			Serial.print(F("Speed = "));
			Serial.println(speed);
			timer3.analog_set_speed_and_direction(speed, backward);
		}
		if (I2C_digitalRead(OP_BUTTON4) == LOW) {
			// button pressed, slower
			Serial.print('-');
			speed -= 10;
			Serial.print(F("Speed = "));
			Serial.println(speed);
			timer3.analog_set_speed_and_direction(speed, backward);
		}
		if ((I2C_digitalRead(OP_BUTTON3) == LOW) & (I2C_digitalRead(OP_BUTTON4) == LOW)) {
			// button pressed, slower
			Serial.println(F("STOP"));
			speed = 0;
			timer3.analog_set_speed_and_direction(speed, backward);
		}
		// If train goes in front of station, stop
		if (digitalRead(CURRENT_DETECT) == LOW) {
			Serial.println(F("Detected"));
			// Stop Train
			timer3.analog_set_speed_and_direction(speed,off);
		}
		return 200;
		break;
	default:
		return 200;
		break;
	}
return 0;
}

scenario train_control(&train_init, &train);

/*********************************************************************************/

void init_manege1(uint8_t &cycle) {
	// Turn Off Manège
	I2C_digitalWrite(R_BALANCELLE,LOW);
	cycle = 0;

}

uint32_t manege1(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_BALANCELLE) == LOW) {
			I2C_digitalWrite(R_BALANCELLE, HIGH);
			requested_music = TRACK_BALANCELLE;
			cycle = 2;
			Serial.println(F("Balancelles"));
		}
		return 500;
		break;
	case 2:
		cycle = 3;
		return MANEGE_DURATION;
	default:
		cycle = 1;
		I2C_digitalWrite(R_BALANCELLE, LOW);
		Serial.println(F("Balancelles OFF"));
		return 500;
		break;
	}
return 0;
}

scenario op_manege1(&init_manege1,&manege1);


/*********************************************************************************/
void init_manege2(uint8_t &cycle) {
	// Turn Off Manège
	I2C_digitalWrite(R_TOUR,LOW);
	cycle = 0;

}
uint32_t manege2(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_TOUR) == LOW) {
			I2C_digitalWrite(R_TOUR, HIGH);
			requested_music = TRACK_TOUR;
			cycle = 2;
			Serial.println(F("Tour"));
		}
		return 500;
		break;
	case 2:
		cycle = 3;
		return MANEGE_DURATION;
	default:
		cycle = 1;
		I2C_digitalWrite(R_TOUR, LOW);
		Serial.println(F("Tour OFF"));
		return 500;
		break;
	}
return 0;
}

scenario op_manege2(&init_manege2, &manege2);

/*********************************************************************************/

/*********************************************************************************/
void init_manege3(uint8_t &cycle) {
	// Turn Off Manège
	I2C_digitalWrite(R_ROUE,LOW);
	cycle = 0;

}
uint32_t manege3(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_ROUE) == LOW) {
			I2C_digitalWrite(R_ROUE, HIGH);
			requested_music = TRACK_ROUE;
			cycle = 2;
			Serial.println(F("Grande Roue"));
		}
		return 500;
		break;
	case 2:
		cycle = 3;
		return MANEGE_DURATION;
	default:
		cycle = 1;
		I2C_digitalWrite(R_ROUE, LOW);
		Serial.println(F("Grande Roue Off 3 OFF"));
		return 500;
		break;
	}
return 0;
}

scenario op_manege3(&init_manege3, &manege3);

/*********************************************************************************/
/*********************************************************************************/
void init_manege4(uint8_t &cycle) {
	// Turn Off Manège
//	I2C_digitalWrite(R_CHEVEAUX,LOW);
	cycle = 0;

}
uint32_t manege4(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_CHEVEAUX) == LOW) {
//			I2C_digitalWrite(R_CHEVEAUX, HIGH);
			requested_music = TRACK_CHEVAUX;
			cycle = 2;
			Serial.println(F("Petit Cheveaux"));
		}
		return 500;
		break;
	case 2:
		cycle = 3;
		return MANEGE_DURATION;
	default:
		cycle = 1;
//		I2C_digitalWrite(R_CHEVEAUX, LOW);
		Serial.println(F("Petit Chevaux OFF"));
		return 500;
		break;
	}
return 0;
}

scenario op_manege4(&init_manege4, &manege4);


/*********************************************************************************/

/*********************************************************************************/
void init_manege5(uint8_t &cycle) {
	// Turn Off Manège
	I2C_digitalWrite(R_CHAISES,LOW);
	cycle = 0;

}
uint32_t manege5(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_CHAISES) == LOW) {
			I2C_digitalWrite(R_CHAISES, HIGH);
			requested_music = TRACK_CHAISES;
			cycle = 2;
			Serial.println(F("Chaises"));
		}
		return 500;
		break;
	case 2:
		cycle = 3;
		return MANEGE_DURATION;
	default:
		cycle = 1;
		I2C_digitalWrite(R_CHAISES, LOW);
		Serial.println(F("Chaises OFF"));
		return 500;
		break;
	}
return 0;
}

scenario op_manege5(&init_manege5, &manege5);

/*********************************************************************************/
/*********************************************************************************/
void init_manege6(uint8_t &cycle) {
	// Turn Off Manège
	I2C_digitalWrite(R_WATERPARK,LOW);
	cycle = 0;

}
uint32_t manege6(uint8_t &cycle) {
	switch (cycle) {
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_WATERPARK) == LOW) {
			I2C_digitalWrite(R_WATERPARK, HIGH);
			requested_music = TRACK_WATERPARK;
			cycle = 2;
			Serial.println(F("Manège 6"));
		}
		return 500;
		break;
	case 2:
		cycle = 3;
		return MANEGE_DURATION;
	default:
		cycle = 1;
		I2C_digitalWrite(R_WATERPARK, LOW);
		return 500;
		break;
	}
return 0;
}

scenario op_manege6(&init_manege6, &manege6);

/*-----------------------------------------------------*/
// Operator button 1 //STOP EVERYTHING, restart after a 1 sec press
uint8_t long_press;

void init_op1(uint8_t &cycle) {
	cycle = 0;
	long_press = 0;
}


uint32_t run_op1_button(uint8_t &cycle) {

	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(B_EMERG) == LOW) {
		 	led_string11.disable(led11_cycle);
		 	led_string12.disable(led12_cycle);
		 	led_string21.disable(led21_cycle);
		 	led_string22.disable(led22_cycle);
		 	led_string34.disable(led34_cycle);
		 	led_string5.disable(led5_cycle);
		 	led_string6.disable(led6_cycle);
		 	train_control.disable(train_cycle);
		 	op_manege1.disable(manege1_cycle);
		 	op_manege2.disable(manege2_cycle);
		 	op_manege3.disable(manege3_cycle);
		 	op_manege4.disable(manege4_cycle);
		 	op_manege5.disable(manege5_cycle);
		 	op_manege6.disable(manege6_cycle);
		 	digitalWrite(FET4, LOW);
		 	stop_request = 1;
		 	Serial.println(F("Operator Button 1 - OFF"));
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(B_EMERG) == HIGH) {
			// Button not pressed, wait for a long press to re-enable
			cycle = 3;
		}
		return 200;
		break;
	case 3:
		if (I2C_digitalRead(B_EMERG) == LOW) {
			// Button pressed, wait for a long press to re-enable
			cycle = 4;
			return 1000;
		}
		return 200;
		break;

	case 4:
		if (I2C_digitalRead(B_EMERG) == LOW) {
			// Button still pressed, re-enable everything
			led_string11.enable(led11_cycle);
			led_string12.enable(led12_cycle);
			led_string21.enable(led21_cycle);
			led_string22.enable(led22_cycle);
			led_string34.enable(led34_cycle);
			led_string5.enable(led5_cycle);
			led_string6.enable(led6_cycle);
			train_control.enable(train_cycle);
			op_manege1.enable(manege1_cycle);
			op_manege2.enable(manege2_cycle);
			op_manege3.enable(manege3_cycle);
			op_manege4.enable(manege4_cycle);
			op_manege5.enable(manege5_cycle);
			op_manege6.enable(manege6_cycle);
		 	digitalWrite(FET4, HIGH);
		 	Serial.println(F("Operator Button 1 - ON"));
			cycle = 5;
		} else {
			cycle = 3;
		}
		return 200;
		break;
	case 5:
	default:
		if (I2C_digitalRead(B_EMERG) == HIGH) {
			// Button not pressed, go back to state 1
			cycle = 1;
		}
		return 200;
		break;

	}
	return 0;
}

scenario op_button1(&init_op1, &run_op1_button);

/*-----------------------------------------------------*/
// Operator button 2 // Switch OFF WATERPARK

void init_op2(uint8_t &cycle) {
	cycle = 0;
	long_press = 0;

}

uint32_t run_op2_button(uint8_t &cycle) {

	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(OP_BUTTON2) == LOW) {
		 	led_string5.disable(led5_cycle);
		 	op_manege6.disable(manege6_cycle);
		 	Serial.println(F("Operator Button 2 - OFF"));
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(OP_BUTTON2) == HIGH) {
			// Button not pressed, wait for a long press to re-enable
			cycle = 3;
		}
		return 200;
		break;
	case 3:
		if (I2C_digitalRead(OP_BUTTON2) == LOW) {
			// Button pressed, wait for a long press to re-enable
			cycle = 4;
			return 1000;
		}
		return 200;
		break;

	case 4:
		if (I2C_digitalRead(OP_BUTTON2) == LOW) {
			// Button still pressed, re-enable everything
			led_string5.enable(led5_cycle);
			op_manege6.enable(manege6_cycle);
		 	Serial.println(F("Operator Button 2 - ON"));
			cycle = 5;
		} else {
			cycle = 3;
		}
		return 200;
		break;
	case 5:
	default:
		if (I2C_digitalRead(OP_BUTTON2) == HIGH) {
			// Button not pressed, go back to state 1
			cycle = 1;
		}
		return 200;
		break;

	}
	return 0;
}

scenario op_button2(&init_op2, &run_op2_button);

/*-----------------------------------------------------*/
// Operator button 3 : disable all menege and train

void init_op3(uint8_t &cycle) {
	cycle = 0;
}

uint32_t run_op3_button(uint8_t &cycle) {
	switch (cycle){
	case 0:
		return 0;
		break;
	case 1:
		if (I2C_digitalRead(OP_BUTTON3) == LOW) {
		 	train_control.disable(train_cycle);
		 	op_manege1.disable(manege1_cycle);
		 	op_manege2.disable(manege2_cycle);
		 	op_manege3.disable(manege3_cycle);
		 	op_manege4.disable(manege4_cycle);
		 	op_manege5.disable(manege5_cycle);
		 	op_manege6.disable(manege6_cycle);
		 	Serial.println(F("Operator Button 3 - ON"));
		 	stop_request = 1;
			cycle = 2;
		}
		return 200;
	case 2:
		if (I2C_digitalRead(OP_BUTTON3) == HIGH) {
			// Button not pressed, wait for a long press to re-enable
			cycle = 3;
		}
		return 200;
		break;
	case 3:
		if (I2C_digitalRead(OP_BUTTON3) == LOW) {
			// Button pressed, wait for a long press to re-enable
			cycle = 4;
			return 1000;
		}
		return 200;
		break;

	case 4:
		if (I2C_digitalRead(OP_BUTTON3) == LOW) {
			// Button still pressed, re-enable everything
			op_manege1.enable(manege1_cycle);
			op_manege2.enable(manege2_cycle);
			op_manege3.enable(manege3_cycle);
			op_manege4.enable(manege4_cycle);
			op_manege5.enable(manege5_cycle);
			op_manege6.enable(manege6_cycle);
		 	Serial.println(F("Operator Button 3 - ON"));
			cycle = 5;
		} else {
			cycle = 3;
		}
		return 200;
		break;
	case 5:
	default:
		if (I2C_digitalRead(OP_BUTTON1) == HIGH) {
			// Button not pressed, go back to state 1
			cycle = 1;
		}
		return 200;
		break;

	}
	return 0;
}

scenario op_button3(&init_op3, &run_op3_button);

/*-----------------------------------------------------*/
// Operator button 4

void init_op4(uint8_t &cycle) {
	cycle = 0;
}

uint32_t run_op4_button(uint8_t &cycle) {

	return 10000;

}

scenario op_button4(&init_op4, &run_op4_button);


/*-----------------------------------------------------*/
// MP3 Control

void init_mp3(uint8_t &cycle) {
	cycle = 0;
	mp3.playStop();
	mp3_status = 0;
	requested_music = 0;
	stop_request = 0;
}

uint32_t run_mp3(uint8_t &cycle) {

	// 1) if track ended, restart with default track
	if (mp3_status == MD_YX5300::STS_FILE_END) {
	 	mp3.volume(MID_VOLUME);
		mp3.playTrack(MP3_DEFAULT);
		Serial.println(F("Default music"));
	}
	// 2) if somebody wants a specific song, play it ...
	if (requested_music != 0) {
		mp3.playTrack(requested_music);
		Serial.print(F("Track : "));
		Serial.println(requested_music);
		requested_music = 0;
	}
	if (stop_request != 0) {
		mp3.playStop();
		stop_request = 0;
	}
	return 100;

}

scenario op_mp3(&init_mp3, &run_mp3);

void cbResponse(const MD_YX5300::cbData *status)
// Used to process device responses either as a library callback function
// or called locally when not in callback mode.
{
	mp3_status = status->code;
	if (bUseCallback)
    Serial.print(F("\nCback status: "));
  else
    Serial.print(F("\nSync Status: "));

  switch (status->code)
  {
  case MD_YX5300::STS_OK:         Serial.print(F("STS_OK"));         break;
  case MD_YX5300::STS_TIMEOUT:    Serial.print(F("STS_TIMEOUT"));    break;
  case MD_YX5300::STS_VERSION:    Serial.print(F("STS_VERSION"));    break;
  case MD_YX5300::STS_CHECKSUM:   Serial.print(F("STS_CHECKSUM")); mp3_status = MD_YX5300::STS_FILE_END;   break;
  case MD_YX5300::STS_TF_INSERT:  Serial.print(F("STS_TF_INSERT"));  break;
  case MD_YX5300::STS_TF_REMOVE:  Serial.print(F("STS_TF_REMOVE"));  break;
  case MD_YX5300::STS_ERR_FILE:   Serial.print(F("STS_ERR_FILE"));  mp3_status = MD_YX5300::STS_FILE_END;  break;
  case MD_YX5300::STS_ACK_OK:     Serial.print(F("STS_ACK_OK"));     break;
  case MD_YX5300::STS_FILE_END:   Serial.print(F("STS_FILE_END"));   break;
  case MD_YX5300::STS_INIT:       Serial.print(F("STS_INIT"));       break;
  case MD_YX5300::STS_STATUS:     Serial.print(F("STS_STATUS"));     break;
  case MD_YX5300::STS_EQUALIZER:  Serial.print(F("STS_EQUALIZER"));  break;
  case MD_YX5300::STS_VOLUME:     Serial.print(F("STS_VOLUME"));     break;
  case MD_YX5300::STS_TOT_FILES:  Serial.print(F("STS_TOT_FILES"));  break;
  case MD_YX5300::STS_PLAYING:    Serial.print(F("STS_PLAYING"));    break;
  case MD_YX5300::STS_FLDR_FILES: Serial.print(F("STS_FLDR_FILES")); break;
  case MD_YX5300::STS_TOT_FLDR:   Serial.print(F("STS_TOT_FLDR"));   break;
  default: Serial.print(F("STS_??? 0x")); Serial.print(status->code, HEX); break;
  }

  Serial.print(F(", 0x"));
  Serial.println(status->data, HEX);
}


void setCallbackMode(bool b)
{
  bUseCallback = b;
  Serial.print(F("\n>Callback "));
  Serial.print(b ? F("ON") : F("OFF"));
  mp3.setCallback(b ? cbResponse : nullptr);
}

void setSynchMode(bool b)
{
  bUseSynch = b;
  Serial.print(F("\n>Synchronous "));
  Serial.print(b ? F("ON") : F("OFF"));
  mp3.setSynchronous(b);
}



#endif /* SRC_HELPERS_H_ */
