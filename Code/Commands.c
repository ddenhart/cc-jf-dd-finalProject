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
	Modified:		Jeremiah Franke  12/04/2014
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
#include "mesif.h"

/* ==================================================================================
	Function name:	ExecuteCommands02
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	int validLines[INPUT_BUFFER_SIZE];
	int invalidLines[INPUT_BUFFER_SIZE];
	int invalidLength = 0;
	int validLength = 0;
	int cacheHit = FALSE;
	unsigned int eviction = 0;

	for (int setCount = 0; setCount < cacheStatistics.numLines; ++setCount)
	{
		if (cachePtr[index].setPtr[setCount].mesifBits != eINVALID)
		{
			if (tag == cachePtr[index].setPtr[setCount].tagBits)
			{
				++cacheStatistics.numHits;
				++cacheStatistics.numAccesses;
				++cacheStatistics.numReads;
				cacheHit = TRUE;
				updateMesifState(index, tag);
				UpdateLRU(index, tag, 0, cacheStatistics.numLines, CACHE_HIT, &eviction);  // TODO: JF: Is the 0 and cacheStatistics.numLines correct Carmen?
				printf("Sending %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
				break;
			}
			else
			{
				validLines[validLength] = setCount;  //TODO: JF: These might not be necessary.
				++validLength;
			}
		}
		else  //Invalid lines
		{
			invalidLines[invalidLength] = setCount;
			++invalidLength;
		}
		if (setCount == (cacheStatistics.numLines - 1) && !cacheHit)
		{
			++cacheStatistics.numMisses;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
			//Add command = 2 -> Why?
			ReadMemory(HexAddress);
			if (invalidLength > 0)  //At least one line is invalid
			{
				// Get snoop result
				// If hit, read from bus
				// If not read from memory
				// Set the Mesif state for the line here
				cachePtr[index].setPtr[invalidLines[0]].tagBits = tag;
			}
			else  //All lines filled, one needs to be evicted.
			{
				int evictedLine = UpdateLRU(index, tag, 0, cacheStatistics.numLines, EVICT_LINE, &eviction);
				cachePtr[index].setPtr[evictedLine].tagBits = tag;
				//Put evicted line into Victim Cache.
			}
		}
	}
	printf("\nTest\n");
	// TO DO
}


/* ==================================================================================
	Function name:	ExecuteCommand1
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand1(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	int validLines[INPUT_BUFFER_SIZE];
	int invalidLines[INPUT_BUFFER_SIZE];
	int invalidLength = 0;
	int validLength = 0;
	int cacheHit = FALSE;
	unsigned int eviction = 0;

	for (int setCount = 0; setCount < cacheStatistics.numLines; ++setCount)
	{
		if (cachePtr[index].setPtr[setCount].mesifBits != eINVALID)
		{
			if (tag == cachePtr[index].setPtr[setCount].tagBits)
			{
				++cacheStatistics.numHits;
				++cacheStatistics.numAccesses;
				++cacheStatistics.numReads;
				cacheHit = TRUE;
				updateMesifState(index, tag);
				UpdateLRU(index, tag, 0, cacheStatistics.numLines, CACHE_HIT, &eviction);  // TODO: JF: Is the 0 and cacheStatistics.numLines correct Carmen?
				printf("Writing %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
				break;
			}
			else
			{
				validLines[validLength] = setCount;  //TODO: JF: These might not be necessary.
				++validLength;
			}
		}
		else  //Invalid lines
		{
			invalidLines[invalidLength] = setCount;
			++invalidLength;
		}
		if (setCount == (cacheStatistics.numLines - 1) && !cacheHit)
		{
			++cacheStatistics.numMisses;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
			//Add command = 2 -> Why?
			ReadMemory(HexAddress);
			if (invalidLength > 0)  //At least one line is invalid
			{
				// Get snoop result
				// If hit, read from bus
				// If not read from memory
				// Set the Mesif state for the line here
				cachePtr[index].setPtr[invalidLines[0]].tagBits = tag;
			}
			else  //All lines filled, one needs to be evicted.
			{
				int evictedLine = UpdateLRU(index, tag, 0, cacheStatistics.numLines, EVICT_LINE, &eviction);
				if (cachePtr[index].setPtr[evictedLine].mesifBits == eMODIFIED)
				{
					//TODO: JF: Put evicted line into Victim Cache.
				}
				cachePtr[index].setPtr[evictedLine].tagBits = tag;
				WriteMemory(HexAddress);
				//TODO: JF: Send message for multiple lines to L2 to evict its addresses, do the math for starting/stopping addresses in the function
			}
		}
	}
	printf("\nCommand 1\n");
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
	//OutputValidLines();
}



