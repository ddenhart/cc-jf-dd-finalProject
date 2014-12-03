/* ==================================================================================
   ECE 485 / Fall 2014 / Cache simulator project
   Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
   File:              CacheSimulator.h
   Date:            11/16/2014
   Author:         Carmen Ciobanu
                        Deborah Denhart
                        Jeremiah Franke
   Description: 	This file contains:
                        - #defines
                        - data structures
                        - function prototypes
   ================================================================================== */


#include <stdint.h>				// for special data types like uint8_t, uint16_t
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling


//#define SILENT                            1
#define DEBUG                             1
#define ADDR_SIZE                      32		// All addresses are 32 bits

// Trace File Commands
#define DATA_READ_REQ            0		// Command 0 = read request from L2 data cache
#define DATA_WRITE_REQ          1		// Command 1 = write request from L2 data cache
#define INSTR_READ_REQ           2		// Command 2 = read request from L2 instruction cache
#define SNOOPED_INVALIDATE   3		// Command 3 = snooped invalidate command
#define SNOOPED_READ              4		// Command 4 = snooped read request
#define SNOOPED_WRITE            5		// Command 5 = snooped write request
#define	SNOOPED_RWIM           6		// Command 6 = snooped read with intent to modify
#define CLEAR_CACHE                 8		// Command 8 = clear cache / reset all states
#define PRINT_VALID_LINES        9		// Command 9 = print valid lines' contents and state

// Pseudo LRU #defines
#define EVICT_LINE						0
#define CACHE_HIT						1
#define CACHE_MISS					2
#define FALSE								0
#define TRUE									1


/*  DATA STRUCTURES  */

// Cache statistics struct
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
struct line_t {
	unsigned int mesifBits;			// Line MESIF coherence protocol bits
	unsigned int tagBits;			// Line Tag bits
};

// Cache set struct
struct set_t {
	unsigned int plruBits;					// Pseudo LRU replacement policy bits of the set
	struct line_t * setPtr;					// Pointer to a line_t struct
};

// Cache (dynamic array of set structs)
struct set_t * cachePtr;			// Cache = array of sets

// Dynamic array for pseudo LRU binary search
int * binarySearchArray;			


/*  FUNCTION PROTOTYPES  */

// Utility Functions
void ParseAddress(unsigned int * address, unsigned int * index, unsigned int * tag);
int ConvertToBase(int num);
void OutputValidLines();
void OutputStatistics();
int GetMesifState(unsigned int set, unsigned int line);
unsigned int GetLineTag(unsigned int set, unsigned int line);
void SetLineTag();
int CreateCache();
void DestroyCache();
void InitParams();
int ValidateInputs();

// Commands Functions
void ExecuteCommands02(unsigned int index, unsigned int tag);
void ExecuteCommand1(unsigned int index, unsigned int tag);
void ExecuteCommand3(unsigned int index, unsigned int tag);
void ExecuteCommand4(unsigned int index, unsigned int tag);
void ExecuteCommand5(unsigned int index, unsigned int tag);
void ExecuteCommand6(unsigned int index, unsigned int tag);
void ExecuteCommand8();
void ExecuteCommand9();

// Pseudo LRU Functions
int GetMidpoint(int min, int max);
int SetPseudoLRU(unsigned int set, unsigned int line, int min, int max);
int GetVictimLine(unsigned int set, int min, int max, int * bitToRead, int * bitValue);
int UpdateLRU(unsigned int set, unsigned int line, int min, int max, int flag, int * eviction);

// Inclusion Function
void MessageToL2Cache(char BusOp, unsigned int address);

// Memory and Write Buffer Functions
void ReadMemory(unsigned int address);
void WriteMemory(unsigned int address);




