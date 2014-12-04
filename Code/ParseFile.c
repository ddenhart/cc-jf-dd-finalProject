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

#include <stdio.h>
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
			ProgramWrapper(lineHolder, operation);
			break;
		}
		if (characterInLine == '\n')
		{
			lineHolder[counter] = '\0';
			ProgramWrapper(lineHolder, operation);
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
	// Close file
	// ----------
	fclose(traceFile);
	return;
}

void ProgramWrapper(char * HexAddress, unsigned int operation)
{
	unsigned int tag = 0;
	unsigned int index = 0;

	ParseHexAddress(HexAddress, &tag, &index);
	CommandCentral(tag, index, operation, HexAddress);
}

void CommandCentral(unsigned int tag, unsigned int index, unsigned int operation, char * HexAddress)
{
	switch (operation)
	{
	case INSTR_READ_REQ:
	case DATA_READ_REQ:
		ExecuteCommands02(index, tag, HexAddress);
		break;
	case DATA_WRITE_REQ:
		ExecuteCommand1(index, tag, HexAddress);
		break;
	case SNOOPED_INVALIDATE:
		ExecuteCommand3(index, tag);
		break;
	case SNOOPED_READ:
		ExecuteCommand4(index, tag);
		break;
	case SNOOPED_WRITE:
		ExecuteCommand5(index, tag);
		break;
	case SNOOPED_RWIM:
		ExecuteCommand6(index, tag);
		break;
	case CLEAR_CACHE:
		ExecuteCommand8();
		break;
	case PRINT_VALID_LINES:
		ExecuteCommand9();
		break;
	default:
		break;
	}
}
void ParseHexAddress(char * HexAddress, unsigned int * tag, unsigned int * index)
{
	unsigned int addressBaseTen = 0;

	_set_printf_count_output(1);
	sscanf(HexAddress, "%x", &addressBaseTen);
	ParseAddress(&addressBaseTen, index, tag);
	return;
}
