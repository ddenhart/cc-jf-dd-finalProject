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
		//writePtr.wBufferArray[0] = -1;
		for (innerCounter = 0; innerCounter < WRITE_BUFFER_SIZE; ++innerCounter)
		{
			if (innerCounter + 1 == WRITE_BUFFER_SIZE)
				writePtr.wBufferArray[innerCounter] = -1;
			else
				writePtr.wBufferArray[innerCounter] = writePtr.wBufferArray[innerCounter + 1];
		}
		return 0;
	}
	if (action == INSERT)
	{
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			if (writePtr.wBufferArray[counter] == -1)
			{
				writePtr.wBufferArray[counter] = fullAddress;
				return 0;
			}			
		}
		writePtr.wBufferArray[0] = fullAddress;
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
				return fullAddress;
			}
		}
		return -1;  //No address matching the checked for address is in the buffer.
	}
	return -3;  //Error, the wrong parameters were entered.
}