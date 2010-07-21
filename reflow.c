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
 * -write driver for MAX6675 - to be tested, hardware no complete yet -Done!
 * 		6675 max conversin time = 220mS
 *
 * -implement zeroCross & test - not complete, w.f. hardware (optocoupler) -Done!
 *
 * -implement timing delay. See motorola doc on triac pulses.
 *		-The closer to zerocross you get the longer pulse is needed 
 *		 as the voltage and therefore the current decreases.
 * -direct general program flow
 *
 */

//delay definition (MAX6675 read frequency) for debugging.
#define DELAY	300
//#define RX_DBG


static void avr_init(void);
void sample(void);

u08 sensorDisconnected;		//global variable, extern via global.h


s16 _PTERM;
s16 _ITERM;
s16 _DTERM;

s16 _ERROR;
s16 _DUMMY;
static u16 tempData;
u32 skips;
u32 skipcounter;			// used by app to skip x amount of cycles. ex: if(!(skipcounter % 3)){fire;}

//char app_status;



//SPid PID_data;
static pidData_t PID_data;

	u08 actualTemp;
	u08 command;
	s16 output;
	//u08 RS232data;

int main(void)
{
    avr_init();
u08 even;

//    u16 i;
//u08 temp;
	command = 60;
//	actualTemp = 85;

//	u16 temp;
    for(;;)
    {
        // Tasks here.

	if(APP_STATUS_REG & BV(DO_PID))
	{
		#ifdef REG_PID	
		//update PID
		output = pid_Controller(command, tempData, &PID_data);
#ifndef RX_DBG
		rprintf("PID raw: ");
		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();
#endif
		//invert PID
		output = PHASE_ANGLE_LIMIT_HIGH - output;
#ifndef RX_DBG
		rprintf("PID scaled: ");
		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();
#endif
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


	if (APP_STATUS_REG & BV(DO_SAMPLE))
	{
		readMAX6675(&tempData);		// get data to tempData variable
		CLEAR_SAMPLE_FLAG;
	//debug, invert pin
	PIND ^= BV(PD5);

	//update PID
//	output = pid_Controller(command, tempData, &PID_data);

	//invert PID
//	output = PHASE_ANGLE_LIMIT_HIGH - output;
	}




// Limit to within 1 half-phase:
	if((output > PHASE_ANGLE_LIMIT_HIGH) || (output < 0)){
#ifndef RX_DBG
		rprintf("Max out");rprintfCRLF();
#endif
		output = PHASE_ANGLE_LIMIT_HIGH;
	}
	
	else if ((output < PHASE_ANGLE_LIMIT_LOW)){// || (output < 0))
#ifndef RX_DBG
		rprintf("Min out");rprintfCRLF();
#endif
		output = PHASE_ANGLE_LIMIT_LOW;
	}



//OCR1A = PHASE_ANGLE_LIMIT_LOW;
//OCR1A = PHASE_ANGLE_LIMIT_HIGH;


#ifndef WALK_PHASEANGLE
		OCR1A = output;
#endif

#ifdef DEBUG_SER
//	uartPrintfNum(10, 6,  TRUE, ' ',   1234);  -->  " +1234"
//	uartPrintfNum(16, 6, FALSE, '.', 0x5AA5);  -->  "..5AA5"

//	rprintfNum(10, 4,  FALSE, ' ',   tempData);//		rprintfCRLF();
#ifndef RX_DBG
	if(sensorDisconnected)
	{
		rprintfProgStrM("Disconnected Probe!\r\n");
	}
	else
	{
		rprintf("Process Value: ");
		rprintfNum(10, 4,  FALSE, ' ',   tempData);		rprintfCRLF();

		rprintf("Target Value: ");
		rprintfNum(10, 4,  FALSE, ' ',   command);		rprintfCRLF();

		rprintf("PID Phaselimit: ");
		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();

		rprintf("OCR1A: ");
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
	if(even++ == 10){
		vt100ClearScreen();
		even = 0;
	}
#endif //#ifndef RX_DBG

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
	OCR1A = PHASE_ANGLE_LIMIT_HIGH;				//temperature sample frequency

	timer0SetPrescaler(TIMER_CLK_DIV1024);		//start temp. sampling

/*************************************** 
  * Timer0 runs at 24Mhz/1024 = 
  *
  * 
  *
  ***************************************/

#ifndef DEBUG_SIM
	#ifdef DEBUG_SER
	// initialize the UART (serial port)
	uartInit();

	// set the baud rate of the UART for our debug/reporting output
	uartSetBaudRate(57600);

	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);

	// print a little intro message so we know things are working
	rprintf("Reflow!\r\n");

//	vt100SetCursorMode(1);
//	vt100SetAttr(VT100_BLINK_OFF);
//	vt100SetAttr(VT100_USCORE_OFF);
//	vt100SetAttr(VT100_REVERSE);

	vt100ClearScreen();
	#endif

	// initialize SPI interface
	spiInit();
#endif

//	SET_HALF_PHASE;
//	SET_SKIP_PHASE;
//	skips = 15;
	//enable interrupts
	sei();


    return;
}
    
	
    
void sample(void)
{
//vt100ClearScreen();

	if(timer0GetOverflowCount() == 17){		// After 17 overflows we set a flag that will tell main() to read the
											// Temperature off the MAX6675
	timer0ClearOverflowCount();				// Clear timer0's overflow counter. 	
	SET_SAMPLE_FLAG;						//set sample flag and reset overflow counter
	}
	//vt100SetAttr(VT100_REVERSE_OFF);
	/*
	#define VT100_ATTR_OFF		0
#define VT100_BOLD			1
#define VT100_USCORE		4
#define VT100_BLINK			5
#define VT100_REVERSE		7
#define VT100_BOLD_OFF		21
#define VT100_USCORE_OFF	24
#define VT100_BLINK_OFF		25
#define VT100_REVERSE_OFF	27
*/

}
