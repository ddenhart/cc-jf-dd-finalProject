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
#include "output.h"
#include "writeBuffer.h"

/* ==================================================================================
	Function name:	ExecuteCommands02
 	Arguments:
	Returns:
	Description: read request from L2 data cache, read request from L2 instruction cache
   ================================================================================== */

//TODO JF: Should we run the parser again on the returned address from the write buffer?  I don't see why you would, especially since it is already done, and at that point you are just looking for
// the data.
//TODO JF: Get the Update Mesif state function from Deb
int ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	int cacheLine = 0;
	int max = cacheStatistics.associativity - 1;
	int min = 0;
	int cmd = DATA_READ_REQ;
	int flag = CACHE_MISS;
	int cacheHit = FALSE;
	unsigned int setCount = 0;
	int checkVictim = 0;
	int baseAddress = 0;
	unsigned int eviction = 0;

	baseAddress = GetLineAddress(HexAddress);
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			flag = CACHE_HIT;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;
			++cacheStatistics.numReads;
         	cacheLine = setCount;
         	UpdateMesif(cmd, HexAddress, index, cacheLine, CACHE_HIT);  //Update the MESIF State
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
		checkVictim = writeBuffer(HexAddress, CHECK, 0);
		if (checkVictim > -1)
			HexAddress = checkVictim;			//Putting the address from the victim cache into the HexAddress.
		else if (checkVictim == -3)
			return -1;
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED)  && (eviction == TRUE))
		{
			writeBuffer(cacheLine, INSERT, 0);
		}
     	cachePtr[index].setPtr[cacheLine].tagBits = tag;
      	UpdateMesif(cmd, HexAddress, index, cacheLine, CACHE_MISS);  //Update the MESIF State
	  	ReadMemory(HexAddress);
	}
	// MessageToL2() Invalidate the line;
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand1
 	Arguments:
	Returns:
	Description: write request from L2 data cache
   ================================================================================== */
int ExecuteCommand1(unsigned int index, unsigned int tag, unsigned int HexAddress)
{	
	int cacheLine = 0;
	int max = cacheStatistics.associativity - 1;
	int min = 0;
    int cmd = DATA_WRITE_REQ;
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
         	UpdateMesif(cmd, HexAddress, index, cacheLine, CACHE_HIT);  //Update the MESIF State
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
		checkVictim = writeBuffer(HexAddress, CHECK, 0);
		if (checkVictim > -1)
			HexAddress = checkVictim;			//Putting the address from the victim cache into the HexAddress.
		else if (checkVictim == -3)
			return -1;							//Return that there was an error in the buffer.
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED) && (eviction == TRUE))
		{
			writeBuffer(cacheLine, INSERT, 0);
			//TODO JF: Put evicted line into Victim Cache;
		}
      	cachePtr[index].setPtr[cacheLine].tagBits = tag;
      	UpdateMesif(cmd, HexAddress, index, cacheLine, CACHE_MISS);  //Update the MESIF State
	  	WriteMemory(HexAddress);
	}
	// MessageToL2();
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand3
 	Arguments:
	Returns:
	Description: snooped invalidate command
   ================================================================================== */
int ExecuteCommand3(unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;

    BusOperation(INVALIDATE, HexAddress, &SnoopResult);
    PutSnoopResult(HexAddress, SnoopResult);
	
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand4
 	Arguments:
	Returns:
	Description: snooped read request
   ================================================================================== */
int ExecuteCommand4(unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;

    BusOperation(READ, HexAddress, &SnoopResult);
    PutSnoopResult(HexAddress, SnoopResult);
	
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand5
 	Arguments:
	Returns:
	Description: snooped write request
   ================================================================================== */
int ExecuteCommand5(unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;

    BusOperation(WRITE, HexAddress, &SnoopResult);
    PutSnoopResult(HexAddress, SnoopResult);
	
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand6
 	Arguments:
	Returns:
	Description: snooped read with intent to modify
   ================================================================================== */
int ExecuteCommand6(unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;

    BusOperation(RWIM, HexAddress, &SnoopResult);
    PutSnoopResult(HexAddress, SnoopResult);
	
	return 0;
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
	
	return 0;
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
	
	return 0;
}



