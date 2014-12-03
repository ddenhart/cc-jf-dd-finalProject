/* ==================================================================================
	ECE 485 / Fall 2014 / Cache simulator project
	Team:
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
   ==================================================================================
	File:				utility.c
	Date:			11/16/2014
	Author:			Carmen Ciobanu
						Jeremiah Franke
	Description:	This file contains: UTILITY FUNCTIONS
   ================================================================================== */


#include "CacheSimulator.h"


/* ==================================================================================
	Function name:	ParseAddress
 	Arguments:			unsigned int * address
 	 	 	 	 				unsigned int * index
 	 	 	 	 				unsigned int * tag
	Returns:				void
	Description:			Address processing function (bit shifting utility function)
   ================================================================================== */
void ParseAddress(unsigned int * address, unsigned int * index, unsigned int * tag)
{
	ConvertToBase(cacheStatistics.lineSize+33);
	unsigned int numOffsetBits = ConvertToBase(cacheStatistics.lineSize);
	//long double numOffsetBits = log2l(cacheStatistics.lineSize);
	//unsigned long long int numOffsetBits = log2(cacheStatistics.lineSize);
	//long double numIndexBits = log2l(cacheStatistics.numSets);
	unsigned int numIndexBits = ConvertToBase(cacheStatistics.numSets);
	//unsigned int numIndexBits = log2(cacheStatistics.numSets);
	unsigned int addressSize = ADDR_SIZE;
	//unsigned long long int addressSize = ADDR_SIZE;
	//long double numTagBits = addressSize - (numOffsetBits + numIndexBits);
	unsigned int numTagBits = addressSize - (numOffsetBits + numIndexBits);
	//unsigned long long int numTagBits = addressSize - (numOffsetBits + numIndexBits);
	printf("\n%d, %d, %d, %d",addressSize, numOffsetBits, numIndexBits, numTagBits);
	*index = ((*address) << numTagBits) >> (numOffsetBits + numTagBits);
	*tag = (*address) >> (numOffsetBits + numIndexBits);
}


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
	Function name:	OutputValidLines
 	Arguments:			void
	Returns:				void
	Description:			Output the contents and state of all valid lines in the cache
   ================================================================================== */
void OutputValidLines()
{
	unsigned int i, j, mesifState;
	int valid;

	// Display valid line information
	printf("\nDisplaying valid lines information\n");
	printf("----------------------------------\n");

	for(i = 0; i < cacheStatistics.numSets; ++i)
	{
		valid = 0;

			for(j = 0; j < cacheStatistics.associativity; ++j)
			{
				mesifState = GetMesifState(i, j);
				if(mesifState != 0)
				{
					// Print line contents and state
					printf("Set %5u       Line %2u       Tag  %#8x       MESIF state: %2u\n", i, j, cachePtr[i].setPtr[j].tagBits, cachePtr[i].setPtr[j].mesifBits);
					valid = 1;
				}
			}
			if(valid)
			{
				printf("-----------------------");
				printf("\nSet %u  LRU %#x\n\n", i, cachePtr[i].plruBits);
			}
	}
}


/* ==================================================================================
	Function name:	OutputStatistics
 	Arguments:			void
	Returns:				void
	Description:			Outputs cache architecture and statistics for current trace file
   ================================================================================== */
void OutputStatistics()
{
	printf("\nCache Architecture\n");
	printf("------------------\n");
	printf("CACHE SIZE:            %u\n", cacheStatistics.cacheSize);
	printf("ASSOCIATIVITY:         %u\n", cacheStatistics.associativity);
	printf("NUMBER OF SETS:        %u\n", cacheStatistics.numSets);
	printf("LINE SIZE:             %u\n", cacheStatistics.lineSize);
	printf("TOTAL NUMBER OF LINES: %u\n", cacheStatistics.numLines);

	printf("\n\nCache Statistics for Current Trace File\n");
	printf("---------------------------------------\n");
	printf("NUMBER OF CACHE ACCESSES: %f\n", cacheStatistics.numAccesses);
	printf("NUMBER OF CACHE HITS:     %f\n", cacheStatistics.numHits);
	printf("CACHE HIT RATIO:          %f\n", cacheStatistics.hitRatio);
	printf("NUMBER OF CACHE READS:    %f\n", cacheStatistics.numReads);
	printf("NUMBER OF CACHE WRITES:   %f\n", cacheStatistics.numWrites);
	printf("NUMBER OF CACHE MISSES:   %f\n\n", cacheStatistics.numMisses);

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
	Function name:	GetMesifState
 	Arguments:		unsigned int set = number of the set containing the desired line
 	 	 	 	 	unsigned int line = number of the line we want to get the MESIF bits for
	Returns:		void
	Description:	Read the line MESIF state
   ================================================================================== */
int GetMesifState(unsigned int set, unsigned int line)
{
		return cachePtr[set].setPtr[line].mesifBits;
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


// ==================================================================
// MESIF functions - do we still need these, are we moving these to mesif.c ??? !!!!!!!!
// ==================================================================
/*
void BusOperation(char BusOp, unsigned int Address, char * SnoopResult)
{

}

char GetSnoopResult(unsigned int Address)
{
	return 0;

}

void PutSnoopResult(unsigned int Address, char SnoopResult)
{

}

 // ==========================================
// Move to memory read/write and buffer file (???!!!!!!!!)
// ===========================================
void ReadMemory(unsigned int address)
{

}

void WriteMemory(unsigned int address)
{

}
*/

