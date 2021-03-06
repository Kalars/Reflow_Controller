#ifndef MAX6675_H
#define MAX6675_H




#define DISCONNECTED	1
#define CONNECTED		0

#define	MAX6675S_DDR	DDRB 
#define MAX6675_CSPIN	PB2
#define MAX6675S_PORT	PORTB

#define MAX6675_MASK_SENSOR_CONNECTED	0x04
#define MAX6675_DATA_INTEGER			0x7FE0
#define MAX6675_DATA_FULL				0x7FF8

#define ENABLE_6675		cbi(MAX6675S_PORT,MAX6675_CSPIN)
#define DISABLE_6675	sbi(MAX6675S_PORT,MAX6675_CSPIN)

#define MAX6675_INTEGER


//u16 readMAX6675(void);
//void readMAX6675(u16 *data);
s08 readMAX6675(u16 *data);




#endif
