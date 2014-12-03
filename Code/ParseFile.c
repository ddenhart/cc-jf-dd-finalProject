/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team:
Carmen Ciobanu
Deborah Denhart
Jeremiah Franke

==================================================================================

File: 			ParseFile.c
Date:			11/16/2014
Author:			Jeremiah Franke
Description: 	This file contains:
-The functions for parsing the trace file.  This includes conversion from hex to 
integer and returning the appropriate values to other functions.
================================================================================== */

#include "parse.h"
#include "CacheSimulator.h"

void ParseFile(char * Filename)
{
	/* While not EOF
	-----------------
	Read line + error check
	Parse line + eror check (See parsing algorithm)
	Save command number in command variable
	Save address index in set index variable
	Save address tag in line tag variable
	Examine command and choose command algorithm (See all commands algorithms)
	Execute all steps in the appropriate command algorithm
	*/
	FILE * traceFile;				// file descriptor for trace file
	unsigned int characterInLine;   // Variable to hold character in line
	unsigned int operation;
	char lineHolder[SIZE];
	int counter = 0;
	int isSpace = 0;

	// Open file and do error check
	// ----------------------------
	traceFile = fopen(Filename, "r");
	if (traceFile == NULL)
	{
		fprintf(stderr, "fopen failed\n");
		exit(1);
	}

	while (1)
	{
		characterInLine = fgetc(traceFile);
		if (feof(traceFile))
		{
			lineHolder[counter] = '\0';
			ParseHexAddress(lineHolder);
			break;
		}
		if (characterInLine == '\n')
		{
			lineHolder[counter] = '\0';
			ParseHexAddress(lineHolder);
			isSpace = 0;
			continue;
		}
		if (isspace(characterInLine))
		{
			isSpace = 1;
			counter = 0;
			continue;
		}
		if (!isSpace)
			operation = characterInLine;
		else
			lineHolder[counter] = characterInLine;
		++counter;
	}
	// Close file
	// ----------
	printf("tester");
	fclose(traceFile);
	return;
}

void ParseHexAddress(char * HexAddress)
{
	unsigned int mini = 0;
	unsigned int tag = 0;
	unsigned int index = 0;
	_set_printf_count_output(1);
	sscanf(HexAddress, "%x", &mini);
	printf("%d\n", mini);
	ParseAddress(&mini, &index, &tag);
	printf("\n%d, %d", index, tag);
	printf("\n%x, %x\n", index, tag);
	//e = (int)strtol(HexAddress, NULL, 16);
	//sprintf(holder, "%x", mini);
	//printf("%s\n", HexAddress);
	return;
}
