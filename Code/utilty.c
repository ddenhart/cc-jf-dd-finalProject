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
#include "mesif.h"
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
	ConvertToBase(cacheStatistics.lineSize+33);
	unsigned int numOffsetBits = ConvertToBase(cacheStatistics.lineSize);
	unsigned int numIndexBits = ConvertToBase(cacheStatistics.numSets);
	unsigned int addressSize = ADDR_SIZE;
	unsigned int numTagBits = addressSize - (numOffsetBits + numIndexBits);
	printf("\n%d, %d, %d, %d",addressSize, numOffsetBits, numIndexBits, numTagBits);
	*index = ((*address) << numTagBits) >> (numOffsetBits + numTagBits);
	*tag = (*address) >> (numOffsetBits + numIndexBits);
}


#ifdef ALWAYS
/* ==================================================================================
	Function name:	handleInputs
 	Arguments:		char* argv
	Returns:		char*
	Description:	Read command line arguments and do error check
================================================================================== */
char* handleInputs(char **argv, int argc, char* filename)
{
	/* 
	   ----------------------------------------------
		Argv[0] = executable
	 	Argv[1] = number of sets (store in cacheStatistics.numSets)
	 	Argv[2] = line size in bytes (store in cacheStatistics.lineSize)
	 	Argv[3] = cache associativity (store in cacheStatistics.associativity)
	 	Argv[4] = trace file name (store in filename)
	*/

	int badInput = TRUE;
	char delim = ' ';
	long int arg[3]; 			// array for storing the trace file name
	char inputs[INPUT_BUFFER_SIZE];
	char *reInputs[5];

	if(argc != 5)
	{
		argc = 0;
		//let user keep trying until good input
		while(badInput)
			fprintf(stderr, "Usage: %s	#Sets	Line size in bytes	Associativity	Trace file name		Debug flag\n", argv[0]);
			if( (fgets(inputs, INPUT_BUFFER_SIZE, stdin) != NULL ) ) 
			{
				// get the first string
				reInputs[argc] = strtok(inputs, &delim);
				argc++;
				/* walk through other tokens */
				while( (reInputs[argc] != NULL) && (argc < 4) ) 
				{
					reInputs[argc] = strtok(NULL, &delim);
					argc++;
				}
				if( (argc >= 4) && (argc >= 5))
				{
					badInput = FALSE;
					arg[0] = atoll(reInputs[1]); //dd TODO: not sure atoll works in c
					arg[1] = atoll(reInputs[2]);
					arg[2] = atoll(reInputs[3]);
					memset(filename, '\0', sizeof(filename));
					strcpy(filename, reInputs[4]);
					setCacheParams(arg);
				}
			}
			
	}
	else
	{
		// Store command line arguments in proper variables
		// ------------------------------------------------
		arg[0] = atoll(argv[1]); //dd TODO: not sure atoll works in c
		arg[1] = atoll(argv[2]);
		arg[2] = atoll(argv[3]);
		memset(filename, '\0', sizeof(filename));
		strcpy(filename, argv[4]);
		setCacheParams(arg);
	}
}


/* ==================================================================================
	Function name:	setCacheParams
 	Arguments:		long int *arg
	Returns:		void
	Description:	set new parameters
================================================================================== */
void setCacheParams(long int *arg)
{
    cacheStatistics.numSets = arg[1];
    cacheStatistics.lineSize = arg[2];
    cacheStatistics.associativity = arg[3];
    
    // Compute and initialize all other cache attributes
    cacheStatistics.numLines = cacheStatistics.numSets * cacheStatistics.associativity;	
	cacheStatistics.cacheSize = cacheStatistics.numLines * cacheStatistics.lineSize ;	
}
#endif


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
int GetLineTag(unsigned int set, unsigned int line)
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
	Function name:	UpdateMesif
 	Arguments:		unsigned int address - the address input from the trace file
					unsigned int cmd - the command from the trace file
					unsigned int set - number of the set containing the desired line
 	 	 	 	 	unsigned int line - number of the line we want to get the MESIF bits for
	Returns:		void
	Description:	Update the MESIF bits
   ================================================================================== */
void UpdateMesif(unsigned int address, unsigned int cmd, unsigned int set, unsigned int line)
{
		
}


/* ==================================================================================
	Function name:	CreateCache
 	Arguments:
	Returns:		void
	Description:
   ================================================================================== */
int CreateCache()
{
	int i;

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
 	Arguments:
	Returns:		void
	Description:
   ================================================================================== */
void DestroyCache()
{
	int i;

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
 	Arguments:
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


 // ==========================================
// Move to memory read/write and buffer file (???!!!!!!!!)
// ===========================================
void ReadMemory(unsigned int address)
{
	// TODO cc changed to ifndef SILENT because we don't want output when SILENT
	// TODO cc changed address to be output in hex
#ifndef SILENT
	printf("Memory read from Address: %#x\n", address);
#endif
}

void WriteMemory(unsigned int address)
{
	// TODO cc changed to ifndef SILENT because we don't want output when SILENT
	// TODO cc changed address to be output in hex
#ifndef SILENT
	printf("Memory write to Address: %#x", address);
#endif
}

/* ===============================================================================
takeLogBase2: Returns log base 2 of the input

@input: unsigned int vars

@output: unsigned int

================================================================================== */
unsigned int takeLogBase2 (unsigned int vars)
{
	double base = 2.;
	double dvars = (double)vars;
	double quot = log(dvars);
	double div = log(base);
	double result = (quot/div);
	return (int)result;
}
