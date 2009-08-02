#include <avr/io.h>
#include <inttypes.h>

// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"

extern u08 sensorDisconnected;
extern s16 _DTERM;
extern s16 _PTERM;
extern s16 _ERROR;
extern s16 _DUMMY;

#define DEBUG_SER

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

#define PHASE_ANGLE_LIMIT_HIGH	30500		//236
#define PHASE_ANGLE_LIMIT_LOW	700			//4


//#define TESTMODE

//#define DEBUG_SIM

#ifdef DEBUG_SIM
	#undef DEBUG_SER
#endif

#define REG_PD
