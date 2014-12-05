/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			output.h
Date:			11/16/2014
Author:			Deborah Denhart
Modified:		Deborah Denhart		12/02/14
Description: 	This file contains:
				- 
	
================================================================================== */
#include <stdio.h>
#include <math.h>
#include "output.h"
#include "CacheSimulator.h"


// TODO please select one of printStatistics()+printSettings and OutputStatistics()

/* ===============================================================================
printStatistics: prints accesses, hits, misses reads and writes

@input: None

@output: None

================================================================================== */

void printStatistics()
{
	double dHitRate = cacheStatistics.numMisses / cacheStatistics.numAccesses;
	double dMissRate = 1.0 - dHitRate;
	double dTolerance = 0.1*dHitRate;

	if (( abs(cacheStatistics.hitRatio - dHitRate) > dTolerance) || 
		( abs(cacheStatistics.hitRatio + dHitRate) > dTolerance))
	{
		cacheStatistics.hitRatio = dHitRate;
	}
	
	printf("\n--------------------Cache Statistics------------\n");
	printf("Total Accesses: %d\tHits: %d\tMisses: %d\n", 
		cacheStatistics.numAccesses, 
		cacheStatistics.numMisses,cacheStatistics.numHits);
	printf("Miss Rate: %2.4f\t Hit Rate: %2.4f\n",
		cacheStatistics.hitRatio ,dMissRate);
	printf("Reads:  %d\tWrites:   %d\n", cacheStatistics.numReads, cacheStatistics.numWrites);
}


/* ===============================================================================
printSettings: prints user settings for the cache including line size,
			   associativity, number of sets, number of lines and total capacity

@input: None

@output: None

================================================================================== */
void printSettings()
{
	printf("\n--------------------Cache Settings------------\n");
	printf("Capacity: %d\tLine Size: %d\tNumber of Lines: %d\n",
		cacheStatistics.cacheSize,
		cacheStatistics.lineSize,cacheStatistics.numLines);
	printf("Associativity: %d\tNumber of Sets: %d\n",
		cacheStatistics.associativity, cacheStatistics.numSets);
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
