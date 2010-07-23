#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <avr/io.h>
#include <inttypes.h>

// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"

extern u08 sensorDisconnected;
extern double _DTERM;
extern double _PTERM;
extern double _ITERM;
extern double _ERROR;
extern double _DUMMY;

extern u32 skipcounter;
extern u32 skips;

#define DEBUG_SER

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

//#define PHASE_ANGLE_LIMIT_HIGH	30500		//236
#define PHASE_ANGLE_LIMIT_HIGH	30400			// end of ac-cyle, this mmeans no output (off)
#define PHASE_ANGLE_LIMIT_LOW	700				// start of ac-cyle, this mmeans full output (on)
#define RESOLUTION  ( PHASE_ANGLE_LIMIT_HIGH - PHASE_ANGLE_LIMIT_LOW )

#define MIN_ENERGY_OUTPUT	PHASE_ANGLE_LIMIT_HIGH
#define MAX_ENERGY_OUTPUT	PHASE_ANGLE_LIMIT_LOW

#define PHASE_RESOLUTION (PHASE_ANGLE_LIMIT_HIGH - PHASE_ANGLE_LIMIT_LOW)

#define PID_SYNC_TO_SAMPLE

#define WALK_PHASEANGLE					// Autoincrements phaseangle, for debug / finding limits
#undef WALK_PHASEANGLE					// Autoincrements phaseangle, for debug / finding limits
#define WALK_PHASEANGLE_STEP	500		// stepsize in timerticks for phaseangle autoincrement

//#define DEBUG_SIM						// Skip some code (UART/SPI mostly) that does not work in the simulator

//#define CMDLINE
#ifdef DEBUG_SIM
	#undef DEBUG_SER
#endif

#define RPRINTF_FLOAT

#define REG_PID                     // Use PID regulator
//#define REG_PD                    // Use PD regulator

#define P_FACTOR	1800.5
#define I_FACTOR    0.035           // 2.5
#define D_FACTOR	12000.35

char app_status;                // keep track of status using flags below
#define APP_STATUS_REG          app_status
#define DO_SAMPLE				0   // perform sampling
#define DO_PID					1   // run PID loop
#define HALF_PHASE              2   // use only upper or lower half of the phase
#define SKIP_PHASE				3   // skip-phase mode
#define HOLD_FIRE               4   // inhibit firing of triac

// MAcros to set & clear flags
#define SET_SAMPLE_FLAG			sbi(APP_STATUS_REG,DO_SAMPLE)
#define CLEAR_SAMPLE_FLAG		cbi(APP_STATUS_REG,DO_SAMPLE)

#define SET_PID_FLAG			sbi(APP_STATUS_REG,DO_PID)
#define CLEAR_PID_FLAG			cbi(APP_STATUS_REG,DO_PID)

#define SET_HALF_PHASE          sbi(APP_STATUS_REG,HALF_PHASE)
#define CLEAR_HALF_PHASE        cbi(APP_STATUS_REG,HALF_PHASE)

#define SET_SKIP_PHASE          sbi(APP_STATUS_REG,SKIP_PHASE)
#define CLEAR_SKIP_PHASE        cbi(APP_STATUS_REG,SKIP_PHASE)

#define SET_HOLD_FIRE          sbi(APP_STATUS_REG,HOLD_FIRE)
#define CLEAR_HOLD_FIRE        cbi(APP_STATUS_REG,HOLD_FIRE)

#endif	// GLOBAL_H_INCLUDED
