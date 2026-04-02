/*-----------------------------------------------------------------------------\
@ModuleName  ::	util.h
  
@Description ::	Header file for Utility routines 

@Copyright	 ::	Copyright 2001- Texas Instruments, Inc.

@History     ::
-------------------------------------------------------------------------------

	Dec.  26, 2001	Kedar C (kedarc@ti.com)	Start
		

\-----------------------------------------------------------------------------*/
#ifndef __UTIL_H__
#define __UTIL_H__ 

  
#include <stdio.h>

/* macros	*/
typedef	unsigned long	Uint32;	/* 32-bit	*/
typedef unsigned short	Uint16;	/* 16-bit	*/
typedef unsigned short	Uint16;	/* 16-bit	*/
typedef unsigned char	Uint8;	/* 8-bit	*/

typedef	unsigned long	Uint32;	/* 32-bit	*/
typedef unsigned short	Uint16;	/* 16-bit	*/
typedef unsigned char	Uint8;	/* 8-bit	*/

typedef int		STATUS;	
typedef Uint8	BOOL;

#define E_PASS		0
#define E_DEVICE	-1

#define DOT		fprintf(stderr, "\n.")

#define TRUE	1
#define FALSE	0

#define KB			1024
#define MB			KB*KB



/* support routines	*/

#endif	/*	__UTIL_H__	*/	
