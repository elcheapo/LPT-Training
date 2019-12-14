/*
 * dcc_timer.cpp
 *
 *  Created on: 25 avr. 2012
 *      Author: florrain
 */

#include "Arduino.h"
#include "dcc_timer.h"


// Offsets from TCCRA for the other registers in the same timer
#define tccrb (tccra+1)
#define tccrc (tccra+2)
#define ocra ((volatile uint16_t *) (tccra+8))
#define ocrb ((volatile uint16_t *) (tccra+0xa))
#define ocrc ((volatile uint16_t *) (tccra+0xc))
#define icr ((volatile uint16_t *) (tccra+6))
#define tcnt ((volatile uint16_t *) (tccra+4))



// Constructors ////////////////////////////////////////////////////////////////

DCC_timer::DCC_timer( volatile uint8_t *_tccra,
		volatile uint8_t *_timsk,
		volatile uint8_t *_tifr,
		volatile uint8_t *_ddr) {
	tccra = _tccra;
	timsk = _timsk;
	tifr = _tifr;
	ddr = _ddr;
	voie_on = false;
}

#ifdef USE_TIMER1
DCC_timer timer1 (&TCCR1A,&TIMSK1,&TIFR1,&DDRB); // Voie 1 Gare
ISR(TIMER1_OVF_vect) {
	timer1.timer_overflow_interrupt();
}
#endif

#ifdef USE_TIMER3
DCC_timer timer3 (&TCCR3A,&TIMSK3,&TIFR3,&DDRE); // Voie 1
#endif
#ifdef USE_TIMER4
DCC_timer timer4 (&TCCR4A,&TIMSK4,&TIFR4,&DDRH); // Voie 2
ISR(TIMER4_OVF_vect) {
	timer4.timer_overflow_interrupt();
}
#endif
#ifdef USE_TIMER5
DCC_timer timer5 (&TCCR5A,&TIMSK5,&TIFR5,&DDRL); // Voie 2 Gare
ISR(TIMER5_OVF_vect) {
	timer5.timer_overflow_interrupt();
}
#endif


void DCC_timer::begin(void){
		*tccra = 1 << WGM10| 0 << WGM11			// PWM Phase correct 8 bit 0-FF
				| 0 << COM1A0	| 0 << COM1A1		// PWM signal on OCRB or OCRC
				| 0 << COM1B0	| 0 << COM1B1
				| 0 << COM1C0	| 0 << COM1C1;

		*tccrb = 0<<WGM13	| 1 << WGM12
				| (1<<CS12) | (0<<CS11) | (0<<CS10);	//  prescaler / 256, source=16 MHz / 256 = 244 Hz
//		*timsk = 1<<OCIE1A; 				// OCRA timer interrupt for adc measurement
		*ocra = 10;
		*ocrb = 0;
		*ocrc = 0;
		if (IS_TIMER1) {
			*ddr |= T1_OCRA|T1_OCRB|T1_OCRC;
			*dcc_port &= ~(T1_OCRA|T1_OCRB|T1_OCRC); // start with output OCRA/B/C deactivated
		} else {
			*ddr |= T3_OCRA|T3_OCRB|T3_OCRC;
			*dcc_port &= ~(T3_OCRA|T3_OCRB|T3_OCRC); // start with output OCRA/B/C deactivated
		}
	voie_on = true;
}

void DCC_timer::end(void) {
	*timsk = 0; // disable timer interrupt
	*tccra = 0 << WGM10| 0 << WGM11
			| 0 << COM1A0	| 0 << COM1A1		// No output on OCxA
			| 0 << COM1B0	| 0 << COM1B1		// No output on OCxB
			| 0 << COM1C0	| 0 << COM1C1;		// No output on OCxC
	*tccrb = 0<<WGM13 | 0 << WGM12
			| (0<<CS12) | (0<<CS11) | (0<<CS10);// timer stopped, no clock
	if (IS_TIMER1) {
		*ddr |= T1_OCRA|T1_OCRB|T1_OCRC;
		*dcc_port &= ~(T1_OCRA|T1_OCRB|T1_OCRC); // turn off all signals
	} else {
		*ddr |= T3_OCRA|T3_OCRB|T3_OCRC;
		*dcc_port &= ~(T3_OCRA|T3_OCRB|T3_OCRC); // turn off all signals
	}
	voie_on = false;
}

