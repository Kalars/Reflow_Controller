#include "global.h"
#include "pd.h"

void initPD( SPd *pd, u16 P_Gain, u16 D_Gain)
{
	pd->p_Gain = P_Gain;
	pd->d_Gain = D_Gain;

	return;
}

#define KONST 13000;

u16 UpdatePD(SPd *pd, s16 error, u08 newTemp)
{
	s32 p_Term, d_Term, dummy;

	dummy = 20000;

	_ERROR = error;

//	error *=2;

	// calculate the proportional term
	p_Term = pd->p_Gain * error;

	_PTERM = p_Term;

	 // calculate the derivative term	
	d_Term = pd->d_Gain * (pd->d_lastTemp - newTemp);
	
	_DTERM = d_Term/4;	

	//update derivative state	
	pd->d_lastTemp = newTemp;

	//return output to controller, adjust to 8 bits
//	dummy = p_Term - d_Term;
	dummy -= (p_Term + d_Term);
/*	
	if (dummy < 0)
	{	return 30000;	}
	else
	{
*/
//		dummy = -30000;
		_DUMMY = dummy;
		return dummy;// + KONST;
//	}
}
