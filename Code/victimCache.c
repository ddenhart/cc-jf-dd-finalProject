/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team:
Carmen Ciobanu
Deborah Denhart
Jeremiah Franke

==================================================================================

File: 			victimCache.c
Date:			11/16/2014
Author:			Jeremiah Franke
Modified		Jeremiah Franke 12/04/2014
Description: 	This file contains:
-The functions for maintaining and adding/removing from the victim cache.
================================================================================== */


#include <stdio.h>
#include "parse.h"
#include "CacheSimulator.h"
#include "victimCache.h"

//All addresses are in the modified state, otherwise they would not be written back to memory.
int victimCache(unsigned int fullAddress, int action, int trigger)
{
	int counter = 0;
	int innerCounter = 0;
	int hit = FALSE;
	if (trigger > 2)
	{
		victimPtr.vCacheArray[0] = -1;
		for (innerCounter = 0; innerCounter < VICTIM_CACHE_SIZE; ++innerCounter)
		{
			if (innerCounter + 1 == VICTIM_CACHE_SIZE)
				victimPtr.vCacheArray[innerCounter] = -1;
			else
				victimPtr.vCacheArray[innerCounter] = victimPtr.vCacheArray[innerCounter + 1];
		}
		return 0;
		//Message saying it is written to memory??
	}
	if (action == INSERT)
	{
		for (counter = 0; counter < VICTIM_CACHE_SIZE; ++counter)
		{
			if (victimPtr.vCacheArray[counter] == -1)
			{
				victimPtr.vCacheArray[counter] = fullAddress;
				hit = TRUE;
			}
			if ((counter == VICTIM_CACHE_SIZE - 1) && (hit == FALSE))
			{
				//MessageToL2Cache()  Need to update this for the victim write
				//Or do we just write it to memory and not do a eviction/replacement in the buffer?
				victimPtr.vCacheArray[counter] = fullAddress;
			}
		}
		return 0;
	}
	else if (action == CHECK)
	{
		for (counter = 0; counter < VICTIM_CACHE_SIZE; ++counter)
		{
			if (victimPtr.vCacheArray[counter] == fullAddress)
			{
				for (innerCounter = counter; innerCounter < VICTIM_CACHE_SIZE; ++innerCounter)
				{
					if (innerCounter + 1 == VICTIM_CACHE_SIZE)
						victimPtr.vCacheArray[innerCounter] = -1;
					else
						victimPtr.vCacheArray[innerCounter] = victimPtr.vCacheArray[innerCounter+1];
				}
				//Message showing a return from the victim cache??
				return fullAddress;
			}
		}
		return -1;
	}
	return -3;  //Error, the wrong parameters were entered.
}