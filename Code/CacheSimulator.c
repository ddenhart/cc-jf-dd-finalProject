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

#include "CacheSimulator.h"

/*==================================================================================
							     DATA STRUCTURES
==================================================================================*/

// Cache statistics struct
// -----------------------
struct statistics_t {
	float numAccesses;				// total number of cache accesses 
	float numHits;					// number of cache hits
	float hitRatio;					// cache hit ratio
	unsigned long int numReads;		// number of cache reads
	unsigned long int numWrites;	// number of cache writes
	unsigned long int numMisses;	// number of cache misses 
	
	unsigned long int cacheSize; 	// cache size in bytes
	unsigned long int lineSize;		// line size in bytes
	unsigned long int associativity;// cache degree of associativity
	unsigned long int numSets; 		// number of sets in the cache 
	unsigned long int numLines;		// number of lines in the cache 
} cacheStatistics;


// Cache line struct
// ------------------
typedef struct{
	unsigned int inclusivityBits;		// Line inclusivity bits (to be determined later)
	unsigned int mesifBits;				// Line MESIF coherence protocol bits
	unsigned int tagBits;				// Line Tag bits
} cacheLine;


// Cache set struct
// ----------------
typedef struct{
	unsigned int validLineCounter;		// Counter for number of valid lines in the set
									// Helpful when printing valid lines to screen
	unsigned int plruBits;				// Pseudo LRU replacement policy bits of the set
	cacheLine * setPtr;					// Pointer to a line_t struct
} cacheSet;


// Cache (dynamic array of set structs)
// ------------------------------------
cacheSet * cachePtr;					// Cache = array of sets

// Debug flag				
// ----------
int debugFlag;						// Debug flag across all functions

/*==================================================================================
							     MAIN FUNCTION
==================================================================================*/


int main(int argc, char * argv[])
{
	// Local variables
	// ---------------
	char filename[100];  			// array for storing the trace file name
	long int arg1, arg2, arg3;
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
	arg1 = atol(argv[1]);
	arg2 = atol(argv[2]);
	arg3 = atol(argv[3]);
	strcpy(filename, argv[4]);
    debugFlag = atoi(argv[5]);
    cacheStatistics.numSets = (unsigned long int)arg1;
    cacheStatistics.lineSize = (unsigned long int)arg2;
    cacheStatistics.associativity = (unsigned long int)arg3;
    

    // Compute and initialize all other cache attributes
    cacheStatistics.numLines = cacheStatistics.numSets * cacheStatistics.associativity;	
	cacheStatistics.cacheSize = cacheStatistics.numLines * cacheStatistics.lineSize ;	
	
    
    // Allocate memory for both sets and cache (initialize all bytes to 0 with calloc)
    // ---------------------------------------
	if ((cachePtr = (cacheSet*)calloc(cacheStatistics.numSets, sizeof(cacheSet))) == NULL)
	{
		fprintf(stderr, "calloc failed\n");
		exit(1);
	}
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
		fprintf(stderr, "fopen failed\n");
		exit(1);
	}
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