void DCC_timer::analog_set_speed_and_direction(uint16_t speed, tdirection direction) {
	speed = speed >> 1;   // range for speed is 0 - 512
	speed = speed & 0xFF; // limit range to 0 - 255
	*ocra = 10; // adc measurement at the beginning of the pulse
	if (direction == off) {
		*tccra = 1 << WGM10| 0 << WGM11			// PWM Phase correct 8 bit 0-FF
				| 0 << COM1A0	| 0 << COM1A1		// No PWM signal
				| 0 << COM1B0	| 0 << COM1B1
				| 0 << COM1C0	| 0 << COM1C1;

		*tccrb = 0<<WGM13	| 1 << WGM12
				| (1<<CS12) | (0<<CS11) | (0<<CS10);	//  prescaler / 256, source=16 MHz / 256 = 244 Hz
		if (IS_TIMER1) {
			*ddr |= T1_OCRB|T1_OCRC;
			*dcc_port &= ~(T1_OCRB|T1_OCRC); // OCRB/C set to 0
		} else {
			*ddr |= T3_OCRB|T3_OCRC;
			*dcc_port &= ~(T3_OCRB|T3_OCRC); // OCRB/C set to 0
		}

	} else if (direction == backward) {
		*tccra = 1 << WGM10| 0 << WGM11			// PWM Phase correct 8 bit 0-FF
				| 0 << COM1A0	| 0 << COM1A1		// PWM signal non-inverted on OCRB
				| 0 << COM1B0	| 1 << COM1B1
				| 0 << COM1C0	| 0 << COM1C1;
		*tccrb = 0<<WGM13	| 1 << WGM12
				| (1<<CS12) | (0<<CS11) | (0<<CS10);	//  prescaler / 256, source=16 MHz / 256 = 244 Hz
		if (IS_TIMER1) {
			*ddr |= T1_OCRC;
			*dcc_port &= ~(T1_OCRC); // OCRC set to 0
		} else {
			*ddr |= T3_OCRC;
			*dcc_port &= ~(T3_OCRC); // OCRC set to 0
		}
		*ocrb = speed;
	} else {// forward
		*tccra = 1 << WGM10| 0 << WGM11			// PWM Phase correct 8 bit 0-FF
				| 0 << COM1A0	| 0 << COM1A1		// PWM signal inverted on OCRC
				| 0 << COM1B0	| 0 << COM1B1
				| 0 << COM1C0	| 1 << COM1C1;

		*tccrb = 0<<WGM13	| 1 << WGM12
				| (1<<CS12) | (0<<CS11) | (0<<CS10);	//  prescaler / 256, source=16 MHz / 256 = 244 Hz
		if (IS_TIMER1) {
			*ddr |= T1_OCRB;
			*dcc_port &= ~(T1_OCRB); // OCRB set to 0
		} else {
			*ddr |= T3_OCRB;
			*dcc_port &= ~(T3_OCRB); // OCRB set to 0
		}
		*ocrc = speed;
	}
}

uint16_t DCC_timer::analog_get_speed(void) {
	if ( (*tccra & (0 << COM1B0 | 1 << COM1B1)) == (0 << COM1B0 | 1 << COM1B1))
		return *ocrb * 2;
	else if ( (*tccra & (0 << COM1C0 | 1 << COM1C1)) == (0 << COM1C0 | 1 << COM1C1))
		return *ocrc * 2;
	else
		return 0;
}

tdirection DCC_timer::analog_get_direction(void) {
	if ( (*tccra & (0 << COM1B0 | 1 << COM1B1)) == (0 << COM1B0 | 1 << COM1B1)) return forward;
	if ( (*tccra & (0 << COM1C0 | 1 << COM1C1)) == (0 << COM1C0 | 1 << COM1C1)) return backward;
	return off;
}


