#include "fireTriac.h"
#include "global.h"
#include <avr/io.h>
#include "timerx8.h"		// include timer support

enum { DOWN, UP };

void initTriac(void)
{
	//set TriacPin High
	STOP_TRIAC;

	//set Triac Pin to output (1)
	sbi(TRIAC_DDR,TRIAC_PIN);



}



/* Attached to timer0 overflow interrupt.
 * Sets TriacFire pin low and starts Timer2 and stops itself
 * Timer2 resets TricFire pin and stops itself
 *
 *
 */
void fireTriac(void)
{
	FIRE_TRIAC;

	//start timer2
	timer2SetPrescaler(TIMERRTC_CLK_DIV1024);

	//stop & reset timer0
	timer0SetPrescaler(TIMER_CLK_STOP);
	TCNT0 = 0;

#ifdef TESTMODE
    static uint8_t direction;

    switch (direction)
    {
        case UP:
            if (++OCR0A == PHASE_ANGLE_LIMIT_HIGH)
                direction = DOWN;
            break;

        case DOWN:
            if (--OCR0A == PHASE_ANGLE_LIMIT_LOW)
                direction = UP;
            break;
    }
#endif


}

/* Attached to timer2 overflow interrupt.
 * Sets TriacFire pin high and stops itself
 *
 *
 */
void stopTriac(void)
{
	STOP_TRIAC;

	//stop & reset timer2
	timer2SetPrescaler(TIMERRTC_CLK_STOP);
	TCNT2 = 0;

}
