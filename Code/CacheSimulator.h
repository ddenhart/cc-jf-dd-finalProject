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

// # includes
// ----------
#include <stdint.h>				// for special data types like uint8_t, uint16_t 
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling
// add more as needed


// #defines
// ---------

// Bus operation types
#define READ			1		// Bus read
#define WRITE			2		// Bus forward (also write-back???)
#define INVALIDATE		3		// Bus invalidate
#define RWIM			4		// Bus read with intend to modify

// Snoop result types
#define NOHIT			0		// No hit
#define HIT				1		// Hit
#define HITM			2		// Hit to a modified line


// MESIF states
#define MODIFIED		1		//
#define EXCLUSIVE		2		//	
#define SHARED			3		//
#define INVALID			4		//
#define FORWARD			5		//


// Function Prototypes
// -------------------
void BusOperation(char BusOp, unsigned int Address, char * SnoopResult);
char GetSnoopResult(unsigned int Address);
void PutSnoopResult(unsigned int Address, char Snoop Result);
void MessageToL2Cache(char BusOp, unsigned int Address);
void SetPseudoLRU();
void GetPseudoLRU();

void OutputValidLines();
unsigned int getMESIF(unsigned int set, unsigned int line);
void ReadMemory();
void WriteMemory();




