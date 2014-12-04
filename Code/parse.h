/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team:
Carmen Ciobanu
Deborah Denhart
Jeremiah Franke

==================================================================================

File: 			parse.h
Date:			11/16/2014
Author:			Jeremiah Franke
Modified:		Jeremiah Franke 12/04/2014
Description: 	This file contains:
-

================================================================================== */

#define SIZE 100  //Size of the HEX array

void ParseHexAddress(char * HexAddress, unsigned int * tag, unsigned int * index);
void ProgramWrapper(char * HexAddress);
void ParseFile(char * Filename);
void CommandCentral(unsigned int tag, unsigned int index, unsigned int operation, char * HexAddress);