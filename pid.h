

typedef struct
{
	double dState; 		// Last position input
	
	double iState; 		// Integrator state
	
	double iMax, iMin; 	// Maximum and minimum allowable integrator state
	
	double iGain; 		// integral gain
	
	double pGain;		// proportional gain
	
	double dGain;		// derivative gain

} SPid;
