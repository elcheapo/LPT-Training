/*
 * dcc_timer.h
 *
 *  Created on: 25 avr. 2012
 *      Author: florrain
 */

#ifndef DCC_TIMER_H_
#define DCC_TIMER_H_
#include "config.h"
// define which timers are used for DCC output
//#define USE_TIMER1
#define USE_TIMER3
//#define USE_TIMER4
//#define USE_TIMER5

// define some handy names for the states of the ISR
#define DOI_IDLE     (0)
#define DOI_PREAMBLE (1)
#define DOI_BSTART   (2)
#define DOI_BYTE     (3)
#define DOI_XOR      (4)
#define DOI_INTER_PACKET (5)
#define DOI_LAST_BIT (6)

typedef struct {
	uint8_t state;                            // current state
	uint8_t ibyte;                            // current index of byte in message
	uint8_t bitcount;							// current bit index n char
	uint8_t cur_byte;                         // current byte value
	uint8_t xor_byte;                         // actual check
	uint8_t repeat_ctr;						// current number of repeat
} doi;

/// This are timing definitions from NMRA
#define PERIOD_1   116L                  // 116us for DCC 1 pulse - do not change
//#define PERIOD_1   120L                  // 104us for DCC 1 pulse - do not change
#define PERIOD_0   232L                  // 232us for DCC 0 pulse - do not change
//#define PERIOD_0   200L                  // 200us for DCC 0 pulse - do not change

class DCC_timer
{
private:
	volatile uint8_t *tccra;
	volatile uint8_t *timsk;
	volatile uint8_t *tifr;
	volatile uint8_t *ddr;

	doi _doi_packet;
	bool voie_on;


public:
	DCC_timer( volatile uint8_t *tccra,
			volatile uint8_t *timsk,
			volatile uint8_t *tifr,
			volatile uint8_t *ddr
//			uint8_t * _timer_pol
			);
	void begin(void);
	void end(void);
	void analog_set_speed_and_direction(uint16_t speed, tdirection direction);
	bool voie_is_on(void);
//	void analog_set_speed(uint16_t speed);
	uint16_t analog_get_speed(void);
//	void analog_set_direction(tdirection direction);
	tdirection analog_get_direction(void);
};

#define dcc_port (ddr+1)
#define IS_TIMER1 ((uint16_t)(ddr) == 0x24)

#define T1_OCRA (0x20)
#define T1_OCRB (0x40)
#define T1_OCRC (0x80)

#define T3_OCRA (0x08)
#define T3_OCRB (0x10)
#define T3_OCRC (0x20)

inline bool DCC_timer::voie_is_on (void) {
	return voie_on;
}

// void dcc_send (void );

#ifdef USE_TIMER1
extern DCC_timer timer1;
#endif

#ifdef USE_TIMER3
extern DCC_timer timer3;
#endif

#ifdef USE_TIMER4
extern DCC_timer timer4;
#endif

#ifdef USE_TIMER5
extern DCC_timer timer5;
#endif

#endif /* DCC_TIMER_H_ */
