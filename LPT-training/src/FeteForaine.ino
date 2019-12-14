
#include <stdint.h>
#include <Wire.h>
#include <SPI.h>
#include <FastLED.h>
#include <MD_YX5300.h>
#include <dcc_timer.h>
//#include <ssd1306.h>
//#include "nano_gfx.h"
#include "i2c_port.h"
#include "scenario.h"
#include "pixeltypes.h"

//#include "sova.h"


#define CURRENT_DETECT 41
// How many leds in your strip?
// Mas de cocagne 24+16
#define NUM_LEDS1 40
// Palais des mirages (2) / Palais des glaces (3) + courone grande roue (12)
#define NUM_LEDS2 17
// grande roue gauche 
#define NUM_LEDS3 26
// Grande roue droite 
#define NUM_LEDS4 26
// Petit bandeau - 57
#define NUM_LEDS5 56
// Grand bandeau 144
#define NUM_LEDS6 144 
// Mas de cocagne 2 12
#define NUM_LEDS7 12



#define FET1 45
#define FET2 44
#define FET3 43
#define FET4 42

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN1 22 // Grande Roue Gauche
#define DATA_PIN2 23 // Palais glace / mirage
#define DATA_PIN3 24 // Grande Roue Droite
#define DATA_PIN4 25 // Petit mas
#define DATA_PIN5 26 // Grand mas
#define DATA_PIN6 27 // Bandeau 1
#define DATA_PIN7 28 // Bandeau 2

// 22 : Couroune derriere la grande roue + Grande roue droite
// 23 : grande roue gauche
// 24 : ?
// 25 : ? 
// 26 : ?
// 27 : 

// define which button control which manège
#define B_BALANCELLE 10 // Balancelle
#define B_TOUR 20 // Tour
#define B_ROUE 12 // Grande roue
#define B_CHEVEAUX 13 // Petit chevaux - musique seule
#define B_TRAIN   14
#define B_CHAISES 11 // Chaises
#define B_WATERPARK 21
#define B_EMERG 24
// define relay 
#define R_BALANCELLE 5 // Balancelle
#define R_TOUR 4 // Tour
#define R_ROUE 3 // Grande Roue
#define R_WATERPARK 1 // Pirates
#define R_CHAISES 7 // Chaises
//#define R_WATERPARK 5 

// Operator buttons on the back

#define OP_BUTTON1 30
#define OP_BUTTON2 31
#define OP_BUTTON3 32
#define OP_BUTTON4 33

#define MANEGE_DURATION 30000

#define HIGH_VOLUME 30
#define MID_VOLUME 20
#define LOW_VOLUME 10

MD_YX5300 mp3(0,0);
bool bUseCallback = true; // use callbacks?
bool bUseSynch = false;   // use synchronous?
uint8_t mp3_status;
uint8_t requested_music;
uint8_t stop_request;

#define TRACK_BALANCELLE 4
#define TRACK_TOUR 2
#define TRACK_ROUE 6
#define TRACK_CHEVAUX 8
#define TRACK_CHAISES 3
#define TRACK_WATERPARK 5

#define MP3_DEFAULT 1 // Default track to play ...
// Define 3 I2C extender
#define NB_I2C_PORT 4
I2c_Port i2c_ports[NB_I2C_PORT]= {I2c_Port(0x20), I2c_Port(0x21), I2c_Port(0x22), I2c_Port(0x23)};

uint8_t led11_cycle;
uint8_t led12_cycle;
uint8_t led21_cycle;
uint8_t led22_cycle;
uint8_t led34_cycle;
uint8_t led5_cycle;
uint8_t led6_cycle;
uint8_t train_cycle;
uint8_t manege1_cycle;
uint8_t manege2_cycle;
uint8_t manege3_cycle;
uint8_t manege4_cycle;
uint8_t manege5_cycle;
uint8_t manege6_cycle;
uint8_t op_button1_cycle;
uint8_t op_button2_cycle;
uint8_t op_button3_cycle;
uint8_t op_button4_cycle;
uint8_t mp3_cycle;

