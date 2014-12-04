/* ==================================================================================
	ECE 485 / Fall 2014 / Cache simulator project
	Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
	File:			Commands.c
	Date:			11/28/2014
	Author:			Carmen Ciobanu
	Description:	This file contains one function for each command 0-9
   ================================================================================== */
// # includes  
#include <stdint.h>				// for special data types like uint8_t, uint16_t 
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling
#include "CacheSimulator.h" 


/* ==================================================================================
	Function name:	ExecuteCommands02
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommands02(unsigned int index, unsigned int tag)
{
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand1
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand1(unsigned int index, unsigned int tag)
{
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand3
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand3(unsigned int index, unsigned int tag)
{
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand4
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand4(unsigned int index, unsigned int tag)
{
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand5
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand5(unsigned int index, unsigned int tag)
{
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand6
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand6(unsigned int index, unsigned int tag)
{
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand8
 	Arguments:
	Returns:
	Description:			Clear cache and invalidate all lines and reset all states
   ================================================================================== */
void ExecuteCommand8()
{
	unsigned int i, j;

	for(i = 0; i < cacheStatistics.numSets ; ++i)
	{
		cachePtr[i].plruBits = 0;
		for(j = 0; j < cacheStatistics.associativity; ++i)
		{
			cachePtr[i].setPtr[j].mesifBits = 0;
			cachePtr[i].setPtr[j].tagBits = 0;
		}
	}
}


/* ==================================================================================
	Function name:	ExecuteCommand9
 	Arguments:
	Returns:
	Description:			Print valid lines
   ================================================================================== */
void ExecuteCommand9()
{
	OutputValidLines();
}



