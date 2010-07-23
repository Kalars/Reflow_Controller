#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "Ext\Extentions.h" /* Extentions library file */
#include <stdio.h>

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
    #include "cmdline.h"	// include cmdline function library
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
void goCmdline(void);
void set_target_temp(void);

u08 sensorDisconnected;		//global variable, extern via global.h

double _PTERM;
double _ITERM;
double _DTERM;

double _ERROR;
double _DUMMY;
static double tempData;
u32 skips;
u32 skipcounter;			// used by app to skip x amount of cycles. ex: if(!(skipcounter % 3)){fire;}
u08 c;

//char app_status;



//SPid PID_data;
static pidData_t PID_data;

	double actualTemp;
	double command;
	double output;
	//u08 RS232data;

int main(void)
{
    u08 even;
    command = 100;
    avr_init();

    for(;;)
    {
        // Tasks here.
       	vt100SetCursorPos(3, 0);

        if (APP_STATUS_REG & BV(DO_SAMPLE))
    	{
    		readMAX6675(&tempData);		// get data to tempData variable
    		CLEAR_SAMPLE_FLAG;
    	
            PIND ^= BV(PD5);    //debug, invert pin every sample
    
#ifdef PID_SYNC_TO_SAMPLE
    	//update PID
    	output = pid_Controller(command, tempData, &PID_data);
    
    	//invert PID
        //output = PHASE_ANGLE_LIMIT_HIGH - output;
#endif
        }

    	if(APP_STATUS_REG & BV(DO_PID))
    	{
    		#ifdef REG_PID	
    		//update PID
    		output = pid_Controller(command, tempData, &PID_data);
#ifndef CMDLINE
    #ifndef RX_DBG
    		rprintf("PID raw: ");
    		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();
    #endif
    		//invert & limit PID
            if(output > 29700) output = 700;
            //output = PHASE_ANGLE_LIMIT_HIGH - output;
    #ifndef RX_DBG
    		rprintf("PID scaled: ");
    		rprintfNum(10, 6,  TRUE, ' ',   output);		rprintfCRLF();
    #endif
#endif
    		#endif
    
    		#ifdef REG_PD
    
    		#endif
    //		actualTemp +=10;
    //		OCR0A = output;
    		CLEAR_PID_FLAG;
    
//    	}
/*
		if((temp = 0xFFFF-output) > 30000)
		{	temp = 30000;}
		else if (temp < 1000)
		{	temp = 1000;}
*/

    // Limit to within 1 half-phase:
    	if((output > PHASE_ANGLE_LIMIT_HIGH) || (output < 0)){
#ifndef CMDLINE
#ifndef RX_DBG
    		rprintf("Max out");rprintfCRLF();
#endif
#endif
    		output = PHASE_ANGLE_LIMIT_HIGH;	// don't fire the triac at all!
    		SET_HOLD_FIRE;						// set flag to not fire triac
    	}
    	else
    	{		
    		CLEAR_HOLD_FIRE;						// set flag to fire triac
    	}
    	
    	if ((output < PHASE_ANGLE_LIMIT_LOW)){  // || (output < 0))
#ifndef CMDLINE
#ifndef RX_DBG
    		rprintf("Min out");rprintfCRLF();
#endif
#endif
    		output = PHASE_ANGLE_LIMIT_LOW;		// fire the triac at zerocrozz to get complete halfwave
    
    	}
#ifndef WALK_PHASEANGLE
		OCR1A = output;
#endif
		//OCR1A = PHASE_ANGLE_LIMIT_LOW;
		//OCR1A = PHASE_ANGLE_LIMIT_HIGH;
	}



#ifdef DEBUG_SER
#ifndef CMDLINE
#ifndef RX_DBG

            if(sensorDisconnected)
        	{
        		rprintfProgStrM("Disconnected Probe!\r\n");
        	}
        	else
        	{
                rprintf("Process Value: ");
                rprintfFloat(5, tempData);  rprintfCRLF();
    
        		rprintf("Target Value: ");
                rprintfFloat(5, command);  rprintfCRLF();
    
        		rprintf("OCR1A: ");
        		rprintfNum(10, 6,  FALSE, ' ',   OCR1A);	rprintfCRLF();
    
        		rprintf("E ");
                rprintfFloat(5, _ERROR);  rprintfCRLF();
                    
        		rprintf("P ");
                rprintfFloat(5, _PTERM);  rprintfCRLF();
                    
        		rprintf("I ");
                rprintfFloat(5, _ITERM);  rprintfCRLF();
                    
        		rprintf("D ");
                rprintfFloat(5, _DTERM);  rprintfCRLF();
            }
    
        	vt100SetCursorPos(3, 0);
        	if(even++ == 10){
        		vt100ClearScreen();
                vt100SetCursorPos(3,0);
        		even = 0;
        	}
#endif //#ifndef RX_DBG
#endif //#ifndef CMDLINE
#endif //#ifdef DEBUG_SER
    }
    
    return(0);
}




static void avr_init(void)
// Initializes AVR controller
{
	MAX6675S_DDR |= BV(MAX6675_CSPIN);		// setup direction for CS pin
	DISABLE_6675;							// Set CS = high for MAX6675	
	initZerocross();						// init ext Int0
	initTriac();							// init Triac pin & direction


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

	//vt100ClearScreen();
	// start command line
//	goCmdline();
 
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
