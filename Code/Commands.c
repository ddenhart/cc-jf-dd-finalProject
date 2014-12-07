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
#include "victimCache.h"

/* ==================================================================================
	Function name:	ExecuteCommands02
 	Arguments:
	Returns:
	Description:
   ================================================================================== */

//TODO JF: Should we run the parser again on the returned address from the victim cache?  I don't see why you would, especially since it is already done, and at that point you are just looking for
// the data.
//TODO JF: Get the Update Mesif state function from Deb
int ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	int cacheLine = 0;
	int max = cacheStatistics.associativity - 1;
	int min = 0;
	int flag = CACHE_MISS;
	int cacheHit = FALSE;
	unsigned int setCount = 0;
	int checkVictim = 0;
	int baseAddress = 0;
	unsigned int eviction = 0;

	printf("Hex Address %x, %d\n", HexAddress, HexAddress);
	baseAddress = BaseAddress(HexAddress);
	printf("Base Address %x, %d\n", baseAddress, baseAddress);
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			flag = CACHE_HIT;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
			cacheLine = setCount;
			//TODO JF & DB: updateMesifState(index, setCount);  Update the MESIF State
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
		checkVictim = victimCache(HexAddress, CHECK, 0);
		printf("CHECK VICTIM: %d\n", checkVictim);
		if (checkVictim > -1)
			HexAddress = checkVictim;			//Putting the address from the victim cache into the HexAddress.
		else if (checkVictim == -3)
			return -1;
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED)  && (eviction == TRUE))
		{
			victimCache(cacheLine, INSERT, 0);
		}
		//TODO JF & DB: Update MESIF
		cachePtr[index].setPtr[cacheLine].tagBits = tag;
	}
	// MessageToL2() Invalidate the line;
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
	unsigned int setCount = 0;
	int checkVictim = 0;
	unsigned int eviction = 0;

	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			flag = CACHE_HIT;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
			cacheLine = setCount;
			//updateMesifState(index, tag);  Update the MESIF state.
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
		checkVictim = victimCache(HexAddress, CHECK, 0);
		if (checkVictim > -1)
			HexAddress = checkVictim;			//Putting the address from the victim cache into the HexAddress.
		else if (checkVictim == -3)
			return -1;							//Return that there was an error in the buffer.
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED) && (eviction == TRUE))
		{
			victimCache(cacheLine, INSERT, 0);
			for (int count = 0; count < VICTIM_CACHE_SIZE; ++count)
				printf("\nVictim Cache %u \n", victimPtr.vCacheArray[count]);
			//TODO JF: Put evicted line into Victim Cache;
		}
		//updateMesifState(index, tag);  Update the MESIF state.
		cachePtr[index].setPtr[cacheLine].tagBits = tag;
	}
	// MessageToL2();
	return 0;
	printf("\nCommand 1\n");
}


/* ==================================================================================
	Function name:	ExecuteCommand3
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
int ExecuteCommand3(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand4
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
int ExecuteCommand4(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand5
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
int ExecuteCommand5(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand6
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
int ExecuteCommand6(unsigned int index, unsigned int tag)
{
	// TODO
}


/* ==================================================================================
	Function name:	ExecuteCommand8
 	Arguments:
	Returns:
	Description:			Clear cache / Invalidate all lines
   ================================================================================== */
int ExecuteCommand8()
{
	unsigned int i, j;

	for(i = 0; i < cacheStatistics.numSets ; ++i)
	{
		for(j = 0; j < cacheStatistics.associativity; ++i)
		{
			cachePtr[i].setPtr[j].mesifBits = eINVALID;
		}
	}
}


/* ==================================================================================
	Function name:	ExecuteCommand9
 	Arguments:
	Returns:
	Description:			Print valid lines
   ================================================================================== */
int ExecuteCommand9()
{
	OutputValidLines();
}



