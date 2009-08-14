/*
			WINAVR Extentions Library
			  (C) Dean Camera, 2005
						Freeware

		  Uses some code sourced from
		 AVRFREAKS.net and it's members
*/

// Port/Pin/DDR Bit Mapping
#ifdef PORTA
	#define PortA FlagVar(PORTA)
	#define PinA  FlagVar(PINA)
	#define DdrA  FlagVar(DDRA)
#endif

#ifdef PORTB
	#define PortB FlagVar(PORTB)
	#define PinB  FlagVar(PINB)
	#define DdrB  FlagVar(DDRB)
#endif

#ifdef PORTC
	#define PortC FlagVar(PORTC)
	#define PinC  FlagVar(PINC)
	#define DdrC  FlagVar(DDRC)
#endif

#ifdef PORTD
	#define PortD FlagVar(PORTD)
	#define PinD  FlagVar(PIND)
	#define DdrD  FlagVar(DDRD)
#endif

#ifdef PORTE
	#define PortE FlagVar(PORTE)
	#define PinE  FlagVar(PINE)
	#define DdrE  FlagVar(DDRE)
#endif

#ifdef PORTF
	#define PortF FlagVar(PORTF)
	#define PinF  FlagVar(PINF)
	#define DdrF  FlagVar(DDRF)
#endif

#ifdef PORTG
	#define PortG FlagVar(PORTG)
	#define PinG  FlagVar(PING)
	#define DdrG  FlagVar(DDRG)
#endif
