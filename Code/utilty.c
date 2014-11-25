/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			utility.c
Date:			11/16/2014
Author:			
Description: 	This file contains:
				- 
	
================================================================================== */
#include "CacheSimulator.h"

/*==================================================================================
							 UTILITY FUNCTIONS
==================================================================================*/

// Address processing function (bit shifting utility function)
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

// Output the contents and state of all valid lines int the cache
// --------------------------------------------------------------
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
				mesifState = GetMesifState(i, j);
				if(mesifState != INVALID)
				{
					printf();	// Print line contents and state
				}					
			}
		}
	}
	
}

//alreadly in mesif files
// Read the line MESIF state
// -------------------------
//unsigned int GetMesifState(unsigned int set, unsigned int line) 
//{
//	return cachePtr[set].setPtr[line].mesifBits;
//}	


// Change the line MESIF state
// ---------------------------
//int SetMesifState(unsigned int set, unsigned int line, unsigned int newValue) 
//{
//	cachePtr[set].setPtr[line].mesifBits = newValue;
//	if((cachePtr[set].setPtr[line].mesifBits < INVALID) || (cachePtr[set].setPtr[line].mesifBits > MODIFIED))
//	{
//		return 1; // If the new MESIF state is not a valid MESIF state, return error code 1
//	}
//	
//}	

// Read the set pseudo LRU bits
// ----------------------------
unsigned int ReadPseudoLRU(unsigned int set)		
{
	return cachePtr[set].plruBits;
}


// Read the line inclusivity bits
// ------------------------------
unsigned int GetInclusivityBits(unsigned int set, unsigned int line)	
{
	return cachePtr[set].setPtr[line].inclusivityBits;
}


// Write the line inclusivity bits
// ------------------------------
unsigned int SetInclusivityBits()	
{

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


