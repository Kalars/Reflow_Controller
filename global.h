#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <avr/io.h>
#include <inttypes.h>

// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"

extern u08 sensorDisconnected;
extern s16 _DTERM;
extern s16 _PTERM;
extern s16 _ITERM;
extern s16 _ERROR;
extern s16 _DUMMY;

extern u32 skipcounter;
extern u32 skips;

#define DEBUG_SER

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

//#define PHASE_ANGLE_LIMIT_HIGH	30500		//236
#define PHASE_ANGLE_LIMIT_HIGH	30400
#define PHASE_ANGLE_LIMIT_LOW	700			//4


#define WALK_PHASEANGLE					// Autoincrements phaseangle, for debug / finding limits
#undef WALK_PHASEANGLE					// Autoincrements phaseangle, for debug / finding limits
#define WALK_PHASEANGLE_STEP	500			// stepsize in timerticks for phaseangle autoincrement

//#define DEBUG_SIM							// Skip some code (UART/SPI mostly) that does not work in the simulator

#ifdef DEBUG_SIM
	#undef DEBUG_SER
#endif

#define REG_PID

#define P_FACTOR	(500 * 2)
#define I_FACTOR	2
#define D_FACTOR	(4000 * 2)
char app_status;
#define DO_SAMPLE				0   // flag, should we sample this time around in the main loop?
#define DO_PID					1
#define HALF_PHASE              2
#define SKIP_PHASE				3

#define APP_STATUS_REG          app_status		

#define SET_SAMPLE_FLAG			sbi(APP_STATUS_REG,DO_SAMPLE)
#define CLEAR_SAMPLE_FLAG		cbi(APP_STATUS_REG,DO_SAMPLE)
#define SET_PID_FLAG			sbi(APP_STATUS_REG,DO_PID)
#define CLEAR_PID_FLAG			cbi(APP_STATUS_REG,DO_PID)

#define SET_HALF_PHASE          sbi(APP_STATUS_REG,HALF_PHASE)
#define CLEAR_HALF_PHASE        cbi(APP_STATUS_REG,HALF_PHASE)

#define SET_SKIP_PHASE          sbi(APP_STATUS_REG,SKIP_PHASE)
#define CLEAR_SKIP_PHASE        cbi(APP_STATUS_REG,SKIP_PHASE)

#endif	// GLOBAL_H_INCLUDED
