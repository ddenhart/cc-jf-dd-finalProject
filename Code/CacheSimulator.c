/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			CacheSimulator.c
Date:			11/16/2014
Author:			Carmen Ciobanu
Description: 	This file contains:
				- global definitions (data structures)
				- main function
				- specific protocol functions (LRU, MESI, inclusivity)
				- utility functions (bit shifting functions, set and get 
									type functions,output, other)
	
================================================================================== */

// # includes  (keep them in the .c to avoid conflicts later)
// ----------
#include <stdint.h>				// for special data types like uint8_t, uint16_t 
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling
// add more as needed
#include "CacheSimulator.h"

#define DEBUG 1 //right now debug is true... we will replace this with a macro or 
                //something later so it can be defined at runtime

==================================================================================
							     MAIN FUNCTION
==================================================================================

int main(int argc, char * argv[])
{
	// Local variables
	// ---------------
	FILE * traceFile;				// file descriptor for trace file
	char filename[100];  			// array for storing the trace file name
	long long arg1, arg2, arg3;
	int i;
	// other local variables		
      	
      	
	/* Read command line arguments and do error check
	   ----------------------------------------------
		Argv[0] = executable
	 	Argv[1] = number of sets (store in cacheStatistics.numSets)
	 	Argv[2] = line size in bytes (store in cacheStatistics.lineSize)
	 	Argv[3] = cache associativity (store in cacheStatistics.associativity)
	 	Argv[4] = trace file name (store in filename)
	 	Argv[5] = debug flag (store in debugFlag)
	*/
	if(argc != 6)
	{
		fprintf(stderr, "Usage: %s	#Sets	Line size in bytes	Associativity	Trace file name		Debug flag\n", argv[0]);
		return 1;
	}
	
	
	// Store command line arguments in proper variables
	// ------------------------------------------------
	arg1 = atoll(argv[1]);
	arg2 = atoll(argv[2]);
	arg3 = atoll(argv[3]);
	filename = argv[4];
    debugFlag = atoi(argv[5]);
    
    cacheStatistics.numSets = (long double)arg1;
    cacheStatistics.lineSize = (long double)arg2;
    cacheStatistics.associativity = (long double)arg3;
    

    // Compute and initialize all other cache attributes
    cacheStatistics.numLines = cacheStatistics.numSets * cacheStatistics.associativity;	
	cacheStatistics.cacheSize = cacheStatistics.numLines * cacheStatistics.lineSize ;	
	
    
    // Allocate memory for both sets and cache (initialize all bytes to 0 with calloc)
    // ---------------------------------------
    if((cachePtr = (struct set_t*)calloc(cacheStatistics.numSets, sizeof(struct set_t))) == NULL)
	{ 
		fprintf(stderr, "calloc failed\n");
		return 1;
	}
 
    for(i = 0; i < cacheStatistics.numSets; ++i)
    {
    	if((cachePtr[i]->setPtr = (struct line_t*)calloc(cacheStatistics.associativity, sizeof(struct line_t))) == NULL)
    	{ 
			fprintf(stderr, "calloc failed\n");
			return 1;
		}    	
    }

    
    // Open file and do error check
    // ----------------------------
 	traceFile = fopen(filename, "r"); 
 	if(traceFile == NULL)
	{ 
		fprintf(stderr, "fopen failed\n");
		exit(1);

	}
	
	/* While not EOF
	-----------------
			Read line + error check
			Parse line + eror check (See parsing algorithm)
			Save command number in command variable
			Save address index in set index variable
			Save address tag in line tag variable
			Examine command and choose command algorithm (See all commands algorithms)
			Execute all steps in the appropriate command algorithm
	*/
	
	
	// Print statistics for the current trace file
	// -------------------------------------------


	// Close file
	// ----------
	fclose(filename);
	
	
	// Deallocate memory for cache structure (all levels)
	// --------------------------------------------------
    free();
    free();

	return 0;
}


