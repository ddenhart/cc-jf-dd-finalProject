/* ==================================================================================
	ECE 485 / Fall 2014 / Cache simulator project
	Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
	File:			utility.c
	Date:			11/16/2014
	Author:			Carmen Ciobanu
	Modified:		Jeremiah Franke		12/04/2014
					Deborah Denhart		12/02/14
	Description:	This file contains: UTILITY FUNCTIONS
   ================================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "CacheSimulator.h"
#include "output.h"


/* ==================================================================================
	Function name:	ParseAddress
 	Arguments:			unsigned int * address
 	 	 	 	 		unsigned int * index
 	 	 	 	 		unsigned int * tag
	Returns:			void
	Description:		Address processing function (bit shifting utility function)
   ================================================================================== */
void ParseAddress(unsigned int * address, unsigned int * index, unsigned int * tag)
{
	unsigned int numOffsetBits = ConvertToBase(cacheStatistics.lineSize);
	unsigned int numIndexBits = ConvertToBase(cacheStatistics.numSets);
	unsigned int addressSize = ADDR_SIZE;
	unsigned int numTagBits = addressSize - (numOffsetBits + numIndexBits);
	//printf("\n%d, %d, %d, %d",addressSize, numOffsetBits, numIndexBits, numTagBits);
	*index = ((*address) << numTagBits) >> (numOffsetBits + numTagBits);
	*tag = (*address) >> (numOffsetBits + numIndexBits);
}

/* ==================================================================================
	Function name: GetLineAddress
 	Arguments:        unsigned int address for trace file address
	Returns:             unsigned int for beginning of the cache line address
	Description:        Gets the beginning of the cache line address, from the trace file address
   ================================================================================== */
unsigned int GetLineAddress(unsigned int address)
{
	unsigned int numOffsetBits, mask, i;

	mask = 0;
	numOffsetBits = ConvertToBase(cacheStatistics.lineSize);
	mask = ~mask;

	for(i = 0; i < numOffsetBits; ++i)
	{
		mask &= ~(1 << i);
	}

#ifdef DEBUG
	printf("Original address(byte address) = %#x\n", address);
#endif

	address &= mask;

#ifdef DEBUG
	printf("Address of the beginning of the line = %#x\n", address);
#endif

	return address;
}


/*==================================================================================
							 MATH FUNCTIONS
==================================================================================*/

/* ==================================================================================
	Function name:	ConvertToBase
 	Arguments:			int num
	Returns:				int
	Description:
   ================================================================================== */
int ConvertToBase(int num)
{
	int total = 0;
	int index = 0;
	while (num != 0)
	{
		if ((num % 2) == 1)
		{
			if (index == 0)
				total += index + 1;
			else
				total += index;
		}
		num = num / 2;
		++index;
	}
	if (total == index - 1)
		return (index - 1);
	else
		return index;
}


/* ==================================================================================
	Function name:	GetLineTag
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
unsigned int GetLineTag(unsigned int set, unsigned int line)
{
	return cachePtr[set].setPtr[line].tagBits;
}


/* ==================================================================================
	Function name:	SetLineTag
 	Arguments:
	Returns:
	Description:
   ================================================================================== */
void SetLineTag(unsigned int set, unsigned int line, unsigned int * tag)
{
	cachePtr[set].setPtr[line].tagBits = *tag;
}


/* ==================================================================================
	Function name:	CreateCache
 	Arguments:		None
	Returns:		void
	Description:	Initial creation of the cache
   ================================================================================== */
int CreateCache()
{
	unsigned int i;
	long long int bufferSize = 8;

    // Allocate memory for both sets and cache (initialize all bytes to 0 with calloc)
    if((cachePtr = (struct set_t*)calloc(cacheStatistics.numSets, sizeof(struct set_t))) == NULL)
	{
		fprintf(stderr, "In function %s, line %d: calloc failed to allocate memory for the cache\n", __FUNCTION__, __LINE__);
		return -1;
	}

	for (i = 0; i < cacheStatistics.numSets; ++i)
	{
		if ((cachePtr[i].setPtr = (struct line_t*)calloc(cacheStatistics.associativity, sizeof(struct line_t))) == NULL)
		{
			fprintf(stderr, "In function %s, line %d: calloc failed to allocate memory for set %d\n", __FUNCTION__, __LINE__, i);
			return -1;
		}
	}

	writePtr.wBufferArray = malloc(sizeof(long long int)* bufferSize);
	for (i = 0; i < WRITE_BUFFER_SIZE; ++i)
	{
		writePtr.wBufferArray[i] = -1;
	}
	
	// Create binary search array for pseudo LRU algorithm
	if ((binarySearchArray = (int*) malloc(sizeof(int) * cacheStatistics.associativity)) == NULL)
	{
		fprintf(stderr, "In function %s, line %d: malloc failed to allocate the binary search array\n", __FUNCTION__, __LINE__);
		return -1;
	}

	// Populate binary search array with all line numbers in the set
	for(i = 0; i <  cacheStatistics.associativity; ++i)
	{
		binarySearchArray[i] = i;
#ifdef DEBUG
		printf("Binary search array [%d] = %d\n", i, i);
#endif
	}

	return 0;
}


