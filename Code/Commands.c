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
#include "output.h"

/* ==================================================================================
	Function name:	ExecuteCommands02
 	Arguments:
	Returns:
	Description:
   ================================================================================== */

//TODO JF: Get the Update Mesif state function from Deb
//TODO JF: Write the victim cache

int ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	int cacheLine = 0;
	int max = cacheStatistics.associativity - 1;
	int min = 0;
	int flag = CACHE_MISS;
	int cacheHit = FALSE;
	unsigned int eviction = 0;

	for (int setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			flag = CACHE_HIT;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
			cacheLine = setCount;
			//updateMesifState(index, tag);  Update the MESIF State
			// MessageToL2();
			//printf("Reading %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
			break;
		}
	}
	cacheLine = UpdateLRU(index, cacheLine, min, max, flag, &eviction);
	if (cacheLine < 0)
		return -1;
	else if (flag == CACHE_MISS)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;
		++cacheStatistics.numReads;
		if (cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED)
		{
			//TODO JF: Put evicted line into Victim Cache;
		}
		cachePtr[index].setPtr[cacheLine].tagBits = tag;
		ReadMemory(HexAddress);
	}
	printf("\nTest\n");
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand1
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
int ExecuteCommand1(unsigned int index, unsigned int tag, unsigned int HexAddress)
{	
	int cacheLine = 0;
	int max = cacheStatistics.associativity - 1;
	int min = 0;
	int flag = CACHE_MISS;
	int cacheHit = FALSE;
	unsigned int eviction = 0;

	for (int setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			flag = CACHE_HIT;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
			cacheLine = setCount;
			//updateMesifState(index, tag);  Update the MESIF state.
			// MessageToL2();
			//printf("Writing %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
			break;
		}
	}
	cacheLine = UpdateLRU(index, cacheLine, min, max, flag, &eviction);
	if (cacheLine < 0)
		return -1;
	else if (flag == CACHE_MISS)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;
		++cacheStatistics.numReads;
		if (cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED)
		{
			//TODO JF: Put evicted line into Victim Cache;
		}
		cachePtr[index].setPtr[cacheLine].tagBits = tag;
		WriteMemory(HexAddress);
	}
	return 0;
	printf("\nCommand 1\n");
}


/* ==================================================================================
	Function name:	ExecuteCommand3
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand3(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand4
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand4(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand5
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand5(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand6
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void ExecuteCommand6(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand8
 	Arguments:
	Returns:
	Description:			Clear cache / Invalidate all lines
   ================================================================================== */
void ExecuteCommand8()
{
	unsigned int i, j;

	for(i = 0; i < cacheStatistics.numSets ; ++i)
	{
		//TODO Do you think LRU should retain state on clear, or not?
		//cachePtr[i].plruBits = 0;
		for(j = 0; j < cacheStatistics.associativity; ++i)
		{
			cachePtr[i].setPtr[j].mesifBits = 0;
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



