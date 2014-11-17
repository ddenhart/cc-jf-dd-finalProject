==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			CacheREADME.txt
Date:			11/16/2014
Author:			Carmen Ciobanu
Description: 	This file contains:
	- A list and description of all the files in the project
	- A list and description of all the algorithms needed for the project
	- A list and description of all the data structures needed for the project
	- A list and description of all the functions in the project
	
==================================================================================
									FILES 
==================================================================================
The project contains the following files:

Code
-----
	CacheSimulator.h 
	CacheSimulator.c
		
Test
-----
	TraceFile.txt

Documentation
--------------
	CacheREADME.txt
	CacheReport.docx
	CacheReport.pdf

CacheSimulator.h 
-----------------
- # defines 
- # includes
- function prototypes

CacheSimulator.c
----------------- 
- global definitions (our data structures: structs and arrays)
- main function
- utility functions (bit shifting functions, set and get type functions,output, other)
- specific protocol functions (LRU, MESIF, inclusivity)

==================================================================================
									ALGORITHMS
==================================================================================
								  
								   Main Function 
----------------------------------------------------------------------------------
	
1. Read command line arguments	+ error check
2. Store command line arguments in proper variables
3. Allocate memory for cache data structure (all levels)
4. Initialize all allocated memory to 0 + error checks
5. Open trace file +  error checks
6. While not End Of File
		Read line + error check
		Parse line + eror check (See parsing algorithm)
		Save command number in command variable
		Save address index in set index variable
		Save address tag in line tag variable
		Examine command and choose command algorithm (See all commands algorithms)
		Execute all steps in the appropriate command algorithm
6. Print statistics for the current trace file
7. Close file
8. Deallocate memory for cache structure (all levels)
9. Done


----------------------------------------------------------------------------------
								 Parsing Algorithm  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 0  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 1  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 2  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 3  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 4  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 5  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 6  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 8  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
								   Command 9  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
						      Pseudo LRU Algorithm  
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
							  Inclusivity Algorithm 
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
							     MESIF Algorithm
----------------------------------------------------------------------------------


==================================================================================
							     DATA STRUCTURES
==================================================================================

Cache Statistics
----------------
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


Cache Line: struct
------------------
struct line_t {
	uint8_t inclusivityBits;		// Line inclusivity bits (to be determined later)
	uint8_t	mesifBits;				// Line MESIF coherence protocol bits
	uint16_t tagBits;				// Line Tag bits
} cacheLine;


Cache set: struct
---------------------------
struct set_t {
	uint8_t validLineCounter;		// Counter for number of valid lines in the set
									// Helpful when printing valid lines to screen
	uint16_t plruBits;				// Pseudo LRU replacement policy bits of the set
	line_t * setPtr					// Pointer to a line_t struct
} cacheSet;


Cache: dynamic array of structs
-------------------------------
set_t * cachePtr;					// Cache = array of sets

 
Debug flag				
----------
int debugFlag;						// Debug flag across all functions


==================================================================================
									FUNCTIONS
==================================================================================
							    PROTOCOL FUNCTIONS 
----------------------------------------------------------------------------------
// MESIF functions
void BusOperation(char BusOp, unsigned int Address, char * SnoopResult);
char GetSnoopResult(unsigned int Address);
void PutSnoopResult(unsigned int Address, char Snoop Result);

// Inclusivity functions
void MessageToL2Cache(char BusOp, unsigned int Address);

// Pseudo LRU functions
void SetPseudoLRU();
void GetPseudoLRU();

// Other
void ReadMemory();
void WriteMemory();

----------------------------------------------------------------------------------
								UTILITY FUNCTIONS
----------------------------------------------------------------------------------
void OutputValidLines();
unsigned int getMESIF(unsigned int set, unsigned int line);






		