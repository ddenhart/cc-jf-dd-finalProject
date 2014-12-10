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
#include "parse.h"
#include "writeBuffer.h"

/* ==================================================================================
	Function name:	ExecuteCommands02
 	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		0 if the function and underlying functions did not have a failure
					-1 if there was a failure
	Description:	read request from L2 data cache.
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
	int bufferFlag = FALSE;
	unsigned int setCount = 0;
	unsigned int baseEvict = 0;
	unsigned int baseAddress = 0;
   unsigned int eviction = 0;
   unsigned int SnoopResult = 0;
   int BusOp = READ;

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
		return FAILURE;
	else if (flag == CACHE_MISS)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;  //Update the miss statistics
		++cacheStatistics.numReads;
		checkVictim = writeBuffer(baseAddress, CHECK, NOACTION);  //Check the write buffer for the base address of the sought address
		if (checkVictim > FAILURE)
			bufferFlag = TRUE;
		else if (checkVictim == -3)
			return -1;
		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED)  && (eviction == TRUE))
		{
			baseEvict = ((cachePtr[index].setPtr[cacheLine].tagBits << (ConvertToBase(index << (ConvertToBase(cacheStatistics.lineSize))))) + (index << (ConvertToBase(cacheStatistics.lineSize))));
			writeBuffer(baseEvict, INSERT, 0);
		}
      cachePtr[index].setPtr[cacheLine].tagBits = tag;
      BusOperation(BusOp, HexAddress, &SnoopResult);
      UpdateMesif(cmd, baseAddress, index, cacheLine, SnoopResult);  //Update the MESIF State
		if (!bufferFlag)
	  		ReadMemory(baseAddress);
	}
	MessageToL2Cache(cmd, baseAddress, &eviction, baseEvict);
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand1
	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		returns 0 if successful
					returns -1 if an error occured
	Description:	write request from L2 data cache.  Update the LRU and MESIF for
					incoming cache lines and cache hits.  Update the write buffer
					with evictions.
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
			++cacheStatistics.numWrites;
			cacheLine = setCount;
         	UpdateMesif(cmd, baseAddress, index, cacheLine, CACHE_HIT);  //Update the MESIF State
			//printf("Writing %d bytes starting at Address %d to L2 cache.\n", cacheStatistics.lineSize, HexAddress);
			break;
		}
	}
	cacheLine = UpdateLRU(index, cacheLine, min, max, flag, &eviction);
	if (cacheLine < 0)
		return FAILURE;
	else if (flag == CACHE_MISS)
	{
		++cacheStatistics.numMisses;
		++cacheStatistics.numAccesses;			//Increment statistics
		++cacheStatistics.numWrites;
		checkVictim = writeBuffer(baseAddress, CHECK, NOACTION);	//Check the write buffer for the base hexaddress
		if (checkVictim == CRITICAL_FAILURE)
			return FAILURE;						//Return that there was an error in the buffer
		else if (checkVictim > FAILURE)
			baseAddress = checkVictim;			//Putting the address from the write buffer into the HexAddress

		if ((cachePtr[index].setPtr[cacheLine].mesifBits == eMODIFIED) && (eviction == TRUE))
		{
			baseEvict = ((cachePtr[index].setPtr[cacheLine].tagBits << (ConvertToBase(index << (ConvertToBase(cacheStatistics.lineSize))))) + (index << (ConvertToBase(cacheStatistics.lineSize))));  //Terrible line to get the base address.
			writeBuffer(baseEvict, INSERT, NOACTION); //Write the base address of the evicted line to buffer.  Offset bits do not matter, and we read a whole line anyways.
		}
      	cachePtr[index].setPtr[cacheLine].tagBits = tag;
		UpdateMesif(cmd, baseAddress, index, cacheLine, CACHE_MISS);  //Update the MESIF State
	}
	MessageToL2Cache(cmd, baseAddress, &eviction, baseEvict); //Message to L2 cache for inclusivity
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand3
	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		returns 0 if successful
					returns -1 if an error occured
	Description:	Snooped invalidate command.  Walk the cache, if the MESIF bits
					are not invalid and the tag matches the passed in tag, if it is 
					in forward or shard, call the bus operation and send message to L2.
					If the MESIF bits are in modified or exclusive, throw an error.
					Continue with the trace file.
   ================================================================================== */
int ExecuteCommand3(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;
	unsigned int baseAddress = 0;
    int cmd = SNOOPED_INVALIDATE;
	int setCount = 0;
	int checkVictim = 0;
	int foundFlag = FALSE;
	int message = FALSE;
	unsigned int eviction = 0;
    int BusOp = INVALIDATE;

    BusOperation(BusOp, HexAddress, &SnoopResult);
	baseAddress = GetLineAddress(HexAddress);	//Get the base address of the HexAddress
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			foundFlag = TRUE;
			if ((cachePtr[index].setPtr[setCount].mesifBits == eFORWARD) || (cachePtr[index].setPtr[setCount].mesifBits == eSHARED))
			{
				message = TRUE;
            	UpdateMesif(cmd, baseAddress, index, setCount, SnoopResult);  //Update the MESIF State
			}
			if ((cachePtr[index].setPtr[setCount].mesifBits == eMODIFIED) || (cachePtr[index].setPtr[setCount].mesifBits == eEXCLUSIVE))
			{
				cachePtr[index].setPtr[setCount].mesifBits = eINVALID;
				printf("\nThis state change is not ever possible.  Running the next command from the parse file.\n");
				return 0;
			}
			break;
		}
	}
	if (message)
	{
		MessageToL2Cache(SNOOPED_INVALIDATE, baseAddress, &eviction, baseAddress); //Message to L2 cache for inclusivity
	}
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand4
 	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		returns 0 if successful
					returns -1 if an error occured
	Description: snooped read request.  Walk the cache to determine if in cache,
					if so, call bus operation to do snoop result.  Otherwise,
					walk the write buffer, if found, writeback to memory.
   ================================================================================== */
