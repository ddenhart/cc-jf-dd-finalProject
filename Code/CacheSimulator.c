/* ==================================================================================
   ECE 485 / Fall 2014 / Cache simulator project
   Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
   File:              CacheSimulator.c
   Date:            11/16/2014
   Author:         Carmen Ciobanu
                        Jeremiah Franke
   Description: 	This file contains: main function
   ================================================================================== */

// # includes  
//dd TODO: (keep them in the .c to avoid conflicts later)
/* ----------
#include <stdint.h>				// for special data types like uint8_t, uint16_t 
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling
*/
// add more as needed
#include "CacheSimulator.h"
#include "parse.h"


#define DEBUG 1 //right now debug is true... we will replace this with a macro or 
                //something later so it can be defined at runtime
/*   MAIN FUNCTION  */

int main(int argc, char * argv[])
{
	char filename[100];
	long int arg1, arg2, arg3;
	int returnValue;
	//dd TODO: merge conflict char filename[INPUT_BUFFER_SIZE];
	//dd TODO: merge conflict int error = 0; 

	/* Read command line arguments
			Argv[0] = executable
	 		Argv[1] = number of sets
	 		Argv[2] = line size in bytes
	 		Argv[3] = cache associativity
	 		Argv[4] = trace file name
	*/
	//dd TODO: merge conflict handleInputs(argv, argc, filename);
	
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
	
	//dd TODO: merge conflict error = initCache();
	
	// Parse trace file and execute commands
	ParseFile(filename);
	
	// Calculate cache hit ratio
	cacheStatistics.hitRatio = cacheStatistics.numHits / cacheStatistics.numAccesses;

	// Print statistics for the current trace file
//dd TODO: merge conflict void printStatistics();
	OutputStatistics();
	
    // Deallocate all cache memory
//dd TODO: delCache();
	DestroyCache();

	return 0;
}

