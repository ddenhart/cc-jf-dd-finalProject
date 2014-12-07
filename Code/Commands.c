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
 	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		0 if the function and underlying functions did not have a failure
					-1 if there was a failure
	Description:	read request from L2 data cache, read request from L2 instruction cache
					update the LRU and mesif states as well as the write buffer for 
					evictions and incoming cache lines.
   ================================================================================== */
int ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	int cacheLine = 0;
	int max = cacheStatistics.associativity - 1;
	int min = 0;
	int checkVictim = 0;
	int cmd = DATA_READ_REQ;
	int flag = CACHE_MISS;
	int cacheHit = FALSE;
	unsigned int setCount = 0;
	unsigned int baseEvict = 0;
	unsigned int baseAddress = 0;
	unsigned int eviction = 0;

	//unsigned int baseIndex = index << (ConvertToBase(cacheStatistics.lineSize));
	//unsigned int baseTag = tag << (ConvertToBase(baseIndex));
	baseAddress = GetLineAddress(HexAddress);	//Convert the hex address to its base hex address by removing the offset
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			flag = CACHE_HIT;				//Trip the hit flag
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;  //Update the hit statistics
			++cacheStatistics.numReads;
         	cacheLine = setCount;			//Get the cache line for the LRU update
         	UpdateMesif(cmd, baseAddress, index, cacheLine, CACHE_HIT);  //Update the MESIF State
			break;
		}
	}
	cacheLine = UpdateLRU(index, cacheLine, min, max, flag, &eviction);
	if (cacheLine < 0)
		return -1;
	else if (flag == CACHE_MISS)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;  //Update the miss statistics
		++cacheStatistics.numReads;
		checkVictim = writeBuffer(baseAddress, CHECK, 0);  //Check the write buffer for the base address of the sought address
		if (checkVictim > -1)
			baseAddress = checkVictim;			//Putting the address from the victim cache into the HexAddress
		else if (checkVictim == -3)
			return -1;
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED)  && (eviction == TRUE))
		{
			baseEvict = ((cachePtr[index].setPtr[cacheLine].tagBits << (ConvertToBase(index << (ConvertToBase(cacheStatistics.lineSize))))) + (index << (ConvertToBase(cacheStatistics.lineSize))));
			writeBuffer(baseEvict, INSERT, 0);
		}
     	cachePtr[index].setPtr[cacheLine].tagBits = tag;
      	UpdateMesif(cmd, baseAddress, index, cacheLine, CACHE_MISS);  //Update the MESIF State
	  	ReadMemory(baseAddress);
	}
	MessageToL2Cache(cmd, baseAddress, &eviction, baseEvict);
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
	int checkVictim = 0;
    int cmd = DATA_WRITE_REQ;
	int flag = CACHE_MISS;
	int cacheHit = FALSE;
	unsigned int setCount = 0;
	unsigned int baseEvict = 0;
	unsigned int baseAddress = 0;
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
         	UpdateMesif(cmd, baseAddress, index, cacheLine, CACHE_HIT);  //Update the MESIF State
			//printf("Writing %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
			break;
		}
	}
	cacheLine = UpdateLRU(index, cacheLine, min, max, flag, &eviction);
	//unsigned int baseIndex = index << (ConvertToBase(cacheStatistics.lineSize));
	//unsigned int baseTag = tag << (ConvertToBase(baseIndex));

	//printf("INDEX + TAG: %u\n", ((tag << (ConvertToBase(index << (ConvertToBase(cacheStatistics.lineSize)))))+(index << (ConvertToBase(cacheStatistics.lineSize)))));
	if (cacheLine < 0)
		return -1;
	else if (flag == CACHE_MISS)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;			//Increment statistics
		++cacheStatistics.numReads;
		checkVictim = writeBuffer(baseAddress, CHECK, 0);	//Check the write buffer for the base hexaddress
		if (checkVictim > -1)
			baseAddress = checkVictim;			//Putting the address from the write buffer into the HexAddress
		else if (checkVictim == -3)
			return -1;							//Return that there was an error in the buffer
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED) && (eviction == TRUE))
		{
			baseEvict = ((cachePtr[index].setPtr[cacheLine].tagBits << (ConvertToBase(index << (ConvertToBase(cacheStatistics.lineSize))))) + (index << (ConvertToBase(cacheStatistics.lineSize))));  //Terrible line to get the base address.
			writeBuffer(baseEvict, INSERT, 0); //Write the base address of the evicted line to buffer.  Offset bits do not matter, and we read a whole line anyways.
		}
      	cachePtr[index].setPtr[cacheLine].tagBits = tag;
		UpdateMesif(cmd, baseAddress, index, cacheLine, CACHE_MISS);  //Update the MESIF State
	  	WriteMemory(baseAddress);
	}
	MessageToL2Cache(cmd, baseAddress, &eviction, baseEvict); //Message to L2 cache for inclusivity
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand3
 	Arguments:
	Returns:
	Description: snooped invalidate command
   ================================================================================== */
