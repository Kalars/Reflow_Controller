#include "pid.h"

void initPID( 	SPid *pid, 
				double p_Gain, 
				double i_Gain, 
				double d_Gain,
				double i_Max,
				double i_Min )
{
	pid->p_Gain = p_Gain;
	pid->i_Gain = i_Gain;
	pid->d_Gain = d_Gain;
	pid->i_Max  = i_Max;
	pid->i_Min  = i_Min;

	return;
}


double UpdatePID(SPid *pid, double error, double newTemp)
{
	double p_Term, d_Term, i_Term;

	// calculate the proportional term
	p_Term = pid->p_Gain * error;
	
	// calculate the integral state
	pid->i_sumErrors += error;

	//apply appropriate limiting	
	if (pid->i_sumErrors > pid->i_Max) 
	{
		pid->i_sumErrors = pid->i_Max;
	}
	else if (pid->i_sumErrors < pid->i_Min)
	{
		 pid->i_sumErrors = pid->i_Min;
	}

	 // calculate the integral term
	i_Term = pid->i_Gain * pid->i_sumErrors;

	 // calculate the derivative term	
	d_Term = pid->d_Gain * (pid->d_lastTemp - newTemp);

	//update derivative state	
	pid->d_lastTemp = newTemp;

	//return output to controller	
	return p_Term + d_Term + i_Term;
}
