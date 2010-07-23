/************************************************************
 * Hardware fires the interrupt as a rising edge when AC-line
 * passes zero, both directions (pos to neg and neg to pos) verified?
 * Make document with screenshot.
 *
 *
 *
 *
 *
 *
 *
 ************************************************************/

 /* todo:
  * -check and verify pulse to int with oscilloscope.
  * -measure pulsewidth and compare to AC waveform
  * -Get optocoupler (today!) from Elfa:
  *		Will use:	TLP620	Elfa:75-362-04
  *
  */

#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>

#include "timerx8.h"
#include "global.h"


void initZerocross(void)
{
#ifdef DEBUG_SIM

#endif
	DDRD |= BV(PD5);					// set as input
	//setup INT0 to fire on rising edge:
	EICRA |= BV(ISC00) | BV(ISC01);		//set sense control, fire on rising edge of INT0
	EIMSK |= BV(INT0);					//enable INT0

	return;

}

/*********************************************************************
 * Zerocross interrupt. This interrupt starts a timer. 100Hz
 * When the timer reaches it output compare value the TRIAC is fired
 * The TRIAC is conducting until the AC-line crosses 0V
 *********************************************************************/
SIGNAL (SIG_INTERRUPT0)
{
#ifdef DEBUG_SIM

#endif
//    static u08 skipcounter;
	skipcounter++;

    static u08 even = 0;

	//zero cross happened, start timer
	//timer0SetPrescaler(TIMER_CLK_DIV1024);
    if (APP_STATUS_REG & BV(HALF_PHASE)){
        if(even){                                  // only fire every other half-period (only positive or negative phase)
            timer1SetPrescaler(TIMER_CLK_DIV8);
            even = 0;
        }else{
            even = 1;
        }
        
    }else if(!(APP_STATUS_REG & BV(SKIP_PHASE))){
        timer1SetPrescaler(TIMER_CLK_DIV8);
    }else
    
    if(!(skipcounter % skips)) {
        timer1SetPrescaler(TIMER_CLK_DIV8);
    }
        
	
#ifndef PID_SYNC_TO_SAMPLE    	
	//set flag to update PID loop
	SET_PID_FLAG;
#endif
	return;
}
