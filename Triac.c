#include "Triac.h"
#include "global.h"
#include <avr/io.h>
#include "timerx8.h"		// include timer support

enum { DOWN, UP };

void initTriac(void)
{
	STOP_TRIAC;										// set TriacPin High (Gate off)
	cbi(TRIAC_DDR,TRIAC_PIN);						// set Triac Pin to output (0)
}



/**************************************************************
 * Attached to timer1 overflow interrupt. 
 * When Timer1 overflows the delay determines the phase-angle
 * Sets TriacFire pin low and starts Timer2 and stops itself
 * Timer2 resets TricFire pin and stops itself
 *
 *
 **************************************************************/
void fireTriac(void)
{
	FIRE_TRIAC;										// turn on Triac Gate
	timer2SetPrescaler(TIMERRTC_CLK_DIV1024);		// start timer2, when it expires turn off triac gate. This determines the Gate pulselength.

	timer1SetPrescaler(TIMER_CLK_STOP);				// stop & reset the timer that called this function
	TCNT1 = 0;

#ifdef WALK_PHASEANGLE										// Testmode changes the phase-angle by 75 timerticks every time the triac fires
    static uint8_t direction;						// It will increment until it reaches the limit and then start decrementing, until 
													// it reaches the other limit

    switch (direction)
    {
        case UP:
            if((OCR1A+=WALK_PHASEANGLE_STEP) > PHASE_ANGLE_LIMIT_HIGH)
                direction = DOWN;
            break;

        case DOWN:
            if((OCR1A-=WALK_PHASEANGLE_STEP) < PHASE_ANGLE_LIMIT_LOW)
                direction = UP;
            break;
    }
#endif
}

/**************************************************************
 * Attached to timer2 overflow interrupt.
 * Sets TriacFire pin high (turn off gate) and stops timer2
 *
 *
 **************************************************************/
void stopTriac(void)
{
	STOP_TRIAC;											// turn off Triac Gate
	timer2SetPrescaler(TIMERRTC_CLK_STOP);				//stop & reset timer2
	TCNT2 = 0;

}
