/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			CacheSimulator.h
Date:			11/16/2014
Author:			Carmen Ciobanu
Description: 	This file contains:
				- #defines
				- #includes
				- function prototypes
	
================================================================================== */


// #defines
// ---------

// Address 
// --------

#define ADDR_SIZE			32		// All addresses are 32 bits

// Trace File Commands
#define DATA_READ_REQ		0		// Command 0 = read request from L2 data cache
#define DATA_WRITE_REQ		1		// Command 1 = write request from L2 data cache
#define INSTR_READ_REQ		2		// Command 2 = read request from L2 instruction cache
#define SNOOPED_INVALIDATE	3		// Command 3 = snooped invalidate command
#define SNOOPED_READ		4		// Command 4 = snooped read request
#define SNOOPED_WRITE		5		// Command 5 = snooped write request
#define	SNOOPED_RWIM		6		// Command 6 = snooped read with intent to modify
#define CLEAR_CACHE			8		// Command 8 = clear cache / reset all states
#define PRINT_VALID_LINES	9		// Command 9 = print valid lines' contents and state



/*==================================================================================
							     DATA STRUCTURES
==================================================================================*/

// Cache statistics struct
// -----------------------
struct statistics_t {
	double numAccesses;				// total number of cache accesses 
	double numHits;					// number of cache hits
	double hitRatio;				// cache hit ratio
	double numReads;				// number of cache reads
	double numWrites;				// number of cache writes
	double numMisses;				// number of cache misses 
	
	unsigned int cacheSize; 		// cache size in bytes
	unsigned int lineSize;			// line size in bytes
	unsigned int associativity;		// cache degree of associativity
	unsigned int numSets; 			// number of sets in the cache 
	unsigned int numLines;			// number of lines in the cache 
} cacheStatistics;


// Cache line struct
// ------------------
struct line_t {
	unsigned int inclusivityBits;	// Line inclusivity bits (to be determined later)
	unsigned int mesifBits;			// Line MESIF coherence protocol bits
	unsigned int tagBits;			// Line Tag bits
};


// Cache set struct
// ----------------
struct set_t {
	unsigned int validLineCounter;	// Counter for number of valid lines in the set
									// Helpful when printing valid lines to screen
	unsigned int plruBits;			// Pseudo LRU replacement policy bits of the set
	struct line_t * setPtr;			// Pointer to a line_t struct
};


// Cache (dynamic array of set structs)
// ------------------------------------
struct set_t * cachePtr;			// Cache = array of sets

// Debug flag - alternate to ifdef				
// ----------
//int debugFlag;						// Debug flag across all functions
//Utility Prototypes
//------------------
void ParseAddress(unsigned int * address, unsigned int * index, unsigned int * tag);
void ConvertToBase(int num);
void OutputValidLines();
unsigned int GetMesifState(unsigned int set, unsigned int line);
int SetMesifState(unsigned int set, unsigned int line, unsigned int newValue);
unsigned int ReadPseudoLRU(unsigned int set);
unsigned int GetInclusivityBits(unsigned int set, unsigned int line);
unsigned int SetInclusivityBits();
unsigned int GetLineTag(unsigned int set, unsigned int line);
unsigned int SetLineTag();

// Function Prototypes
// -------------------

void MessageToL2Cache(char BusOp, unsigned int Address);
void SetPseudoLRU();
void GetPseudoLRU();

void OutputValidLines();  

void ReadMemory();
void WriteMemory();




