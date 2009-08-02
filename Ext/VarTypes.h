/*
			WINAVR Extentions Library
			  (C) Dean Camera, 2005
						Freeware

		     Uses code sourced from
		 AVRFREAKS.net and it's members
*/

// Variable Type Defines
#define u08 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#define u64 unsigned long long
#define s08 signed char
#define s16 signed int
#define s32 signed long
#define s64 signed long long

// EEPROM Type Define
#define EEPROM __attribute__ ((section (".eeprom")))

// Variable Byte Size Defines
#define CharSize  sizeof(char)
#define LongSize  sizeof(long)
#define IntSize   sizeof(int)
#define FloatSize sizeof(float)
#define DoubleSize sizeof(double)
