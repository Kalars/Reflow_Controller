/*
			WINAVR Extentions Library
			  (C) Dean Camera, 2005
						Freeware

		     Uses code sourced from
		 AVRFREAKS.net and it's members
*/

#include <avr/io.h>

/*
	List of Extentions Library Functions and Definitions:
	
		  >> FUNCTION MACROS <<
		=====================
			_BV32(x)										Returns the bit mask of a bit (compatible with longs)
			BIT(x)										Returns the bit mask of a bit (alias to _BV(x))
			BIT32(x)										Same as the _BV32 macro
			sbi(port, bit)								Sets a bit (number, NOT value) in a PORT (or any variable) register
			cbi(port, bit)								Clears a bit (number, NOT value) in a PORT (or any variable) register
			SetBit(port, bit)							Same as the sbi macro
			ClrBit(port, bit)							Same as the cbi macro
			ChkBit(port, bit)							Returns bit mask number if the bit number in the port is set, otherwise returns 0
			FlpBit(port, bit)							Inverts the value of the bit number in the port
			FlagVar(var)								Aliases (via another #define) a variable so that its individual bits can be set. 
															 Same bit structure as the PortA-PortG aliases.
			inb(register)								Loads data from a variable/register into another variable/register
			outb(value, register)					Loads data into a variable/register from another variable register
			inp(register)								Same as inpb macro
			outp(value, register)					Same as outb macro
			inw(register)								Same as inpb macro
			outw(value, register)					Same as outb macro
			LowIntB(intvariable)						Returns the low byte of an integer variable
			HighIntB(intvariable)					Returns the high byte of an integer variable
			LowLongI(longvariable)					Returns the low integer of a long variable
			HighLongI(longvariable)					Returns the high integer of a long variable			
			Higher(a,b) / Lower(a,b)				Returns the highest or lowest of two variables
			LoopUntilSet(var)						Pauses execution until while "var" is zero ( can be used for variables as well as bits)
			LoopUntilClear(var)						Pauses execution until while "var" is non-zero ( can be used for variables as well as bits)
			loop_until_bit_is_set(sfr, bit)			Pauses execution until the Bit is set (1) - Bit should bit bit number, not mask/value
			loop_until_bit_is_clear(sfr, bit)		Pauses execution until the Bit is clear (0) - Bit should bit bit number, not mask/value
			dbg_printf(...)							Use this instead of printf(...) when printing debug information. If the macro "DEBUG" is
															defined, printf is substituted. If not, the lines are ignored and no codespace wasted.

		  >> DEFINITIONS <<
		=====================
			B0-B7											Values (bit masks) of the bits of an unsigned character
			TRUE											Equivelent to 1
			FALSE											Equivelent to 0
			FOREVER										Equivelent to 1 (used for eternal while loops for readability)
			INPUTS										Equivelent to 0x00 (0)
			OUTPUTS										Equivelent to 0xFF (255)
			PortA-PortG 								Aliases to the standard PORTs (allows individual byte alteration)
															 Only the ports supported by the selected MCU have aliases defined
			PinA-PinG 									Aliases to the standard PINs (allows individual byte alteration)
															 Only the ports supported by the selected MCU have aliases defined
			DdrA-DdrG 									Aliases to the standard DDRs (allows individual byte alteration)
															 Only the ports supported by the selected MCU have aliases defined
			EEPROM										Variable attribute. When used the declared variable is defined in EEPROM
			u08-u64										Same as uintX_t; unsigned chars, longs, ints, floats and doubles
			s08-s64										Same as intX_t; signed chars, longs, ints, floats and doubles
			CharSize										Size of a char variable (in bytes)
			IntSize										Size of a integer variable (in bytes)
			LongSize										Size of a long variable (in bytes)
			FloatSize									Size of a float variable (in bytes)
			DoubleSize									Size of a double variable (in bytes)


		  >> VARIABLES <<
		=====================
			register0-register31						Direct access to the 32 AVR arithmetic registers. Use with care (usually
															 inside ASM sections) because trashing compiler data can lead to major glitches
*/

#ifndef _AVR_EXTENTIONS_
	#define _AVR_EXTENTIONS_

	#include "Bits.h"
	#include "Ports.h"
	#include "VarTypes.h"
	#include "Registers.h"	

	// Input/Output (Deprecated Macros)
	#define inb(register) (register)
	#define outb(value, register) ((register) = (value))

	#define inp(register) (register)
	#define outp(value, register) ((register) = (value))

	#define inw(register) (register)
	#define outw(value, register) ((register) = (value))

	// Partial Variable Macros
	#define LowIntB(intvariable)    ((intvariable) & 0x00FF)
	#define HighIntB(intvariable)   ((intvariable) & 0xFF00)
	#define LowLongI(longvariable)  ((longvariable) & 0x0000FFFF)
	#define HighLongI(longvariable) ((longvariable) & 0xFFFF0000)
	
	// Loop Macros and Definitions
	#define FOREVER 1
	#define LoopForever while(FOREVER)
	
	// Value Macros	
	#define Higher(a,b) ((a>b)? a : b)
	#define Lower(a,b)  ((a<b)? a : b)

	// Debug Print Macro
	#ifdef DEBUG
     #define dbg_printf(...) printf(__VA_ARGS__)
	#else
     #define dbg_printf(...) (int) 0
	#endif 

#endif