// Define the array of leds
CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGB leds4[NUM_LEDS4];
CRGB leds5[NUM_LEDS5];
CRGB leds6[NUM_LEDS6];
CRGB leds7[NUM_LEDS7];



uint32_t current_time;

uint8_t current_color;

#include "helpers.h"
void setup() { 
	uint8_t i,ret;
	
	Wire.begin();
	Serial.begin(230400);

	// Turn Off Relays
	Wire.beginTransmission(0x20); // transmit to PCF8574
	Wire.write(0x00);
	Wire.endTransmission();

	pinMode(CURRENT_DETECT, INPUT);           // set pin to input for current detector
	digitalWrite(CURRENT_DETECT, HIGH);       // turn on pullup resistors

	delay(2000);

	timer3.end();
	// MP3 init
	mp3.begin();
	setCallbackMode(bUseCallback);
	setSynchMode(bUseSynch);

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
    FastLED.addLeds<WS2812B, DATA_PIN5, GRB>(leds1, NUM_LEDS1);  // Mas double (40)
    FastLED.addLeds<WS2812B, DATA_PIN3, GRB>(leds2, NUM_LEDS2);	 // Palais glace/mirage + courone grande roue 
    FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds3, NUM_LEDS3);  // Grande Roue gauche
    FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds4, NUM_LEDS4);  // Grande Roue droite
    FastLED.addLeds<WS2812B, DATA_PIN7, GRB>(leds5, NUM_LEDS5);  // petit bandeau
    FastLED.addLeds<WS2812B, DATA_PIN6, GRB>(leds6, NUM_LEDS6);  // Grand bandeau OK
    FastLED.addLeds<WS2812B, DATA_PIN4, GRB>(leds7, NUM_LEDS7);  // Petit mas OK

	for (uint8_t i=0; i< NUM_LEDS1; i++) {
		leds1[i] = CRGB::Black;
	}
	for (uint8_t i=0; i< NUM_LEDS2; i++) {
		leds2[i] = CRGB::Black;
	}
	for (uint8_t i=0; i< NUM_LEDS3; i++) {
		leds3[i] = CRGB::Black;
	}
	for (uint8_t i=0; i< NUM_LEDS4; i++) {
		leds4[i] = CRGB::Black;
	}
	for (uint8_t i=0; i< NUM_LEDS5; i++) {
		leds5[i] = CRGB::Black;
	}
	for (uint8_t i=0; i< NUM_LEDS6; i++) {
		leds6[i] = CRGB::Black;
	}
	for (uint8_t i=0; i< NUM_LEDS7; i++) {
		leds7[i] = CRGB::Black;
	}



 	FastLED.show();
 	delay(500);

    leds1[0] = CRGB::Red;
    leds1[1] = CRGB::Green;
    leds1[2] = CRGB::Blue;
    leds1[3] = CRGB::White;
    
    leds2[0] = CRGB::Red;
    leds2[1] = CRGB::Green;
    leds2[2] = CRGB::Blue;
    leds2[3] = CRGB::White;
    leds2[4] = CRGB::White;
    
    leds3[0] = CRGB::Red;
    leds3[1] = CRGB::Green;
    leds3[2] = CRGB::Blue;
    leds3[3] = CRGB::White;
    leds3[4] = CRGB::White;
    leds3[5] = CRGB::White;
    
    leds4[0] = CRGB::Red;
    leds4[1] = CRGB::Green;
    leds4[2] = CRGB::Blue;
    leds4[3] = CRGB::White;
    leds4[4] = CRGB::White;
    leds4[5] = CRGB::White;
    leds4[6] = CRGB::White;
    
    leds5[0] = CRGB::Red;
    leds5[1] = CRGB::Green;
    leds5[2] = CRGB::Blue;
    leds5[3] = CRGB::White;
    leds5[4] = CRGB::White;
    leds5[5] = CRGB::White;
    leds5[6] = CRGB::White;
    leds5[7] = CRGB::White;
    
    
    leds6[0] = CRGB::Red;
    leds6[1] = CRGB::Green;
    leds6[2] = CRGB::Blue;
    leds6[3] = CRGB::White;
    leds6[4] = CRGB::White;
    leds6[5] = CRGB::White;
    leds6[6] = CRGB::White;
    leds6[7] = CRGB::White;
    leds6[8] = CRGB::White;
    
    leds7[0] = CRGB::Red;
    leds7[1] = CRGB::Green;
    leds7[2] = CRGB::Blue;
    leds7[3] = CRGB::White;
    leds7[4] = CRGB::White;
    leds7[5] = CRGB::White;
    leds7[6] = CRGB::White;
    leds7[7] = CRGB::White;
    leds7[8] = CRGB::White;
    leds7[9] = CRGB::White;
    
 	FastLED.show();

    delay(1000);

