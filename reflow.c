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
#include "Triac.h"

#ifdef REG_PID
	#include "pid.h"
#endif
#ifdef REG_PD
	#include "pd.h"
#endif



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
#define DELAY	300



static void avr_init(void);
void sample(void);

u08 sensorDisconnected;		//global variable, extern via global.h


s16 _PTERM;
s16 _ITERM;
s16 _DTERM;

s16 _ERROR;
s16 _DUMMY;
static u16 tempData;



//SPid PID_data;
static pidData_t PID_data;

	u08 actualTemp;
	u08 command;
	s16 output;
	u08 RXdata;

int main(void)
{
    avr_init();


//    u16 i;

	command = 100;
//	actualTemp = 85;

//	u16 temp;
    for(;;)
    {
        // Tasks here.

		if (uartReceiveByte(&RXdata))
		{	//reset integrator
 			if(RXdata == 13)
			PID_data.sumError = 0;
		}

	

	#ifdef DEBUG_SIM
		tempData = actualTemp;
	#endif


	if(SAMPLE_STATUS_REG & BV(DO_PID))
	{
		#ifdef REG_PID	
		//update PID
		output = pid_Controller(command, tempData, &PID_data);

		//invert PID
		output = PHASE_ANGLE_LIMIT_HIGH - output;
		#endif

		#ifdef REG_PD

		#endif
//		actualTemp +=10;
//		OCR0A = output;
		CLEAR_PID_FLAG;

	}


/*
		if((temp = 0xFFFF-output) > 30000)
		{	temp = 30000;}
		else if (temp < 1000)
		{	temp = 1000;}
*/


	if (SAMPLE_STATUS_REG & BV(DO_SAMPLE))
	{
		readMAX6675(&tempData);		// get data to tempData variable
		CLEAR_SAMPLE_FLAG;
	//debug
	PIND |= BV(PD5);


	//update PID
//	output = pid_Controller(command, tempData, &PID_data);

	//invert PID
//	output = PHASE_ANGLE_LIMIT_HIGH - output;
	}



		rprintf("PID out: ");
		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();



//		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();



//		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();

	if((output > PHASE_ANGLE_LIMIT_HIGH) || (output < 0))
	{	output = PHASE_ANGLE_LIMIT_HIGH;}
	
	else if ((output < PHASE_ANGLE_LIMIT_LOW))// || (output < 0))
	{	output = PHASE_ANGLE_LIMIT_LOW;}



	OCR1A = output;



#ifdef DEBUG_SER
//	uartPrintfNum(10, 6,  TRUE, ' ',   1234);  -->  " +1234"
//	uartPrintfNum(16, 6, FALSE, '.', 0x5AA5);  -->  "..5AA5"

//	rprintfNum(10, 4,  FALSE, ' ',   tempData);//		rprintfCRLF();

	if(sensorDisconnected)
	{
		rprintfProgStrM("Disconnected Probe!\r\n");
	}
	else
	{
//		rprintf("temp: ");
		rprintfNum(10, 4,  FALSE, ' ',   tempData);		rprintfCRLF();

//		rprintf("PID out: ");
//		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();

//		rprintf("OCR1A: ");
		rprintfNum(10, 6,  FALSE, ' ',   OCR1A);	rprintfCRLF();

//-----------
//		rprintf("dummy: ");
//		rprintfNum(10, 6,  TRUE, ' ',   _DUMMY);		rprintfCRLF();

		rprintf("E ");
		rprintfNum(10, 4,  TRUE, ' ',   _ERROR);		rprintfCRLF();

		rprintf("P ");
		rprintfNum(10, 8,  TRUE, ' ',   _PTERM);		rprintfCRLF();

		rprintf("I ");
		rprintfNum(10, 8,  TRUE, ' ',   _ITERM);		rprintfCRLF();

		rprintf("D ");
		rprintfNum(10, 8,  TRUE, ' ',   _DTERM);		rprintfCRLF();
//-----------

	}

	vt100SetCursorPos(0, 0);



#endif
    }
    
    return(0);
}




static void avr_init(void)
// Initializes AVR controller
{
	MAX6675S_DDR |= BV(MAX6675_CSPIN);		//setup direction for CS pin
	DISABLE_6675;							// Set CS = high for MAX6675	

	initZerocross();						//init ext Int0

	initTriac();							//init Triac pin & direction


#ifdef REG_PID
//	initPID(&PID_data, 1, 0, 10, 10, -10);

//void pid_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, struct PID_DATA *pid);
	pid_Init(P_FACTOR, I_FACTOR, D_FACTOR, &PID_data);
#endif
#ifdef REG_PD
	initPD(&PD_data, 800, 1000);
#endif


	//attach functions to timer interrupts
	timerAttach(TIMER0OVERFLOW_INT, sample);
	timerAttach(TIMER2OUTCOMPAREA_INT, stopTriac);
	timerAttach(TIMER1OUTCOMPAREA_INT, fireTriac);

	//enable output compare for timers
	sbi(TIMSK0, TOIE0);
	sbi(TIMSK1, OCIE1A);
	sbi(TIMSK2, OCIE2A);

	//set OCR values. 
//	OCR0A = PHASE_ANGLE_LIMIT_HIGH;				//firing angle of triac
	OCR2A = 1;									//length of firing pulse
	OCR1A = 30500;								//temperature sample frequency

	timer0SetPrescaler(TIMER_CLK_DIV1024);		//start temp. sampling


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

	vt100ClearScreen	  ();
	#endif

	// initialize SPI interface
	spiInit();
#endif
	//enable interrupts
	sei();


    return;
}
    
	
    
void sample(void)
{


	if(timer0GetOverflowCount() == 17)
	{
		//set sample flag and reset overflow counter
	
	timer0ClearOverflowCount();	///< Clear timer0's overflow counter. 	
	SET_SAMPLE_FLAG;
	}

}
