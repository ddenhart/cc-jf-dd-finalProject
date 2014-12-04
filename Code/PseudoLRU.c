/* ==================================================================================
	ECE 485 / Fall 2014 / Cache simulator project
	Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
	File:			PseudoLRU.c
	Date:			11/25-28/2014
	Author:			Carmen Ciobanu
	Description:	This file contains functions for pseudo LRU algorithm
   ================================================================================== */
#include <stdint.h>				// for special data types like uint8_t, uint16_t 
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling
#include "CacheSimulator.h"


/* ==================================================================================
  	Function name:	UpdateLRU
  	Arguments:		unsigned int set: current set
 					unsigned int line: current line in current set
 					int min: min line index in set
 					int max: max line index in set
 					int flag: signals cache hit or miss
  	Returns:		int: success or failure for current function
  					return value < 0 = failure
  					return value > 0 = success
  	Description:	After a cache hit, writes to LRU bits to update line hierarchy
  	                After a cache miss, finds line to evict and updates line hierarchy
   ================================================================================== */
int UpdateLRU(unsigned int set, unsigned int line, int min, int max, int flag, int * eviction)
{
	int i, bitToRead, bitValue, evictedLine, errorCode;
	bitToRead = -1;
	bitValue = -1;
	errorCode = 0;
	evictedLine = -1;
	
	
	// Catch line numbers greater than allowed in set
	if(line > (cacheStatistics.associativity - 1))
	{
		fprintf(stderr, "In function %s, line %d: Line number out of bounds\n", __FUNCTION__, __LINE__);
		return -1; 
	}

	// If the request was made due to a cache hit or a miss due to an invalid line, write LRU bits (no need to read)
	if((flag == CACHE_HIT) || (flag == CACHE_MISS ))
	{
		if((errorCode = SetPseudoLRU(set, line, min, max)) == -1)
		{
			return -1;
		}
		else
		{
			*eviction = FALSE;
			evictedLine = 0;
		}
	}
	// If the request was made due to a cache miss, find an invalid line to bring in new line or find line to evict if and write LRU bits
	else if(flag == EVICT_LINE)
	{	
		int i;
		unsigned int mesifState;


		// First of all, search for remaining invalid lines in the set
		for(i = 0; i < cacheStatistics.associativity; ++i)
		{
			mesifState = cachePtr[set].setPtr[i].mesifBits;
			if(mesifState == 0)
			{
				evictedLine = i;
				if((errorCode = SetPseudoLRU(set, evictedLine, min, max)) == -1)
					{
						return -1;
					}
				*eviction = FALSE;	// No, we did not evict a line, since we had an invalid line available
				return evictedLine;
			}
		}

		// If no invalid lines, we need to select a victim line to evict
		if((errorCode = GetVictimLine(set, min, max, &bitToRead, &bitValue)) == -1)
		{
			return -1;
		}

		if(bitValue == 0)
		{
			evictedLine = bitToRead;
			if((errorCode = SetPseudoLRU(set, evictedLine, min, max)) == -1)
			{
				return -1;
			}
		}
		else if(bitValue == 1)
		{
			evictedLine = bitToRead + 1;
			if((errorCode = SetPseudoLRU(set, evictedLine, min, max)) == -1)
			{
				return -1;
			}
		}
		*eviction = TRUE;		// Yes, we ended up evicting a line
	}
	// If flag is bogus, report error and terminate program
	else
	{
		fprintf(stderr, "In function %s, line %d: unknown flag value\n", __FUNCTION__, __LINE__);
		return -1; 
	}

	return evictedLine; 
}


/* ==================================================================================
	Function name:	SetPseudoLRU
 	Arguments:		unsigned int set: current set
 					unsigned int line: current line in current set
 					int min: min line index in set
 					int max: max line index in set
	Returns:		int: error code for any binary search algorithm errors:
					-1 = min < max --> invalid inputs
					1 = max = min --> stop algorithm
	Description:	Recursive binary search algorithm function to set/clear LRU bits
   ================================================================================== */
