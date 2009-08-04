//header file for Triac firing and initialization
#ifndef TRIAC_H
#define TRIAC_H

#define TRIAC_DDR	DDRC
#define TRIAC_PORT	PORTC
#define TRIAC_MASK	0x20
#define TRIAC_PIN	PC5


#define FIRE_TRIAC		cbi(TRIAC_PORT,TRIAC_PIN)		// Fire Triac: Turn on Gate: Pin Low
#define STOP_TRIAC		sbi(TRIAC_PORT,TRIAC_PIN)		// Turn off Gate: Pin high


void fireTriac(void);

void initTriac(void);

void stopTriac(void);

#endif
