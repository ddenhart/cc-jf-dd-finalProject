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
Modified		Jeremiah Franke 12/08/2014
Description: 	This file contains:
-The functions for maintaining and adding/removing from the write buffer.
================================================================================== */


#include <stdio.h>
#include "parse.h"
#include "CacheSimulator.h"
#include "writeBuffer.h"

//All addresses are in the modified state, otherwise they would not be written back to memory.
/* ==================================================================================
Function name:	writeBuffer
Arguments:		long long int fullAddress
				unsigned int action
				int trigger
Returns:		0 for valid return, -1 for no address matching
Description:	Write buffer that takes evicted lines and holds them until trigger is called.
				Allows the cache to have a small buffer in case evicted lines are used again in
				the short term.
================================================================================== */
int writeBuffer(long long int fullAddress, int action, int trigger)
{
	int counter = 0;
	int innerCounter = 0;
	int hit = FALSE;

	if (trigger >= MIN_TRIGGER_STATE)								//Trigger the removal of an item from the write buffer
	{
		fullAddress = writePtr.wBufferArray[0];		//Get the address of the item written to memory
		if (fullAddress != EMPTY)
		{
#if DEBUGBUFFER
			printf("\n------------------------------------------Writing to memory Address in Write Buffer---------------\n");
			printf(" Address in Write Buffer written to memory: %#x", fullAddress);
			printf("\n--------------------------------------------------------------------------------------------------\n\n");
#endif
			WriteMemory(fullAddress);					//Output that it is being written to memory
		}
#if DEBUGBUFFER
		else
		{
			printf("\n------------------------------------------Address in Write Buffer is invalid----------------------\n");
			printf(" Address in Write Buffer is empty, cycling to next address");
			printf("\n--------------------------------------------------------------------------------------------------\n\n");
		}
#endif
		for (innerCounter = 0; innerCounter < WRITE_BUFFER_SIZE; ++innerCounter)
		{
			if (innerCounter + 1 == WRITE_BUFFER_SIZE)
				writePtr.wBufferArray[innerCounter] = EMPTY;			//Walk through the array and shift all the contents to the left by one and replace the last line with a -1 for checking.
			else
				writePtr.wBufferArray[innerCounter] = writePtr.wBufferArray[innerCounter + 1];
#if DEBUGBUFFER
			printf("WriteBuffer After Cycle reflex - Index: %d  Address: %d\n", innerCounter, writePtr.wBufferArray[innerCounter]);
#endif
		}
		return 0;
	}
	if (action == CLEAR)							//Clear the buffer
	{
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			if (writePtr.wBufferArray[counter] != EMPTY)
			{
				WriteMemory(writePtr.wBufferArray[counter]);						//Write out the memory address that was written
				writePtr.wBufferArray[counter] = EMPTY;
			}
		}
	}
	if (action == INSERT)							//Insert an item into the write buffer
	{
#if DEBUGBUFFER
		printf("\n------------------------------------------Inserting Address in Write Buffer-----------------\n");
		printf(" Address inserted into Write Buffer: %#x", fullAddress);
		printf("\n--------------------------------------------------------------------------------------------\n\n");
#endif
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			if (writePtr.wBufferArray[counter] == fullAddress)
			{
				writePtr.wBufferArray[counter] = fullAddress;		//To mimic converging write buffer, "converge" the data bits from the found address with those passed in.
				return 0;
			}
		}
		if (writePtr.wBufferArray[WRITE_BUFFER_SIZE - 1] == EMPTY)
		{
			writePtr.wBufferArray[WRITE_BUFFER_SIZE - 1] = fullAddress;		//The first "NULL" found in the write buffer, insert the address.
		}
		else
		{
			writeBuffer(NOACTION, NOACTION, MIN_TRIGGER_STATE);
#if DEBUGBUFFER
			printf("\n------------------------------------------Done Cycling, Inserting Address-----------------\n");
#endif
			writePtr.wBufferArray[WRITE_BUFFER_SIZE - 1] = fullAddress;						//If completely full, trigger an eviction early, and add the full address.
		}
#if DEBUGBUFFER
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			printf("WriteBuffer - Index: %d  Address: %d\n", counter, writePtr.wBufferArray[counter]);
		}
#endif
		return 0;
	}
	else if (action == CHECK)
	{
		for (counter = 0; counter < WRITE_BUFFER_SIZE; ++counter)
		{
			if (writePtr.wBufferArray[counter] == fullAddress)		//Check if the address passed in is the same as in the buffer
			{
#if DEBUGBUFFER
				printf("\n------------------------------------------Address Found in Write Buffer-----------------\n");
				printf(" Address found in Write Buffer: %#x", writePtr.wBufferArray[counter]);
				printf("\n----------------------------------------------------------------------------------------\n\n");
#endif
				if (trigger = WRITE_TO_MEMORY)									//Check to see if we need to immediately write out, per the MESIF states and commands
				{
					for (innerCounter = counter; innerCounter < WRITE_BUFFER_SIZE; ++innerCounter)
					{
						if (innerCounter + 1 == WRITE_BUFFER_SIZE)
							writePtr.wBufferArray[innerCounter] = EMPTY;		//Redo the array to move off the first in the buffer and shift to the left.
						else
							writePtr.wBufferArray[innerCounter] = writePtr.wBufferArray[innerCounter + 1];
#if DEBUGBUFFER
						printf("WriteBuffer - Index: %d  Address: %d\n", counter, writePtr.wBufferArray[counter]);
#endif
					}
					WriteMemory(fullAddress);						//Write out the memory address that was written
				}
				return fullAddress;
			}
		}
		return -1;  //No address matching the checked for address is in the buffer.
	}
	return -3;  //Error, the wrong parameters were entered.
}