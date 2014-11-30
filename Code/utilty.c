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
#include "mesif.h"

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
	Returns:				void
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
	unsigned int i, j;
	unsigned int mesifState;
	for(i = 0; i < cacheStatistics.numSets; ++i)
	{
		if(cachePtr[i].validLineCounter != 0)
		{
			for(j = 0; j < cacheStatistics.associativity; ++j)
			{
				/*
				mesifState = GetMesifState(i, j);
				if(mesifState != eINVALID)
				{
					printf();	// Print line contents and state
				}		
				*/			
			}
		}
	}
	
}



/* ==================================================================================
	Function name:	GetMesifState
 	Arguments:		unsigned int set = number of the set containing the desired line
 	 	 	 	 	unsigned int line = number of the line we want to get the MESIF bits for
	Returns:		void
	Description:	Read the line MESIF state
   ================================================================================== */
/*
unsigned int GetMesifState(unsigned int set, unsigned int line)
{
		return cachePtr[set]->setPtr[line]->mesifBits;
}
*/

/* ==================================================================================
	Function name:	GetInclusivityBits
 	Arguments:			unsigned int set = number of the set containing the desired line
 	 	 	 	 	 	 	 	unsigned int line = number of the line we want to get the inclusivity bits for
	Returns:				unisgned int = contains the value of the inclusivity bits
	Description:			Read the inclusivity bits value for a line
   ================================================================================== */

unsigned int GetInclusivityBits(unsigned int set, unsigned int line)	
{
	return cachePtr[set]->setPtr[line]->inclusivityBits;
}


// Write the line inclusivity bits
// ------------------------------
unsigned int SetInclusivityBits()	
{
	return 0;

}


// Read the line tag
// -----------------
unsigned int GetLineTag(unsigned int set, unsigned int line)		
{
	return cachePtr[set].setPtr[line].tagBits;
}


// Write the line tag
// ------------------
unsigned int SetLineTag()		
{
	return 0;
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
	return 0;

}

void PutSnoopResult(unsigned int Address, char SnoopResult)
{

}

// Inclusivity functions
void MessageToL2Cache(char BusOp, unsigned int Address)
{

}

// Pseudo LRU functions
void WritePseudoLRU()	// Write the pseudo LRU bits
{

}

void GetVictimLine() // Find line to be evicted
{

}

// Other
void ReadMemory()
{

}

void WriteMemory()
{

}

