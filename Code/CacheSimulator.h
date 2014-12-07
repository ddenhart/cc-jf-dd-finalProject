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
				Deborah Denhart
				Jeremiah Franke
Modified:		Deborah Denhart		12/02/2014
				Jeremiah Franke		12/04/2014
				Carmen Ciobanu		12/04/2014
                
Description: 	This file contains:
				- #defines
				- #includes
				- function prototypes
================================================================================== */


// #defines
// ---------
#define TRUE 1
#define FALSE 0

#define DEBUG_LRU		1
#define DEBUG				1
#define SILENT				1
#define ADDR_SIZE			32		// All addresses are 32 bits
#define INPUT_BUFFER_SIZE 	100
#define VICTIM_CACHE_SIZE	8

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

// Pseudo LRU #defines
#define CACHE_HIT			1
#define CACHE_MISS			0

//bus operation types
#define READ 1
#define WRITE 2
#define INVALIDATE 3
#define RWIM 4

//snoop results
#define NOHIT 0
#define HIT 1
#define HITM 2

//#define ALWAYS				1  //For the two utility prototypes.

// victimCache actions
#define INSERT				0
#define CHECK				1

//MESIF states
//Invalid was chosen to be first in the enum list so that it could be also used
//to initialize the bits
enum Mesif_states {
	eINVALID = 0, // Cache line is invalid
	eMODIFIED = eINVALID + 1, // Only this cache has the line and the memory is stale
	eEXCLUSIVE = eMODIFIED + 1, // Only this cache has the line and the memory is up to date 
	eSHARED = eEXCLUSIVE + 1, // At least two other caches have the line and memory is up to date
	eFORWARD = eSHARED + 1, // Cache line is in the shared state and this is the forwarding processor
	eMAX_STATES = eFORWARD + 1
};


/*  DATA STRUCTURES  */

// Cache victim struct
struct vCache_C {
	long int * vCacheArray;				//Victim Array Buffer
} vCache;

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
struct vCache_C victimPtr;			// Victim Pointer

// Dynamic array for pseudo LRU binary search
int * binarySearchArray;			


/*  FUNCTION PROTOTYPES  */
// Utility Prototypes
// ------------------
void ParseAddress(unsigned int * address, unsigned int * index, unsigned int * tag);
unsigned int GetLineAddress(unsigned int address);
int ConvertToBase(int num);
void SetLineTag();
int CreateCache();
void DestroyCache();
void InitParams();
int ValidateInputs();

// Commands Functions
int ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress);
int ExecuteCommand1(unsigned int index, unsigned int tag, unsigned int HexAddress);
int ExecuteCommand3(unsigned int HexAddress);
int ExecuteCommand4(unsigned int HexAddress);
int ExecuteCommand5(unsigned int HexAddress);
int ExecuteCommand6(unsigned int HexAddress);
int ExecuteCommand8();
int ExecuteCommand9();

// Pseudo LRU Functions
int GetMidpoint(int min, int max);
int SetPseudoLRU(unsigned int set, unsigned int line, int min, int max);
int GetVictimLine(unsigned int set, int min, int max, int * bitToRead, int * bitValue);
int UpdateLRU(unsigned int set, unsigned int line, int min, int max, int flag, int * eviction);

// MESIF Functions
void InitMesif();
void UpdateMesif(unsigned int cmd, unsigned int address, unsigned int set, unsigned int line, int found);
unsigned int GetMesifState(unsigned int set, unsigned int line);
// Used to simulate a bus operation and to capture the 
// snoop results of last level caches of other processors
void BusOperation(int BusOp, unsigned int Address, unsigned int  *SnoopResult);

// Inclusion Function
void MessageToL2Cache(int cmd, unsigned int address, int * eviction, unsigned int evictedLineAddr);

// Memory and Write Buffer Functions
void ReadMemory(unsigned int address);
void WriteMemory(unsigned int address);




