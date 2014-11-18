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

==================================================================================
							     DATA STRUCTURES
==================================================================================

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
	unsigned long int numSets; 		// number of sets in the cache (might be redundant)
	unsigned long int numLines		// number of lines in the cache (might be redundant)
} cacheStatistics;


// Cache line struct
// ------------------
struct line_t {
	uint8_t inclusivityBits;		// Line inclusivity bits (to be determined later)
	uint8_t	mesifBits;				// Line MESIF coherence protocol bits
	uint16_t tagBits;				// Line Tag bits
} cacheLine;


// Cache set struct
// ----------------
struct set_t {
	uint8_t validLineCounter;		// Counter for number of valid lines in the set
									// Helpful when printing valid lines to screen
	uint16_t plruBits;				// Pseudo LRU replacement policy bits of the set
	line_t * setPtr;					// Pointer to a line_t struct
} cacheSet;


// Cache (dynamic array of set structs)
// ------------------------------------
set_t * cachePtr;					// Cache = array of sets

// Debug flag				
// ----------
int debugFlag;						// Debug flag across all functions

==================================================================================
							     MAIN FUNCTION
==================================================================================


int main(int argc, char * argv[])
{
	// Local variables
	// ---------------
	FILE * traceFile;				// file descriptor for trace file
	char filename[100];  			// array for storing the trace file name
	long int arg1, arg2, arg3;
	// other local variables		
      	
      	
	/* Read command line arguments and do error check
	   ----------------------------------------------
		Argv[0] = executable
	 	Argv[1] = cache size in bytes (store in cacheStatistics.cacheSize)
	 	Argv[2] = line size in bytes (store in cacheStatistics.lineSize)
	 	Argv[3] = cache associativity (store in cacheStatistics.associativity)
	 	Argv[4] = trace file name (store in filename)
	 	Argv[5] = debug flag (store in debugFlag)
	*/
	if(argc != 6)
	{
		fprintf(stderr, "Usage: %s argument\n", argv[0]);
		return 1;
	}
	
	
	// Store command line arguments in proper variables
	// ------------------------------------------------
	arg1 = atol(argv[1]);
	arg2 = atol(argv[2]);
	arg3 = atol(argv[3]);
	filename = argv[4];
    debugFlag = atoi(argv[5]);
    
    cacheStatistics.cacheSize = (unsigned long int)arg1;
    cacheStatistics.lineSize = (unsigned long int)arg2;
    cacheStatistics.associativity = (unsigned long int)arg3;
    

    // Compute and initialize all other cache attributes
	cacheStatistics.numLines = cacheStatistics.cacheSize / cacheStatistics.lineSize;	
	cacheStatistics.numSets = cacheStatistics.numLines / cacheStatistics.associativity; 	
    
    
    // Allocate memory for both sets and cache (initialize all bytes to 0 with calloc)
    // ---------------------------------------
    if(cacheSet.setPtr = (line_t*)calloc(sizeof(line_t*cacheStatistics.associativity)) == NULL)
    { 
		fprintf(stderr, "calloc failed\n");
		exit(1);

	}
    
    if(cachePtr = (set_t*)calloc(sizeof(set_t*cacheStatistics.numSets)) == NULL)
    { 
		fprintf(stderr, "calloc failed\n");
		exit(1);

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

==================================================================================
							 UTILITY FUNCTIONS
==================================================================================
void OutputValidLines()
{
	int i, j;
	unsigned int mesif;
	for(i = 0; i < cacheStatistics.numSets; ++i)
	{
		if(cachePtr[i]->validLineCounter != 0)
		{
			for(j = 0; j < cacheStatistics.asscociativity; ++j)
			{
				mesif = getMESIF(i, j);
				if(mesif != INVALID)
				{
					printf();
				}					
			}
		}
	}
	
}


unsigned int getMESIF(unsigned int set, unsigned int line)
{

}	

// more to be determined
	
==================================================================================
							 PROTOCOL FUNCTIONS
==================================================================================

// MESIF functions
void BusOperation(char BusOp, unsigned int Address, char * SnoopResult)
{

}

char GetSnoopResult(unsigned int Address)
{

}

void PutSnoopResult(unsigned int Address, char Snoop Result)
{

}

// Inclusivity functions
void MessageToL2Cache(char BusOp, unsigned int Address)
{

{

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


