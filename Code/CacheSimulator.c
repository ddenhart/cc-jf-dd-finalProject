/* ==================================================================================
   ECE 485 / Fall 2014 / Cache simulator project
   Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
   File:			CacheSimulator.c
   Date:            11/16/2014
   Author:			Carmen Ciobanu
   Modified:		Jeremiah Franke		12/04/2014
   Description: 	This file contains: main function
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
#include "parse.h"
#include "output.h"


/*   MAIN FUNCTION  */

int main(int argc, char * argv[])
{
	char filename[INPUT_BUFFER_SIZE];
	long int arg1, arg2, arg3;
	int returnValue;

	/* Read command line arguments
			Argv[0] = executable
	 		Argv[1] = number of sets
	 		Argv[2] = line size in bytes
	 		Argv[3] = cache associativity
	 		Argv[4] = trace file name
	*/
	
	if(argc != 5)
	{
		fprintf(stderr, "Usage: %s	#Sets	Line size in bytes		Associativity		Trace file name\n", argv[0]);
		return 1;
	}
	
	// Store command line arguments
	arg1 = atoll(argv[1]);
	arg2 = atoll(argv[2]);
	arg3 = atoll(argv[3]);
	strcpy(filename, argv[4]);
    
    cacheStatistics.numSets = arg1;
    cacheStatistics.lineSize = arg2;
    cacheStatistics.associativity = arg3;

    // Validate inputs
    returnValue = ValidateInputs();
    if(returnValue < 0)
    {
       	return 1;
    }

    // Initialize  cache parameters
    InitParams();

    // Allocate memory for the cache
    returnValue =  CreateCache();
    if(returnValue < 0)
    {
    	return 1;
    }
	
	// Parse trace file and execute commands
	ParseFile(filename);
	
	// Calculate cache hit ratio
	cacheStatistics.hitRatio = cacheStatistics.numHits / cacheStatistics.numAccesses;

	// Print statistics for the current trace file
	OutputStatistics();
	
    // Deallocate all cache memory
	DestroyCache();

	return 0;
}

