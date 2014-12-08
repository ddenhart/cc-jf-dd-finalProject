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
Description: 	This file contains:
				- 
	
================================================================================== */


void OutputStatistics();
void OutputValidLines();
// Used to simulate a bus operation and to capture the 
// snoop results of last level caches of other processors
void BusOperation(int BusOp, unsigned int Address, unsigned int  *SnoopResult);
unsigned int GetSnoopResult(unsigned int Address);
void PutSnoopResult(unsigned int Address, unsigned int SnoopResult);
void snoopResultToString(unsigned int SnoopResult, int *post, char* strres);
