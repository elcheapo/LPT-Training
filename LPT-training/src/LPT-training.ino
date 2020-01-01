
#include <stdint.h>
#include <Wire.h>
#include <SPI.h>
#include <FastLED.h>
#include <ssd1306.h>
#include "nano_gfx.h"
#include "i2c_port.h"
#include "scenario.h"
#include "pixeltypes.h"

//#include "sova.h"


// How many leds in your strip?
#define NUM_LEDS1 5

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN1 A2 // Grande Roue Gauche

// Define 3 I2C extender
#define NB_I2C_PORT 1
I2c_Port i2c_ports[NB_I2C_PORT]= {I2c_Port(0x20)};

// Define the array of leds
CRGB leds1[NUM_LEDS1];

uint32_t current_time;

uint8_t current_color;

#include "helpers.h"
void setup() { 
	uint8_t i,ret;

	delay(500);
	
	Wire.begin();
	Serial.begin(115200);

	Serial.println(F("\nScanning I2C bus"));
	for (i=1; i<127; i++) {
		// scan I2C bus
		Wire.beginTransmission(i); // transmit to PCF8574
		ret = Wire.endTransmission();
		if (ret == 0) {
			Serial.print(F("I2C dev at address : 0x"));
			Serial.println(i,16);
		}
	}
	Serial.println(F("Done"));

    // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds1, NUM_LEDS1);

	for (uint8_t i=0; i< NUM_LEDS1; i++) {
		leds1[i] = CRGB::Black;
	}

 	FastLED.show();
 	delay(500);

    leds1[0] = CRGB::Red;
    leds1[1] = CRGB::Green;
    leds1[2] = CRGB::Blue;
    leds1[3] = CRGB::White;
    
 	FastLED.show();

    delay(1000);

 	current_color = 0;
 	FastLED.setBrightness(0xFF);
 	/* Set button as input ports */
 	for( uint8_t i=0; i<8; i++) {
 	 	I2C_pinMode(i, INPUT);
  	}

 	for (uint8_t i = 0; i < NB_I2C_PORT; i++) {
 		i2c_ports[i].set_input_i2c();
 	}

 	current_time=0;

 	led_string1.init();
 	led_string2.init();
 	led_string3.init();
 	led_string4.init();
 	led_string5.init();
// 	led_string6.init();
 	op_button1.init();
 	op_button2.init();
 	op_button3.init();
 	op_button4.init();

 	led_string1.enable();
 	led_string2.enable();
 	led_string3.enable();
 	led_string4.enable();
 	led_string5.enable();
// 	led_string6.enable();
 	op_button1.enable();
 	op_button2.enable();
 	op_button3.enable();
 	op_button4.enable();

 	for (uint8_t hue=0; hue < 255; hue++) {
 		FastLED.showColor(CHSV(hue, 255, 180));
 		delay(10);
 	}
}


void loop() { 
	uint16_t i;
	delay(20);
//	Serial.println('L');
	/* All the background processing */
	current_time = millis();

	// Update I2C ports vales
	for (i=0; i<NB_I2C_PORT; i++) {
//		Serial.println(i);
		i2c_ports[i].read_i2c();
		i2c_ports[i].write_i2c();
	}

	FastLED.show();

	/* end of "background tasks */
	led_string1.run();
	led_string2.run();
	led_string3.run();
	led_string4.run();
	led_string5.run();
	led_string6.run();
 	op_button1.run();
 	op_button2.run();
 	op_button3.run();
 	op_button4.run();
	}