int ExecuteCommand4(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
	unsigned int SnoopResult = 0;
	unsigned int baseAddress = 0;
	int setCount = 0;
	int checkVictim = 0;
	int foundFlag = FALSE;
    int cmd = SNOOPED_READ;
    int BusOp = READ;

    BusOperation(BusOp, HexAddress, &SnoopResult);
	baseAddress = GetLineAddress(HexAddress);	//Get the base address of the HexAddress
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			foundFlag = TRUE;
      	    UpdateMesif(cmd, baseAddress, index, setCount, SnoopResult);  //Update the MESIF State
			break;
		}
	}
	if (!foundFlag)
	{
		checkVictim = writeBuffer(baseAddress, CHECK, WRITE_TO_MEMORY);  //Check the write buffer for the base address of the sought address, if found, write it to memory.
		if (checkVictim == CRITICAL_FAILURE)
			return FAILURE;						//Return that there was an error in the buffer
		else if (checkVictim > FAILURE)
		{
			baseAddress = checkVictim;			//Putting the address from the write buffer into the HexAddress
			foundFlag = TRUE;
		}					
	}
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand5
 	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		returns 0 if successful
					returns -1 if an error occured
	Description: snooped write request.  Nothing much happens on a write when snooping
   ================================================================================== */
int ExecuteCommand5(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;
    int cmd = SNOOPED_WRITE;
    int BusOp = WRITE;

    //use forwarding
    BusOperation(BusOp, HexAddress, &SnoopResult);
    UpdateMesif(cmd, HexAddress, 0, 0, SnoopResult);  //Update the MESIF State
	
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand6
	Arguments:		unsigned int index
					unsigned int tag
					unsigned int HexAddress
	Returns:		returns 0 if successful
					returns -1 if an error occured
	Description:	snooped read with intent to modify.  
					Walk the cache to determine if in cache,
					if so, call bus operation to do snoop result.  Otherwise,
					walk the write buffer, if found, set flag to true.  If the flag
					was set to true, write message to L2 and WriteMemory message.
   ================================================================================== */
int ExecuteCommand6(unsigned int index, unsigned int tag, unsigned int HexAddress)
{
    unsigned int SnoopResult = 0;
	unsigned int baseAddress = 0;
   int cmd = SNOOPED_RWIM;
	int setCount = 0;
	int checkVictim = 0;
	int foundFlag = FALSE;
	int message = FALSE;
	unsigned int eviction = 0;
    int BusOp = RWIM;

	baseAddress = GetLineAddress(HexAddress);	//Get the base address of the HexAddress
   BusOperation(BusOp, HexAddress, &SnoopResult);
	for (setCount = 0; setCount < cacheStatistics.associativity; ++setCount)
	{
		if ((cachePtr[index].setPtr[setCount].mesifBits != eINVALID) && (tag == cachePtr[index].setPtr[setCount].tagBits))
		{
			foundFlag = TRUE;
			message = TRUE;
         	UpdateMesif(cmd, baseAddress, index, setCount, SnoopResult);  //Update the MESIF State
			break;
		}
	}
	if (!foundFlag)
	{
		checkVictim = writeBuffer(baseAddress, CHECK, WRITE_TO_MEMORY);  //Check the write buffer for the base address of the sought address
		if (checkVictim == CRITICAL_FAILURE)
			return -1;							//Return that there was an error in the buffer
		else if (checkVictim > FAILURE)
		{
			baseAddress = checkVictim;			//Putting the address from the write buffer into the HexAddress
			eviction = TRUE;
			foundFlag = TRUE;
		}
	}

	if (message)
	{
		WriteMemory(baseAddress);
		MessageToL2Cache(SNOOPED_RWIM, baseAddress, &eviction, baseAddress); //Message to L2 cache for inclusivity
	}
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand8
 	Arguments:		Nothing
	Returns:		0
	Description:	Clear cache / Invalidate all lines
   ================================================================================== */
int ExecuteCommand8()
{
	unsigned int i, j;

	for(i = 0; i < cacheStatistics.numSets ; ++i)
	{
		for(j = 0; j < cacheStatistics.associativity; ++j)
		{
			cachePtr[i].setPtr[j].mesifBits = eINVALID;
		}
	}
	
	return 0;
}


/* ==================================================================================
	Function name:	ExecuteCommand9
 	Arguments:		Nothing
	Returns:		0
	Description:	Print valid lines
   ================================================================================== */
int ExecuteCommand9()
{
	OutputValidLines();
	
	return 0;
}



