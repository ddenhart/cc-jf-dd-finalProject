/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team:
Carmen Ciobanu
Deborah Denhart
Jeremiah Franke

==================================================================================

File: 			writeBuffer.c
Date:			11/16/2014
Author:			Jeremiah Franke
Modified		Jeremiah Franke 12/04/2014
Description: 	This file contains:
-The functions for maintaining and adding/removing from the write buffer.
================================================================================== */


#include <stdio.h>
#include "parse.h"
#include "CacheSimulator.h"
#include "writeBuffer.h"

//All addresses are in the modified state, otherwise they would not be written back to memory.
int writeBuffer(long long int fullAddress, int action, int trigger)
{
	int counter = 0;
	int innerCounter = 0;
	int hit = FALSE;
	if (trigger > 2)
	{
		writePtr.wBufferArray[0] = -1;
		for (innerCounter = 0; innerCounter < WRITE_BUFFER_SIZE; ++innerCounter)
		{
			if (innerCounter + 1 == WRITE_BUFFER_SIZE)
				writePtr.wBufferArray[innerCounter] = -1;
			else
				writePtr.wBufferArray[innerCounter] = writePtr.wBufferArray[innerCounter + 1];
		}
		return 0;
		//Message saying it is written to memory??
	}
	if (action == INSERT)
	{
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			if (writePtr.wBufferArray[counter] == -1)
			{
				writePtr.wBufferArray[counter] = fullAddress;
				hit = TRUE;
			}
			if ((counter == WRITE_BUFFER_SIZE - 1) && (hit == FALSE))
			{
				//MessageToL2Cache()  Need to update this for the victim write
				//Or do we just write it to memory and not do a eviction/replacement in the buffer?
				writePtr.wBufferArray[counter] = fullAddress;
			}
		}
		return 0;
	}
	else if (action == CHECK)
	{
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			if (writePtr.wBufferArray[counter] == fullAddress)
			{
				for (innerCounter = counter; innerCounter < WRITE_BUFFER_SIZE; ++innerCounter)
				{
					if (innerCounter + 1 == WRITE_BUFFER_SIZE)
						writePtr.wBufferArray[innerCounter] = -1;
					else
						writePtr.wBufferArray[innerCounter] = writePtr.wBufferArray[innerCounter + 1];
				}
				//Message showing a return from the write Buffer??
				return fullAddress;
			}
		}
		return -1;
	}
	return -3;  //Error, the wrong parameters were entered.
}