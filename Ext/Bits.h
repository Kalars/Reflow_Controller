/*
			WINAVR Extentions Library
			  (C) Dean Camera, 2005
						Freeware

		     Uses code sourced from
		 AVRFREAKS.net and it's members
*/

// Byte component (Bit) and value Structure
typedef union
{
	unsigned char Value;
	
	struct
	{
		unsigned b0:1;
		unsigned b1:1;
		unsigned b2:1;
		unsigned b3:1;
		unsigned b4:1;
		unsigned b5:1;
		unsigned b6:1;
		unsigned b7:1;
	};
} Byte_Bitfield;

// Boolean Defines
#define FALSE 0
#define TRUE  1

// Bit Value Defines
#define B0 0x01
#define B1 0x02
#define B2 0x04
#define B3 0x08
#define B4 0x10
#define B5 0x20
#define B6 0x40
#define B7 0x80

// Bit-Related Macros
#define _BV32(bit) (1L << (bit))
#define BIT(bit)   _BV(bit)
#define BIT32(bit) _BV32(bit)

#define sbi(port,bit)    ((port) |= _BV(bit))
#define cbi(port,bit)    ((port) &= ~_BV(bit))

#define SetBit(port,bit) ((port) |= _BV(bit))
#define ClrBit(port,bit) ((port) &= ~_BV(bit))
#define ChkBit(port,bit) ((port) & _BV(bit))
#define FlpBit(port,bit) ((port) ^= _BV(bit))

#define loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
#define loop_until_bit_is_clear(sfr, bit) do { } while (bit_is_set(sfr, bit))
#define LoopUntilSet(var)                 while(!(var)) { }
#define LoopUntilClear(var)               while((var))  { }

// Bit/Flag Macros
#define FlagVar(var) (*((volatile Byte_Bitfield*) (&(var))))
