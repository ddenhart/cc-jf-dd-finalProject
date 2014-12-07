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
Modified		Jeremiah Franke 12/04/2014
Description: 	This file contains:
-The functions for parsing the trace file.  This includes conversion from hex to 
integer and returning the appropriate values to other functions.
================================================================================== */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "CacheSimulator.h"
#include "victimCache.h"

//TODO JF:  Contact faust about if we are going to need to check the file.
int ParseFile(char * Filename)
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
	int victimCounter = 0;
	int victimReturn = 0;

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
			ProgramWrapper(lineHolder, operation);
			break;
		}
		if (characterInLine == '\n')
		{
			++victimCounter;
			if (victimCounter > 2)
			{
				victimReturn = victimCache(0, 0, victimCounter);
				if (victimReturn == -1)
					return -1;
				victimCounter = 0;
			}
			lineHolder[counter] = '\0';
			int returnValue = ProgramWrapper(lineHolder, operation);
			if (returnValue < 0)
				return -1;
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
			operation = characterInLine - '0';
		else
			lineHolder[counter] = characterInLine;
		++counter;
	}
	fclose(traceFile);
	return 0;
}

int ProgramWrapper(char * HexAddress, unsigned int operation)
{
	unsigned int tag = 0;
	unsigned int index = 0;
	unsigned int convertedHex = 0;

	ParseHexAddress(HexAddress, &tag, &index, &convertedHex);
	return CommandCentral(tag, index, operation, convertedHex);
}

int CommandCentral(unsigned int tag, unsigned int index, unsigned int operation, unsigned int HexAddress)
{
	switch (operation)
	{
	case INSTR_READ_REQ:
	case DATA_READ_REQ:
		return ExecuteCommands02(index, tag, HexAddress);
		break;
	case DATA_WRITE_REQ:
		return ExecuteCommand1(index, tag, HexAddress);
		break;
	case SNOOPED_INVALIDATE:
		return ExecuteCommand3(index, tag);
		break;
	case SNOOPED_READ:
		return ExecuteCommand4(index, tag);
		break;
	case SNOOPED_WRITE:
		return ExecuteCommand5(index, tag);
		break;
	case SNOOPED_RWIM:
		return ExecuteCommand6(index, tag);
		break;
	case CLEAR_CACHE:
		return ExecuteCommand8();
		break;
	case PRINT_VALID_LINES:
		return ExecuteCommand9();
		break;
	default:
		return 0;
		break;
	}
}

void ParseHexAddress(char * HexAddress, unsigned int * tag, unsigned int * index, unsigned int * convertedHex)
{
	unsigned int addressBaseTen = 0;

	_set_printf_count_output(1);
	sscanf(HexAddress, "%x", &addressBaseTen);
	*convertedHex = addressBaseTen;
	ParseAddress(&addressBaseTen, index, tag);
}
