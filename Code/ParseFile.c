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
#include "writeBuffer.h"

/* ==================================================================================
Function name:	ParseFile
Arguments:		char * Filename
Returns:		0 for valid return, -1 for error
Description:	This parses the trace file and passes the command and address
				to the main part of the program.
================================================================================== */
int ParseFile(char * Filename)
{
	FILE * traceFile;				// file descriptor for trace file
	unsigned int characterInLine;   // Variable to hold character in line
	unsigned int operation;
	char lineHolder[SIZE];
	int counter = 0;
	int isSpace = 0;
	int victimCounter = 0;
	int victimReturn = 0;

	traceFile = fopen(Filename, "r");	//Open the file in read mode
	if (traceFile == NULL)
	{
		fprintf(stderr, "fopen failed\n");	//If it failed
		exit(1);
	}

	while (1)
	{
		characterInLine = fgetc(traceFile);	//Get the first character in the file
		if (feof(traceFile))	//If the end of file
		{
			lineHolder[counter] = '\0';
			if ((isSpace == 0) && (counter > 0))	//If the counter is greater than zero, but there has not been a space, then at the end of the line at the end of the file.
			{
				writeBuffer(0, CLEAR, 0);	//Clear the buffer by writing out to the memory
				break;
			}
			else
			{
				int returnValue = ProgramWrapper(lineHolder, operation);	//Run the last line through the program
				if (returnValue < 0)
					return -1;
			}
			writeBuffer(0, CLEAR, 0);	//Clear the buffer by writing out to the memory
			break;
		}
		if (characterInLine == '\n')		//If new line character
		{
			++victimCounter;				//counter for the buffer as lines are read
			if (victimCounter > 3)
			{
				victimReturn = writeBuffer(0, 0, victimCounter);  //Write to memory the next address in the write buffer
				if (victimReturn == -1)
					return -1;
				victimCounter = 0;
			}
			lineHolder[counter] = '\0';
			int returnValue = ProgramWrapper(lineHolder, operation);  //Run the program on the line that was created from the file
			if (returnValue < 0)
				return -1;
			isSpace = 0;					//Reset is space variable.
			continue;
		}
		if (isspace(characterInLine))		//If the character is a space
		{
			isSpace = 1;
			counter = 0;					//Reset the counter so the lineHolder can start new
			continue;
		}
		if (!isSpace)
			operation = characterInLine - '0';	//If not a space, get the operation from the line
		else
			lineHolder[counter] = characterInLine;
		++counter;
	}
	fclose(traceFile);							//Close the trace file
	return 0;
}

/* ==================================================================================
Function name:	ProgramWrapper
Arguments:		char * HexAddress
				unsigned int operation
Returns:		0 for valid return, -1 for error
Description:	This is the wrapper function that runs the cache program.
================================================================================== */
int ProgramWrapper(char * HexAddress, unsigned int operation)
{
	unsigned int tag = 0;
	unsigned int index = 0;
	unsigned int convertedHex = 0;

	ParseHexAddress(HexAddress, &tag, &index, &convertedHex);	//Run the parser on the address
	
	if (Silent)
	{
		printf("\n\n\n------------------------------------------Trace File Line-----\n");
		printf("Command: %d  Address: %#x", operation, convertedHex);
		printf("\n--------------------------------------------------------------\n\n");
	}
	return CommandCentral(tag, index, operation, convertedHex);	//Return the value that comes from the command function, check to see if it is -1 to exit the program
}

/* ==================================================================================
Function name:	CommandCentral
Arguments:		unsigned int tag
				unsigned int index
				unsigned int operation
				unsigned int HexAddress
Returns:		0 for valid return, -1 for error
Description:	Runs the commands depending on the command passed in from the wrapper function.
================================================================================== */
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
		return ExecuteCommand3(index, tag, HexAddress);
		break;
	case SNOOPED_READ:
		return ExecuteCommand4(index, tag, HexAddress);
		break;
	case SNOOPED_WRITE:
		return ExecuteCommand5(index, tag, HexAddress);
		break;
	case SNOOPED_RWIM:
		return ExecuteCommand6(index, tag, HexAddress);
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

/* ==================================================================================
Function name:	ParseHexAddress
Arguments:		unsigned int * tag
				unsigned int * index
				char * HexAddress
				unsigned int * convertedHex
Returns:		0 for valid return, -1 for error
Description:	Runs the commands depending on the command passed in from the wrapper function.
================================================================================== */
void ParseHexAddress(char * HexAddress, unsigned int * tag, unsigned int * index, unsigned int * convertedHex)
{
	unsigned int addressBaseTen = 0;

	sscanf(HexAddress, "%x", &addressBaseTen);	//Convert the hex array to an unsigned integer
	*convertedHex = addressBaseTen;
	ParseAddress(&addressBaseTen, index, tag);	//Parse the address to get the tag and index
}
