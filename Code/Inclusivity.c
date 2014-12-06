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
	// Read (cmd 0, 2): L3 gets the data, and sends it to L2 (keeps a copy)
	// Write (cmd 1): L3 gets the data, writes it, and sends it to L2 (keeps a copy)
	if((cmd == DATA_READ_REQ) || (cmd == DATA_WRITE_REQ) || (cmd == INSTR_READ_REQ))
	{
		// L3 needs to send 2*L2 line size bytes to L2, along with the lines addresses
#ifndef SILENT
		printf("L3 to L2: write %u bytes at address %#x\n", cacheStatistics.lineSize / 2, address);
		printf("L3 to L2: write %u bytes at address %#x\n", cacheStatistics.lineSize / 2, address + (cacheStatistics.lineSize / 2));
#endif
		// If L3 evicted a line in the process of satisfying the request from L3
		// L2 needs to invalidate the line, if present in L2 cache
		// L3 to L2 line size ratio = 2:1; L2 needs to invalidate 2 lines for every l3 line
		if(*eviction)
		{
#ifndef SILENT
			printf("L3 to L2: invalidate line at address %#x\n", evictedLineAddr);
			printf("L3 to L2: invalidate line at address %#x\n\n", evictedLineAddr + (cacheStatistics.lineSize / 2));
#endif
		}
	}
	// Snooped Invalidate (cmd 3) and snooped RWIM (cmd 6):
	// L2 needs to invalidate the lines, if present in L2 cache
	else if((cmd == SNOOPED_INVALIDATE) || (cmd == SNOOPED_RWIM))
	{
#ifndef SILENT
		printf("L3 to L2: invalidate line at address %#x\n", address);
		printf("L3 to L2: invalidate line at address %#x\n\n", address + (cacheStatistics.lineSize / 2));
#endif
	}
	// Clear cache(cmd 8):
	// L2 needs to invalidate all lines
	else if(cmd == CLEAR_CACHE)
	{
#ifndef SILENT
		printf("L3 to L2: invalidate all lines\n\n");
#endif
	}
	else
	{
#ifndef SILENT
		printf("No message to send to L2\n");
#endif
	}
}

