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

//cc TODO these parameters are not correct, see function "validateInputs" for correct values of all params
// default cache parameters - set to max available?
#define WORD_SIZE 8
#define MAX_CAPACITY (8 * 1024 * 1024)
#define BLOCK_SIZE 8
#define MAX_ASSOCIATIVITY 16
#define LINES_SIZE 64

//dd TODO: should we use silent or debug? i am used to debug but faust uses silent...
//cc DEBUG and SILENT are different and we need both
//cc DEBUG is just for us to print whatever for debug
//cc SILENT is for suppressing all output except for statistics
#define DEBUG				1
#define SILENT				1
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

// Pseudo LRU #defines
#define CACHE_HIT			1
#define CACHE_MISS			0

// consts
#define INPUT_BUFFER_SIZE 100
//#define ALWAYS				1  //For the two utility prototypes.

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
// Utility Prototypes
// ------------------
//char* handleInputs(char **argv, int argc, char *filename);
//void setCacheParams(long int *arg);
unsigned int takeLogBase2 (unsigned int vars);
void ParseAddress(unsigned int * address, unsigned int * index, unsigned int * tag);
int ConvertToBase(int num);
void UpdateMesif(unsigned int address, unsigned int cmd, unsigned int set, unsigned int line);
unsigned int GetLineTag(unsigned int set, unsigned int line);
void SetLineTag();
int CreateCache();
void DestroyCache();
void InitParams();
int ValidateInputs();

// Commands Functions
int ExecuteCommands02(unsigned int index, unsigned int tag, unsigned int HexAddress);
int ExecuteCommand1(unsigned int index, unsigned int tag, unsigned int HexAddress);
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
void MessageToL2Cache(char BusOp, unsigned int Address);

// Memory and Write Buffer Functions
void ReadMemory(unsigned int address);
void WriteMemory(unsigned int address);




