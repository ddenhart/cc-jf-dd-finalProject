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
<<<<<<< HEAD
	char filename[100];  			// array for storing the trace file name
	long int arg1, arg2, arg3;
=======
	FILE * traceFile;				// file descriptor for trace file
	char filename[100];  			// array for storing the trace file name
	long long arg1, arg2, arg3;
	int i;
>>>>>>> origin/master
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
<<<<<<< HEAD
	for (unsigned int i = 0; i < cacheStatistics.numSets; ++i)
	{
		if ((cachePtr[i].setPtr = (cacheLine*)calloc(cacheStatistics.associativity, sizeof(cacheLine))) == NULL)
		{
			fprintf(stderr, "calloc failed\n");
			exit(1);
		}
	}
	
	ParseFile(filename);
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

	
	
	// Deallocate memory for cache structure (all levels)
	// --------------------------------------------------
    free(cachePtr->setPtr);
    free(cachePtr);

	return 0;
}

/*==================================================================================
							 UTILITY FUNCTIONS
==================================================================================*/
void ParseFile(char * Filename)
{
	FILE * traceFile;				// file descriptor for trace file
	unsigned int characterInLine;   // Variable to hold character in line
	unsigned int operation;
	char lineHolder[SIZE];
	int counter = 0;
	int isSpace = 0;

	// Open file and do error check
	// ----------------------------
	traceFile = fopen(Filename, "r");
	if (traceFile == NULL)
	{
=======
 
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
>>>>>>> origin/master
		fprintf(stderr, "fopen failed\n");
		exit(1);

	}
<<<<<<< HEAD
	while (1)
	{
		characterInLine = fgetc(traceFile);
		if (feof(traceFile))
		{
			lineHolder[counter] = '\0';
			ParseAddress(lineHolder);
			break;
		}
		if (characterInLine == '\n')
		{
			lineHolder[counter] = '\0';
			ParseAddress(lineHolder);
			isSpace = 0;
			continue;
		}
		if (isspace(characterInLine))
		{
			isSpace = 1;
			counter = 0;
			continue;
		}
		if (!isSpace)
			operation = characterInLine;
		else
			lineHolder[counter] = characterInLine;
		++counter;
	}
	// Close file
	// ----------
	printf("tester");
	fclose(traceFile);
	return;
}
=======
	
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
>>>>>>> origin/master

void ParseAddress(char * HexAddress)
{
	int mini = 0;
	int e = 0;
	char holder[100];
	_set_printf_count_output(1);
	sscanf(HexAddress, "%x", &mini);
	printf("%d\n",mini);
	strtol()
	//sprintf(holder, "%x", mini);
	printf(holder);
	printf("%s\n",HexAddress);
	return;
}

<<<<<<< HEAD
void OutputValidLines()
{
	unsigned int i, j;
	unsigned int mesif;
	for(i = 0; i < cacheStatistics.numSets; ++i)
	{
		if(cachePtr[i].validLineCounter != 0)
		{
			for(j = 0; j < cacheStatistics.associativity; ++j)
			{
				mesif = getMESIF(i, j);
				if(mesif != INVALID)
				{
					printf("PlaceHolderText\n");
				}					
			}
		}
	}
	
}


unsigned int getMESIF(unsigned int set, unsigned int line)
{
	unsigned int returnValue = 1;
	return returnValue;
}	

// more to be determined
	
/*==================================================================================
							 PROTOCOL FUNCTIONS
==================================================================================*/

// MESIF functions
void BusOperation(char BusOp, unsigned int Address, char * SnoopResult)
{

}

char GetSnoopResult(unsigned int Address)
{
	char c = 'a';
	return c;
}

void PutSnoopResult(unsigned int Address, char SnoopResult)
{

}

// Inclusivity functions
void MessageToL2Cache(char BusOp, unsigned int Address)
{

}

// Pseudo LRU functions
void SetPseudoLRU()
{

}

void GetPseudoLRU()
{

}

// Other
void ReadMemory()
{

}

void WriteMemory()
{

}

=======
>>>>>>> origin/master