#if 0
    for( uint16_t i=0; i<NUM_LEDS1; i++) {
    	leds1[i] = CRGB::Blue;
    }
 	for( uint16_t i=0; i<NUM_LEDS2; i++) {
  	  leds2[i] = CRGB::Red;
  	}
    for( uint16_t i=0; i<NUM_LEDS3; i++) {
    	leds3[i] = CRGB::Green;
    	leds4[i] = CRGB::Yellow;
    }
    for( uint16_t i=0; i<NUM_LEDS5; i++) {
    	leds5[i] = CRGB::Orange;
    }
    for( uint16_t i=0; i<NUM_LEDS6; i++) {
    	leds6[i] = CRGB::Purple;
    }
    for( uint16_t i=0; i<NUM_LEDS7; i++) {
    	leds7[i] = CRGB::Pink;
    }

 	FastLED.show();

   delay(5000);

#endif

 	current_color = 0;
 	FastLED.setBrightness(0xFF);
 	/* Set button as input ports */
 	for( uint16_t i=0; i<5; i++) {
 	 	I2C_pinMode(10+i, INPUT);
 	 	I2C_pinMode(20+i, INPUT);
 	 	I2C_pinMode(30+i, INPUT);
  	}

 	for (uint8_t i = 0; i < NB_I2C_PORT; i++) {
 		i2c_ports[i].set_input_i2c();
 	}
	I2C_digitalWrite(R_BALANCELLE, LOW);
	I2C_digitalWrite(R_TOUR, LOW);
	I2C_digitalWrite(R_ROUE, LOW);
//	I2C_digitalWrite(R_CHEVEAUX, LOW);
	I2C_digitalWrite(R_CHAISES, LOW);
	I2C_digitalWrite(R_WATERPARK, LOW);
	

 	current_time=0;

 	led_string11.init(led11_cycle);
 	led_string12.init(led12_cycle);
 	led_string21.init(led21_cycle);
 	led_string22.init(led22_cycle);
 	led_string34.init(led34_cycle);
 	led_string5.init(led5_cycle);
 	led_string6.init(led6_cycle);
 	train_control.init(train_cycle);
 	op_manege1.init(manege1_cycle);
 	op_manege2.init(manege2_cycle);
 	op_manege3.init(manege3_cycle);
 	op_manege4.init(manege4_cycle);
 	op_manege5.init(manege5_cycle);
 	op_manege6.init(manege6_cycle);
 	op_button1.init(op_button1_cycle);
	op_button2.init(op_button1_cycle);
	op_button3.init(op_button1_cycle);
	op_button4.init(op_button1_cycle);
	op_mp3.init(mp3_cycle);

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
 	op_button1.enable(op_button1_cycle);
 	op_button2.enable(op_button1_cycle);
 	op_button3.enable(op_button1_cycle);
 	op_button4.enable(op_button1_cycle);
	op_mp3.enable(mp3_cycle);

 	for (uint8_t hue=0; hue < 255; hue++) {
 		FastLED.showColor(CHSV(hue, 255, 180));
 		delay(10);
 	}
 	/* Turn on FET */
 	pinMode(FET1,OUTPUT);
 	pinMode(FET2,OUTPUT);
 	pinMode(FET3,OUTPUT);
 	pinMode(FET4,OUTPUT);
 	digitalWrite(FET1, LOW);
 	digitalWrite(FET2, HIGH);
 	digitalWrite(FET3, LOW);
 	digitalWrite(FET4, HIGH);
 	mp3.volume(MID_VOLUME);

 	delay (1000);

}


