

typedef struct
{
	//state variables
	double d_lastTemp;	 		// Last temperature input
	double i_sumErrors; 		// Integrator state, sum of all errors
	double i_Max, i_Min;	 	// Maximum and minimum allowable integrator state

	//gain variables	
	double p_Gain;		// proportional gain
	double i_Gain; 		// integral gain
	double d_Gain;		// derivative gain

} SPid;

//calculates drive using PID regulation technique
double UpdatePID(SPid *pid, double error, double position);


//initiates PID variables
void initPID( 	SPid *pid, 
				double p_Gain, 
				double i_Gain, 
				double d_Gain,
				double i_Max,
				double i_Min );
