/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
File: 			PseudoLRU.h
Date:			11/26/2014
Author:			Carmen Ciobanu
Description: 	.h file for pseudo LRU
================================================================================== */


// #includes
// ---------
#include <stdint.h>				// for special data types like uint8_t, uint16_t
#include <stdio.h>				// I/O library
#include <ctype.h>				// useful for parsing
#include <errno.h>				// error macros
#include <math.h>				// basic match library
#include <stdlib.h>				// general utilities
#include <string.h>				// string handling


// #defines
// --------
#define CACHE_HIT			1
#define CACHE_MISS			0
#define DEBUG				1


// Function Prototypes
// -------------------
int GetMidpoint(int min, unsigned int max);
int SetPseudoLRU(unsigned int set, unsigned int line, int min, int max);
int GetVictimLine(unsigned int set, int min, int max, int * bitToRead, int * bitValue);
int UpdateLRU(unsigned int set, unsigned int line, int min, int max, int flag);







