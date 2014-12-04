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