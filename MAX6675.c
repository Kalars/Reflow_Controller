#include "global.h"
#include "MAX6675.h"
#include "spi.h"
#include "rprintf.h"

void readMAX6675(u16 *data)
{
	u16 thermo;
	ENABLE_6675;			//CS->low
#ifdef DEBUG_SIM
	//debugging, test value
	thermo = 0x04;
	*data = 0x7FE0;
#else
		thermo = spiTransferWord(0xFFFF);				// get data from MAX6675	
//		*data = spiTransferWord(0xFFFF);				// get data from MAX6675	
#endif

		if (thermo & MAX6675_MASK_SENSOR_CONNECTED)	// is the thermocouple-probe connected?
//		if (*data & MAX6675_MASK_SENSOR_CONNECTED)	// is the thermocouple-probe connected?
		{
			sensorDisconnected = 1;					// if not set flag high
		}
		else
		{
			sensorDisconnected = 0;					// set flag low
		}

#ifdef MAX6675_INTEGER
		thermo &= MAX6675_DATA_INTEGER;	//mask out the data, integer part only
		thermo >>= 5;						//align data
//		*data &= MAX6675_DATA_INTEGER;	//mask out the data, integer part only
//		*data >>= 5;						//align data
#else
		thermo &= MAX6675_DATA_FULL;		//mask out the data, including quarters
		thermo >>= 3;						//align data
#endif
	DISABLE_6675;			//CS->high

	*data = thermo;

	return;

}
