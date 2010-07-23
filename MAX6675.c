#include "global.h"
#include "MAX6675.h"
#include "spi.h"
#include "rprintf.h"

s08 readMAX6675(double *data)
{
	u16 MAX6675_Data;
	ENABLE_6675;												//CS->low

#ifdef DEBUG_SIM
		MAX6675_Data = 0x04;									// debugging, test value
		*data = 0x7FE0;
#else
		MAX6675_Data = spiTransferWord(0xFFFF);					// get data from MAX6675	
#endif
		DISABLE_6675;												// CS->high
		if(MAX6675_Data & MAX6675_MASK_SENSOR_CONNECTED){		// is the thermocouple-probe connected?
			return sensorDisconnected = 1;						// if not set flag
		}
		else{
			sensorDisconnected = 0;								// clear flag
		}

#ifdef MAX6675_INTEGER
		MAX6675_Data &= MAX6675_DATA_INTEGER;					// mask out the data, integer part only
		MAX6675_Data >>= 5;										// align data
		*data = MAX6675_Data;									// pass data		
#else
		MAX6675_Data &= MAX6675_DATA_FULL;						// mask out the data, including quarters
		MAX6675_Data >>= 3;										// align data
		u08 quarters = MAX6675_Data & 0x03;						// get quarters. 2 bits
		MAX6675_Data >>= 2;										// get rid of decimals
		*data = MAX6675_Data + (quarters * 0.25);

#endif
	return 0;

}
