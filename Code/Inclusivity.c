/* ==================================================================================
	ECE 485 / Fall 2014 / Cache simulator project
	Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
	File:			Inclusivity.c
	Date:			11/28/2014
	Author:			Carmen Ciobanu
	Description:	This file contains functions for inclusivity
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
	Function name:	MessageToL2Cache
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void MessageToL2Cache(int cmd, unsigned int address, int * eviction, unsigned int evictedLineAddr)
{
	if((cmd == DATA_READ_REQ) || (cmd == DATA_WRITE_REQ) || (cmd == INSTR_READ_REQ))
	{
#ifndef SILENT
		printf("L3 to L2: write %u bytes at address %#x\n", cacheStatistics.lineSize / 2, address);
		printf("L3 to L2: write %u bytes at address %#x\n", cacheStatistics.lineSize / 2, address + (cacheStatistics.lineSize / 2));
#endif
		if(*eviction)
		{
#ifndef SILENT
			printf("L3 to L2: invalidate line at address %#x\n", evictedLineAddr);
			printf("L3 to L2: invalidate line at address %#x\n", evictedLineAddr + (cacheStatistics.lineSize / 2));
#endif
		}
	}
	else if((cmd == SNOOPED_INVALIDATE) || (cmd == SNOOPED_RWIM))
	{
#ifndef SILENT
		printf("L3 to L2: invalidate line at address %#x\n", address);
		printf("L3 to L2: invalidate line at address %#x\n", address + (cacheStatistics.lineSize / 2));
#endif
	}
	else if(cmd == CLEAR_CACHE)
	{
#ifndef SILENT
		printf("L3 to L2: invalidate all lines\n");
#endif
	}
	else
	{
#ifndef SILENT
		printf("No message to send to L2\n");
#endif
	}
}








