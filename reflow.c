#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "Ext\Extentions.h" /* Extentions library file */

#include "global.h"

#include "spi.h"			// include SPI support
#include "timerx8.h"		// include timer support
#include "MAX6675.h"
#include "zerocross.h"
#include "fireTriac.h"

#ifdef DEBUG_SER
	#include "vt100.h"		// include VT100 terminal support
	#include "rprintf.h"	// include printf function library
	#include "uart.h"		// include uart function library
//	#include "debug.h"		// include debug function library
#endif


/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <spam@thelasse.com> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Lars Andersson
 * ----------------------------------------------------------------------------
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

/*
 * Todo:
 * add SPI interface - DONE!
 * add UART interface - DONE!
 *
 * -write driver for MAX6675 - to be tested, hardware no complete yet
 * 		6675 max conversin time = 220mS
 *
 * -implement zeroCross & test - not complete, w.f. hardware (optocoupler)
 *
 * -implement timing delay. See motorola doc on triac pulses.
 *		-The closer to zerocross you get the longer pulse is needed 
 *		 as the voltage and therefore the current decreases.
 * -direct general program flow
 *
 */

//delay definition (MAX6675 read frequency) for debugging.
#define DELAY	600



static void avr_init(void);
//void incAngle(void);

u08 sensorDisconnected;		//global variable, extern -> global.h
static u16 tempData;



int main(void)
{
    avr_init();

    u16 i;
    for(;;)
    {
        // Tasks here.


		readMAX6675(&tempData);		// get data to tempData variable
#ifndef DEBUG_SIM
		//delay 250mS, safe update time for MAX6675
		for(i=0;i<DELAY;i++)
		{
			_delay_ms(1);	
		}	        
#endif

#ifdef DEBUG_SER
//	uartPrintfNum(10, 6,  TRUE, ' ',   1234);  -->  " +1234"
//	uartPrintfNum(16, 6, FALSE, '.', 0x5AA5);  -->  "..5AA5"
	if(sensorDisconnected)
	{
		rprintfProgStrM("Disconnected Probe!\r\n");
	}
	else
	{
		rprintf("temp: ");
		rprintfNum(10, 4,  FALSE, ' ',   tempData);
	}
	rprintf("\r");


#endif
    }
    
    return(0);
}

/*
void incAngle(void)
// invoked on timer1 output compareA
{
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

}
*/

static void avr_init(void)
// Initializes AVR controller
{
	MAX6675S_DDR |= BV(MAX6675_CSPIN);		//setup direction for CS pin
	DISABLE_6675;							// Set CS = high for MAX6675	

	initZerocross();						//init ext Int0

	initTriac();							//init Triac pin & direction

	//attach functions to timer interrupts
	timerAttach(TIMER0OUTCOMPAREA_INT, fireTriac);
	timerAttach(TIMER2OUTCOMPAREA_INT, stopTriac);
//	timerAttach(TIMER1OUTCOMPAREA_INT, incAngle);

	//enable output compare for timers
	sbi(TIMSK0, OCIE0A);
//	sbi(TIMSK1, OCIE1A);
	sbi(TIMSK2, OCIE2A);

	//set OCR values. 
	OCR0A = PHASE_ANGLE_LIMIT_HIGH;				//firing angle of triac
	OCR2A = 1;									//length of firing pulse
//	OCR1A = 100;								//temperature sample frequency

//	timer1SetPrescaler(TIMER_CLK_DIV1024);		//start temp. sampling


#ifndef DEBUG_SIM

	#ifdef DEBUG_SER
	// initialize the UART (serial port)
	uartInit();

	// set the baud rate of the UART for our debug/reporting output
	uartSetBaudRate(115200);

	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);

	// print a little intro message so we know things are working
	rprintf("Reflow!\r\n");
	#endif

	// initialize SPI interface
	spiInit();

#endif
	//enable interrupts
	sei();
	  
    return;
}
    
	
    