/* ==================================================================================
	Function name:	DestroyCache
 	Arguments:		None
	Returns:		void
	Description:	Release all memory from the cache
   ================================================================================== */
void DestroyCache()
{
	unsigned int i;

	// Deallocate memory for cache structure (all levels)
    for (i = 0; i < cacheStatistics.numSets; ++i)
	{
		if (cachePtr[i].setPtr)
		{
			free(cachePtr[i].setPtr);
		}
	}

    free(cachePtr);
    free(binarySearchArray);
}


/* ==================================================================================
	Function name:	InitParams
 	Arguments:
	Returns:		void
	Description:	Initializes all statistics and computes all other cache params
   ================================================================================== */
void InitParams()
{
    cacheStatistics.numLines = cacheStatistics.numSets * cacheStatistics.associativity;
	cacheStatistics.cacheSize = cacheStatistics.numLines * cacheStatistics.lineSize ;

	cacheStatistics.hitRatio = 0;
	cacheStatistics.numAccesses = 0;
	cacheStatistics.numHits = 0;
	cacheStatistics.numMisses = 0;
	cacheStatistics.numReads = 0;
	cacheStatistics.numWrites = 0;

}


/* ==================================================================================
	Function name:	ValidateInputs
 	Arguments:		None
	Returns:		void
	Description:	Initializes all statistics and computes all other cache params
   ================================================================================== */
int ValidateInputs()
{
	// We want the number of sets to stay in the range 1 to 65535
	if((cacheStatistics.numSets < 1) ||  (cacheStatistics.numSets > 65535))
	{
		fprintf(stderr, "In function %s, line %d: Number of sets out of bounds\n", __FUNCTION__, __LINE__);
		return -1;
	}

	// We want the line  size to be 32, 64, 128, 256, 512, 1024 or 2048 bytes
	if((cacheStatistics.lineSize != 32) && (cacheStatistics.lineSize != 64) &&
		(cacheStatistics.lineSize != 128) && (cacheStatistics.lineSize != 256) &&
		(cacheStatistics.lineSize != 512) && (cacheStatistics.lineSize != 1024) &&
		(cacheStatistics.lineSize != 2048))
	{
		fprintf(stderr, "In function %s, line %d: Line size out of bounds\n", __FUNCTION__, __LINE__);
		return -1;
	}

	// We want the associativity to be 4, 8, 16 or 32
	if((cacheStatistics.associativity != 4) && (cacheStatistics.associativity != 8) &&
		(cacheStatistics.associativity != 16) && (cacheStatistics.associativity != 32))
	{
		fprintf(stderr, "In function %s, line %d: Associativity value out of bounds\n", __FUNCTION__, __LINE__);
		return -1;
	}
	return 0;
}


/* ==================================================================================
Function name:	ReadMemory
Arguments:		unsigned int address
Returns:		void
Description:	Print the memory address read into the buffer.
================================================================================== */
void ReadMemory(unsigned int address)
{
	// TODO Call GetLineAddress() before sending the address; new address is return value

	if (!Silent)
	{
		printf("\n------------------------------------------Read Memory---------\n");
		printf("Memory read from Address: %#x", address);
		printf("\n--------------------------------------------------------------\n\n");
	}
}

/* ==================================================================================
Function name:	WriteMemory
Arguments:		unsigned int address
Returns:		void
Description:	Print the memory address written to memory.
================================================================================== */
void WriteMemory(unsigned int address)
{
	// TODO Call GetLineAddress() before sending the address; new address is return value
	if (!Silent)
	{
		printf("\n------------------------------------------Write Memory--------\n");
		printf("Memory write to Address: %#x", address);
		printf("\n--------------------------------------------------------------\n\n");
	}
}