int SetPseudoLRU(unsigned int set, unsigned int line, int min, int max)
{
	// Max cannot be less than min, so report invalid inputs
	if(max < min)
	{
		fprintf(stderr, "In function %s, line %d: invalid inputs\n", __FUNCTION__, __LINE__);
		return -1;
	}
	// If max becomes equal to min, stop the recursive algorithm
	else if(max == min)
	{
		return 1;
	}	
	else
	{
#ifdef DEBUG
		printf("\nGetting the midpoint between min = %d and max = %d\n", min, max);
#endif

		// Get bit number to write in the imaginary LRU bits tree
		int bitToSet = GetMidpoint(min, max);
		
		// If MRU line index is less than or equal to current bit to write, set bit (1)
		if(line <= binarySearchArray[bitToSet])
		{
			cachePtr[set].plruBits |= (1 << bitToSet);
#ifdef DEBUG
			printf("Pseudo LRU bits = 0x%x\n\n", cachePtr[set].plruBits);
#endif
			return SetPseudoLRU(set, line, min, bitToSet);
		}
		// If MRU line index is greater than current bit to write, clear bit (0)
		else
		{
			cachePtr[set].plruBits &= ~(1 << bitToSet);
#ifdef DEBUG			
			printf("Pseudo LRU bits = 0x%x\n\n", cachePtr[set].plruBits);
#endif
			return SetPseudoLRU(set, line, bitToSet+1, max);
		}
	}
}


/* ==================================================================================
   	Function name:	GetVictimLine
 	Arguments:		unsigned int set: current set
 					unsigned int line: current line in current set
 					int min: min line index in set
 					int max: max line index in set
 					int * bitToRead: pointer to the current LRU bit to read
 					int * bitValue: pointer to the value of the current LRU bit to read
	Returns:		int: error code for any binary search algorithm errors:
					-1: min < max --> invalid inputs
					1: max = min --> stop algorithm		
   	Description:	Selects line to be evicted
   ================================================================================== */
int GetVictimLine(unsigned int set, int min, int max, int * bitToRead, int * bitValue) 
{
	// Max cannot be less than min, so report invalid inputs
	if(max < min)
	{
		fprintf(stderr, "In function %s, line %d: invalid inputs\n", __FUNCTION__, __LINE__);
		return -1;
	}
	// If max becomes equal to min, stop the recursive algorithm
	else if (min == max)
	{
		return 1;
	}
	else
	{
		// Get bit number to read from imaginary LRU bits tree
		* bitToRead = GetMidpoint(min, max);
#ifdef DEBUG
		printf("We are reading bit %d\n", *bitToRead);
#endif

		// Get value of current bit being read from the imaginary LRU bit tree
		* bitValue = ((cachePtr[set].plruBits) & (1 << (*bitToRead))) >> (*bitToRead);
#ifdef DEBUG
		printf("The value of bit %d is %d\n", *bitToRead, *bitValue);
#endif		

		// If current bit is 0, take left branch of imaginary LRU bits tree
		if(*bitValue == 0)
		{
			return GetVictimLine(set, min, *bitToRead, bitToRead, bitValue);
		}
		// If current bit is 1, take right branch of imaginary LRU bits tree
		else
		{
			return GetVictimLine(set, *bitToRead+1, max, bitToRead, bitValue);
		}
	}
}


/* ==================================================================================
  	Function name:	GetMidpoint
   	Arguments:		int min: min line index in set
   					int max: max line index in set
  	Returns:		int: midpoint between min and max
  	Description:	Calculates and returns midpoint between two min and max values
     ================================================================================== */
int GetMidpoint(int min, int max) // Find line to be evicted
{
#ifdef DEBUG
	printf("New midpoint is %d\n", min + ((max - min) / 2));
#endif

	return (min + ((max - min) / 2));
}



