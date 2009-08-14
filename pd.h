typedef struct
{
	//state variables
	u08 d_lastTemp;		// Last temperature input

	//gain variables	
	s16 p_Gain;		// proportional gain
	s16 d_Gain;		// derivative gain

} SPd;

//calculates drive using PID regulation technique
u16 UpdatePD(SPd *pd, s16 error, u08 position);


//initiates PID variables
void initPD(SPd *pd, u16 p_Gain, u16 d_Gain);