void loop() { 
	uint16_t i;
	delay(20);
//	Serial.println('L');
	/* All the background processing */
	current_time = millis();
	// MP3 player
	mp3.check();

	// Update I2C ports vales
	for (i=0; i<NB_I2C_PORT; i++) {
//		Serial.println(i);
		i2c_ports[i].read_i2c();
		i2c_ports[i].write_i2c();
	}
#if 0
	// Simulate Button Press with character received over Serial
	if (Serial.available() > 0) {
		// read the incoming byte:
		i = Serial.read();
		switch (i) {
		case '1':
			i2c_ports[1].read_value &= 0xfe; // Button 1 pressed
			break;
		case '2':
			i2c_ports[1].read_value &= 0xfd; // Button 2 pressed
			break;
		case '3':
			i2c_ports[1].read_value &= 0xfb; // Button 3 pressed
			break;
		case '4':
			i2c_ports[1].read_value &= 0xf7; // Button 4 pressed
			break;
		case '5':
			i2c_ports[1].read_value &= 0xef; // Button 5 pressed
			break;
		case '6':
			i2c_ports[2].read_value &= 0xfe; // Button 6 pressed
			break;
		case '7':
			i2c_ports[2].read_value &= 0xfd; // Button 7 pressed
			break;
		case '8':
			i2c_ports[2].read_value &= 0xfb; // Button 8 pressed
			break;
		case '9':
			i2c_ports[2].read_value &= 0xf7; // Button 9 pressed
			break;
		case '0':
			i2c_ports[2].read_value &= 0xef; // Button 10 pressed
			break;
		case 'A':
			i2c_ports[3].read_value &= 0xfe; // Button 0 pressed
			break;
		case 'B':
			i2c_ports[3].read_value &= 0xfd; // Button 0 pressed
			break;
		case 'C':
			i2c_ports[3].read_value &= 0xfb; // Button 0 pressed
			break;
		case 'D':
			i2c_ports[3].read_value &= 0xf7; // Button 0 pressed
			break;
		default:
			break;
		}
	}
#endif
	if (Serial.available() > 0) {
		// read the incoming byte:
		i = Serial.read();
		switch (i) {
		case '1'...'9':
			requested_music = i-'0';
			break;
		case 'A':
			i2c_ports[3].read_value &= 0xfe; // Button 0 pressed
			break;
		case 'B':
			i2c_ports[3].read_value &= 0xfd; // Button 0 pressed
			break;
		case 'C':
			i2c_ports[3].read_value &= 0xfb; // Button 0 pressed
			break;
		case 'D':
			i2c_ports[3].read_value &= 0xf7; // Button 0 pressed
			break;
		default:
			break;
		}
	}

	FastLED.show();

	/* end of "background tasks */
	led_string11.run(led11_cycle); // Grand mas de cocagne
	led_string12.run(led12_cycle);
	led_string21.run(led21_cycle);
	led_string22.run(led22_cycle);
	led_string34.run(led34_cycle);
	led_string5.run(led5_cycle);
	led_string6.run(led6_cycle);
	train_control.run(train_cycle);
 	op_manege1.run(manege1_cycle);
 	op_manege2.run(manege2_cycle);
 	op_manege3.run(manege3_cycle);
 	op_manege4.run(manege4_cycle);
 	op_manege5.run(manege5_cycle);
 	op_manege6.run(manege6_cycle);
 	op_button1.run(op_button1_cycle);
 	op_button2.run(op_button1_cycle);
 	op_button3.run(op_button1_cycle);
 	op_button4.run(op_button1_cycle);
	op_mp3.run(mp3_cycle);

	
	}

