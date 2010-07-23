/*! \file debug_vt100.h \brief VT100 special debug functions. */
//*****************************************************************************
//
// File Name	: 'debug_vt100.h'
// Title		: VT100 terminal function library
// Author		: Lars Andersson - Copyright (C) 2010
// Created		: 2010.07.23
// Revised		: 2010
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup general
/// \defgroup vt100 VT100 special debug functions (debug_vt100.c)
/// \code #include "debug_vt100.h" \endcode
/// \par Overview
///		This library provides functions for using VT100 emulation to
///	output debug data on the screen. Part of the screen can be used for static
/// labels and another for rolling message output.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef DEBUG_VT100_H_INCLUDED
#define DEBUG_VT100_H_INCLUDED
		
// this is coordinates on the screen in rows * columns format
#define CH_POS          3,9
#define CH2_POS         3,15
#define RFC_POS         4,5
#define FMSK_POS        4,21
#define NAMG_POS        4,38
#define ASTH_POS        4,53
#define QUIN_POS        7,6
#define QUIN_S_POS      7,8
#define SNCD_POS        7,21
#define HOPS_POS        7,37
#define CNT_TO_POS      7,55
#define SNIFF_EN_POS    9,17
#define MASK_EN_POS     9,36
#define OWN_LEVEL_POS   9,55
#define CNT_POS         12,5
#define CNT_RTL_TO_POS  12,33
#define CNT_ASTH_POS    12,60
#define AGCD_POS        14,6

#define DBG_GENERAL_DATA_POS    17
#define DBG_GENERAL_MAXLINES    6

void drawStatic(void);
void set_general_dbg_cursor(void);

#endif  // DEBUG_VT100_H_INCLUDED