int ExecuteCommand3(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;
	unsigned int baseAddress = 0;
	int setCount = 0;
	int checkVictim = 0;
	int foundFlag = FALSE;
	int message = FALSE;
	unsigned int eviction = 0;

	baseAddress = GetLineAddress(HexAddress);	//Get the base address of the HexAddress
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			foundFlag = TRUE;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;		//Increment statistics
			++cacheStatistics.numReads;
			if ((cachePtr[index].setPtr[setCount].mesifBits == eFORWARD) || (cachePtr[index].setPtr[setCount].mesifBits == eSHARED))
			{
				message = TRUE;
			}
			//printf("Writing %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
			break;
		}
	}
	if (!foundFlag)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;			//Increment statistics
		++cacheStatistics.numReads;
		checkVictim = writeBuffer(baseAddress, CHECK, 0);  //Check the write buffer for the base address of the sought address
		if (checkVictim > -1)
		{
			baseAddress = checkVictim;			//Putting the address from the write buffer into the HexAddress
			foundFlag = TRUE;
		}
		else if (checkVictim == -3)
			return -1;							//Return that there was an error in the buffer
	}

	//TODO DEB: Do you need to know that it failed finding it so that we can sent the snoop result?
    BusOperation(INVALIDATE, baseAddress, &SnoopResult);
    PutSnoopResult(baseAddress, SnoopResult);

	if (message)
	{
		MessageToL2Cache(SNOOPED_INVALIDATE, baseAddress, &eviction, baseAddress); //Message to L2 cache for inclusivity
	}
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand4
 	Arguments:
	Returns:
	Description: snooped read request
   ================================================================================== */
int ExecuteCommand4(unsigned int index, unsigned int tag, unsigned int HexAddress)
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
int ExecuteCommand5(unsigned int index, unsigned int tag, unsigned int HexAddress)
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
int ExecuteCommand6(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;
	unsigned int baseAddress = 0;
	int setCount = 0;
	int checkVictim = 0;
	int foundFlag = FALSE;
	int message = FALSE;
	unsigned int eviction = 0;

	baseAddress = GetLineAddress(HexAddress);	//Get the base address of the HexAddress
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			foundFlag = TRUE;
			++cacheStatistics.numHits;
			++cacheStatistics.numAccesses;		//Increment statistics
			++cacheStatistics.numReads;
			message = TRUE;
			//printf("Writing %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
			break;
		}
	}
	if (!foundFlag)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;			//Increment statistics
		++cacheStatistics.numReads;
		checkVictim = writeBuffer(baseAddress, CHECK, 0);  //Check the write buffer for the base address of the sought address
		if (checkVictim > -1)
		{
			baseAddress = checkVictim;			//Putting the address from the write buffer into the HexAddress
			eviction = TRUE;
			foundFlag = TRUE;
		}
		else if (checkVictim == -3)
			return -1;							//Return that there was an error in the buffer
	}
    BusOperation(RWIM, baseAddress, &SnoopResult);
    PutSnoopResult(baseAddress, SnoopResult);
	
	if (message)
	{
		MessageToL2Cache(SNOOPED_RWIM, baseAddress, &eviction, baseAddress); //Message to L2 cache for inclusivity
	}
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



