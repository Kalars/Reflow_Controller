/************************************************************
 * Hardware fires the interrupt as a rising edge when AC-line
 * passes zero, both directions (pos to neg and neg to pos)
 * 
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
	DDRD |= BV(PD5);

	//DDRD |= BV(PD2);					// INT0 = input
	//setup INT0 to fire on rising edge:
	EICRA |= BV(ISC00) | BV(ISC01);		//set sense control, fire on rising edge of INT0
	EIMSK |= BV(INT0);					//enable INT0



	return;

}

SIGNAL (SIG_INTERRUPT0)
{
#ifdef DEBUG_SIM

#endif
	//zero cross happened, start timer
	//timer0SetPrescaler(TIMER_CLK_DIV1024);
	timer1SetPrescaler(TIMER_CLK_DIV8);

	//debug
	PIND |= BV(PD5);

	return;
}
