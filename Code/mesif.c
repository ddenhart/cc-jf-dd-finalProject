/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			mesif.c
Date:			11/16/2014
Author:			Deborah Denhart
Modified:		Deborah Denhart		12/02/14
				Deborah Denhart		12/03/14
Description: 	This file contains:
				- 
	
================================================================================== */

#include <stdio.h>
#include <math.h>
#include "mesif.h"
#include "CacheSimulator.h"


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
void UpdateMesif(unsigned int cmd, unsigned int address, unsigned int set, unsigned int line, int found)
{
    enum mesif_err eError = eNO_ERROR;
    enum Mesif_states eNext = eMAX_STATES;
    enum Mesif_states eCurrent = eMAX_STATES;

    sMesifBits.address = address;
    sMesifBits.cmd = cmd;
    sMesifBits.set = set;
    sMesifBits.line = line;

    eCurrent = GetCurrentState();

    eError = CommandMux(eCurrent); //event 1

    if(eError == eNO_ERROR)
    {
        eError = GetHitMiss(found, eCurrent); //event 2
    }

    if(eError == eNO_ERROR)
    {
        eError = eventCodeCheck(eCurrent, &eNext);
    }

    if(eError == eNO_ERROR)
    {
        printState(eCurrent, eNext);
        eError = SetMesifState(eNext);
    }

    if(eError != eNO_ERROR)
    {
        PrintError(eError, eCurrent);
    }

    cleanMesif();

}


/* ===============================================================================


================================================================================== */
void InitMesif()
{
    InitError();
    cleanMesif(&sMesifBits);

    fromM.toM = 0;
    fromM.toE = 0;
    fromM.toS = 0;
    fromM.toI = 0;
    fromM.toF = 0;

    fromE.toM = 0;
    fromE.toE = 0;
    fromE.toS = 0;
    fromE.toI = 0;
    fromE.toF = 0;

    fromS.toM = 0;
    fromS.toE = 0;
    fromS.toS = 0;
    fromS.toI = 0;
    fromS.toF = 0;

    fromI.toM = 0;
    fromI.toE = 0;
    fromI.toS = 0;
    fromI.toI = 0;
    fromI.toF = 0;

    fromF.toM = 0;
    fromF.toE = 0;
    fromF.toS = 0;
    fromF.toI = 0;
    fromF.toF = 0;
}


/* ===============================================================================


================================================================================== */
void cleanMesif()
{
    sMesifBits.address = 0; 
    sMesifBits.cmd = 0;
    sMesifBits.set = 0;
    sMesifBits.line = 0;
    sMesifBits.eFlag; 
    sMesifBits.iEventCode[0] = 0;
    sMesifBits.iEventCode[1] = 0;
    sMesifBits.iEventCode[2] = 0;
    sMesifBits.sCol_done[0] = FALSE;
    sMesifBits.sCol_done[1] = FALSE;
    sMesifBits.sCol_done[2] = FALSE;
}


/* ===============================================================================


================================================================================== */
void InitError()
{
    errorCount = 0;

    sErrorTable.errCode[0] = eNO_ERROR;
    sErrorTable.errCode[1] = eINVALID_STATE_ERROR;
    sErrorTable.errCode[2] = eFLAG_ERROR;
    sErrorTable.errCode[3] = eINVALID_COLUMN_ERROR;
    sErrorTable.errCode[4] = eCBUS_ERROR;
    sErrorTable.errCode[5] = eSBUS_ERROR;
    sErrorTable.errCode[6] = eSYNTAX_NULL_ERROR;
    sErrorTable.errCode[7] = eINVALID_COMMAND;
    sErrorTable.errCode[8] = eINVALID_SNOOP;
    sErrorTable.errCode[9] = eINVALID_BUS_OP;

    sErrorTable.errMsg[0] = errRow0;
    sErrorTable.errMsg[1] = errRow1;
    sErrorTable.errMsg[2] = errRow2;
    sErrorTable.errMsg[3] = errRow3;
    sErrorTable.errMsg[4] = errRow4;
    sErrorTable.errMsg[5] = errRow5;
    sErrorTable.errMsg[6] = errRow6;
    sErrorTable.errMsg[7] = errRow7;
    sErrorTable.errMsg[8] = errRow7;
    sErrorTable.errMsg[9] = errRow9;
}


/* ===============================================================================
BusOperation:	Used to simulate a bus operation and to capture the 
				snoop results of last level caches of other processors

@input:			enum cpu_bus BusOp 
				unsigned int Address 
				enum system_bus *SnoopResult

@output:		None

================================================================================== */
void BusOperation(int BusOp, unsigned int Address, unsigned int  *SnoopResult)
{
    enum mesif_err eError = eNO_ERROR;
    enum Mesif_states eNext = eMAX_STATES;
    enum Mesif_states eCurrent = eMAX_STATES;

    sMesifBits.address = Address;

    eCurrent = GetCurrentState();
        
    switch(BusOp)
    {
        case READ:
            sMesifBits.cmd = SNOOPED_READ;
            break;
        case WRITE:
            sMesifBits.cmd = SNOOPED_WRITE;
            break;
        case INVALIDATE:
            sMesifBits.cmd = SNOOPED_INVALIDATE;
            break;
        case RWIM:
            sMesifBits.cmd = SNOOPED_RWIM;
            break;
        default:
            eError = eINVALID_BUS_OP;
            PrintError(eError, eCurrent);
    }

    eError = CommandMux(eCurrent); //event 1

    if((eError == eNO_ERROR) && (sMesifBits.cmd != SNOOPED_WRITE))
    {
        *SnoopResult = GetSnoopResult(Address, eCurrent); //event 2
    }

    if(eError == eNO_ERROR)
    {
        eError = eventCodeCheck(eCurrent, &eNext);
    }

    if(eError == eNO_ERROR)
    {
        printState(eCurrent, eNext);
        eError = SetMesifState(eNext);
    }

    if(eError != eNO_ERROR)
    {
        PrintError(eError, eCurrent);
    }

    PutSnoopResult(Address, *SnoopResult);

    cleanMesif();
}


/* ===============================================================================
GetSnoopResult: Used to simulate the reporting of snoop results by other caches

@input:			unsigned int Address

@output:		enum system_bus

================================================================================== */
unsigned int GetSnoopResult(unsigned int Address, enum Mesif_states eCurrent)
{
    enum mesif_err eError = eNO_ERROR;

    unsigned int firstHalf = Address & 0xFFFF0000;
    unsigned int secondHalf = Address & 0x0000FFFF;
    unsigned int modRes = 0;
    unsigned int snoopResult = 0;
    sMesifBits.sCol_done[eCOL_2] = TRUE;

    firstHalf = firstHalf >> 16;
    modRes = firstHalf ^ secondHalf;
    snoopResult = modRes%3;


    if(snoopResult == NOHIT)
    {
        switch(eCurrent)
        {
            case eMODIFIED:
                //eError = action_DoNothing(sMesifBits.eFlag);
                eError = actionM_HITM(sMesifBits.eFlag);
                break;
            case eEXCLUSIVE:
                //eError = action_DoNothing(sMesifBits.eFlag);
                eError = actionE_HIT(sMesifBits.eFlag);
                break;
            case eSHARED:
                eError = action_DoNothing(sMesifBits.eFlag);
                break;
            case eINVALID:
                eError = action_DoNothing(sMesifBits.eFlag);
                break;
            case eFORWARD:
                //eError = action_DoNothing(sMesifBits.eFlag);
                eError = actionF_HIT(sMesifBits.eFlag);
                break;
            default:
                eError = eINVALID_STATE_ERROR;
        }
    }
    else if(snoopResult == HIT)
    {
        switch(eCurrent)
        {
            case eMODIFIED:
                //eError = actionM_HIT(sMesifBits.eFlag);
                eError = actionM_HITM(sMesifBits.eFlag);
                break;
            case eEXCLUSIVE:
                eError = actionE_HIT(sMesifBits.eFlag);
                break;
            case eSHARED:
                eError = actionS_HIT(sMesifBits.eFlag);
                break;
            case eINVALID:
                eError = actionI_HIT(sMesifBits.eFlag);
                break;
            case eFORWARD:
                eError = actionF_HIT(sMesifBits.eFlag);
                break;
            default:
                eError = eINVALID_STATE_ERROR;
        }
    }
    else if(snoopResult == HITM)
    {
        switch(eCurrent)
        {
            case eMODIFIED:
                eError = actionM_HITM(sMesifBits.eFlag);
                break;
            case eEXCLUSIVE:
                //eError = actionE_HITM(sMesifBits.eFlag);
                eError = actionE_HIT(sMesifBits.eFlag);
                break;
            case eSHARED:
                eError = actionS_HITM(sMesifBits.eFlag);
                break;
            case eINVALID:
                eError = actionI_HITM(sMesifBits.eFlag);
                break;
            case eFORWARD:
                //eError = actionF_HITM(sMesifBits.eFlag);
                eError = actionF_HIT(sMesifBits.eFlag);
                break;
            default:
                eError = eINVALID_STATE_ERROR;
        }
    }
    else
    {
        eError = eINVALID_SNOOP;
    }

    if(eError != eNO_ERROR)
    {
        PrintError(eError, eCurrent);
    }

    return snoopResult;
}


/* ===============================================================================
PutSnoopResult:	Used to report the result of our snooping bus 
				operations by other caches 

@input:			unsigned int Address
				enum system_bus SnoopResult

@output:		None

================================================================================== */
void PutSnoopResult(unsigned int Address, unsigned int SnoopResult)
{
#if SILENT 
   printf("\n------------------------------------------Snoop Result--------\n");
   printf("SnoopResult: Address %#x, SnoopResult: %d", Address, SnoopResult);
   printf("\n--------------------------------------------------------------\n\n");
#endif 
}


/* ==================================================================================
	Function name:	GetCurrentState
 	Arguments:		unsigned int set = number of the set containing the desired line
 	 	 	 	 	unsigned int line = number of the line we want to get the MESIF bits for
	Returns:		void
	Description:	Read the line MESIF state
   ================================================================================== */
enum Mesif_states GetCurrentState()
{
    return cachePtr[sMesifBits.set].setPtr[sMesifBits.line].mesifBits;
}


unsigned int GetMesifState(unsigned int set, unsigned int line)
{
    return cachePtr[set].setPtr[line].mesifBits;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum Mesif_states GetHitMiss(int found, enum Mesif_states eCurrent)
{
    enum mesif_err eError = eNO_ERROR;

    sMesifBits.sCol_done[1] = TRUE;

    if(found == CACHE_MISS)
    {
        switch(eCurrent)
        {
            case eMODIFIED:
                eError = actionM_NOHIT(sMesifBits.eFlag);
                break;
            case eEXCLUSIVE:
                eError = actionE_NOHIT(sMesifBits.eFlag);
                break;
            case eSHARED:
                eError = actionS_NOHIT(sMesifBits.eFlag);
                break;
            case eINVALID:
                eError = actionI_NOHIT(sMesifBits.eFlag);
                break;
            case eFORWARD:
                eError = actionF_NOHIT(sMesifBits.eFlag);
                break;
            default:
                eError = eINVALID_STATE_ERROR;
        }
    }
    else if(found == CACHE_HIT)
    {
        switch(eCurrent)
        {
            case eMODIFIED:
                eError = actionM_HIT(sMesifBits.eFlag);
                break;
            case eEXCLUSIVE:
                eError = actionE_HIT(sMesifBits.eFlag);
                break;
            case eSHARED:
                eError = actionS_HIT(sMesifBits.eFlag);
                break;
            case eINVALID:
                eError = actionI_HIT(sMesifBits.eFlag);
                break;
            case eFORWARD:
                eError = actionF_HIT(sMesifBits.eFlag);
                break;
            default:
                eError = eINVALID_STATE_ERROR;
        }
    }
    else
    {
        eError = eCBUS_ERROR;
    }

    return eError;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum mesif_err SetMesifState(enum Mesif_states eState)
{
    enum mesif_err eError = eNO_ERROR;

    if((cachePtr) && (cachePtr[sMesifBits.set].setPtr))
    {
        cachePtr[sMesifBits.set].setPtr[sMesifBits.line].mesifBits = eState;
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }
    
    return eError;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum mesif_err StateSelect(int iStateRow, enum Mesif_states eCurrent, enum Mesif_states *eNext)
{
    enum mesif_err eError = eNO_ERROR;

    switch(eCurrent)
    {
        case eINVALID:
            if(sMesifBits.eFlag == eCBUS)
            {
                if(eCROW_I_M_XF_M_XS_M_XF_M == iStateRow)
                {
                    *eNext = eMODIFIED;
                    ++fromI.toM;
                }
                else if(eCROW_I_E == iStateRow)
                {
                    *eNext = eEXCLUSIVE;
                    ++fromI.toE;
                }
                else if(eCROW_I_F == iStateRow)
                {
                    *eNext = eFORWARD;
                    ++fromI.toF;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(sMesifBits.eFlag ==eSBUS)
            {
                *eNext = eINVALID;
                ++fromI.toI;
            }
            else
            {
                eError = eFLAG_ERROR;
            }
            break;
        case eMODIFIED:
            if(sMesifBits.eFlag == eCBUS)
            {
                *eNext = eMODIFIED;
                ++fromM.toM;
            }
            else if(sMesifBits.eFlag == eSBUS)
            {
                if(eSROW_M_I == iStateRow)
                {
                    *eNext = eINVALID;
                    ++fromM.toI;
                }
                else if(eSROW_M_S == iStateRow)
                { 
                    *eNext = eSHARED;
                    ++fromM.toS;
                }
                else if(eSROW_I_I == iStateRow)
                {
                    *eNext = eMODIFIED;
                    ++fromM.toM;
                }
                else
                {
                    eError = eSBUS_ERROR;
                }
            }
            else
            {
                eError = eFLAG_ERROR;
            }
            break;
        case eEXCLUSIVE:
            if(sMesifBits.eFlag == eCBUS)
            {
                if(eCROW_M_M_XF_F_XS_S_XE_E == iStateRow)
                {
                    *eNext = eEXCLUSIVE;
                    ++fromE.toE;
                }
                else if(eCROW_M_M_XE_M == iStateRow)
                {
                    *eNext = eMODIFIED;
                    ++fromE.toM;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(sMesifBits.eFlag ==eSBUS)
            {
                if(eSROW_F_I_XE_I == iStateRow)
                {
                    *eNext = eINVALID;
                    ++fromE.toI;
                }
                else if(eSROW_F_S_XE_S == iStateRow)
                {
                    *eNext = eSHARED;
                    ++fromE.toS;
                }
                else if(eSROW_I_I == iStateRow)
                {
                    *eNext = eEXCLUSIVE;
                    ++fromE.toE;
                }
                else
                {
                    eError = eSBUS_ERROR;
                }
            }
            else
            {
                eError = eFLAG_ERROR;
            }
            break;
        case eSHARED:
            if(sMesifBits.eFlag == eCBUS)
            {
                if(eCROW_I_M_XF_M_XS_M_XF_M == iStateRow)
                {
                    *eNext = eMODIFIED;
                    ++fromS.toM;
                }
                else if(eCROW_M_M_XF_F_XS_S_XE_E == iStateRow)
                {
                    *eNext = eSHARED;
                    ++fromS.toS;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(sMesifBits.eFlag == eSBUS)
            {
                if(eSROW_S_I == iStateRow)
                {
                    *eNext = eINVALID;
                    ++fromS.toI;
                }
                else if(eSROW_S_S == iStateRow)
                {
                    *eNext = eSHARED;
                    ++fromS.toS;
                }
                else if(eSROW_I_I == iStateRow)
                {
                    *eNext = eSHARED;
                    ++fromS.toS;
                }
                else
                {
                    eError = eSBUS_ERROR;
                }
            }
            else
            {
                eError = eFLAG_ERROR;
            }
            break;
        case eFORWARD:
            if(sMesifBits.eFlag == eCBUS)
            {
                if(eCROW_M_M_XF_F_XS_S_XE_E == iStateRow)
                {
                    *eNext = eFORWARD;
                    ++fromF.toF;
                }
                else if(eCROW_F_M_XS_M == iStateRow)
                {
                    *eNext = eMODIFIED;
                    ++fromF.toM;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(sMesifBits.eFlag ==eSBUS)
            {
                if(eSROW_F_I_XE_I == iStateRow)
                {
                    *eNext = eINVALID;
                    ++fromF.toI;
                }
                else if(eSROW_F_S_XE_S == iStateRow)
                {
                    *eNext = eSHARED;
                    ++fromF.toS;
                }
                else if(eSROW_I_I == iStateRow)
                {
                    *eNext = eFORWARD;
                    ++fromF.toF;
                }
                else
                {
                    eError = eSBUS_ERROR;
                }
            }
            else
            {
                eError = eFLAG_ERROR;
            }
            break;
        default:
            eError = eINVALID_COLUMN_ERROR;
    }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err eventCodeCheck(enum Mesif_states eCurrent, enum Mesif_states *eNext)
{
    enum mesif_err eError = eNO_ERROR;
    int iRow = 0;

    iRow = compareCodes();

    if(iRow >= 0)
    {
        //use row and flag to get the next state
        eError = StateSelect(iRow, eCurrent, eNext);
    }
    else
    {
        eError = eINVALID_COLUMN_ERROR;
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
int compareCodes()
{
    int col, row;
    int bSame = FALSE;

    if(sMesifBits.iEventCode)
    {
        if(sMesifBits.eFlag == eCBUS)
        {
            for(row = 0; row < CPU_CODE_ROWS; ++row)
            {
                for(col = 0; col < MAX_STATE_COMBO; ++col)
                {
                    if(sMesifBits.iEventCode[col] == valid_CPU_Codes[row][col])
                    {
                        bSame = TRUE;
                    }
                    else if(valid_CPU_Codes[row][col] == eCB_DONTCARE)
                    {
                        bSame = TRUE;
                    }
                    else
                    {
                        bSame = FALSE;
                        break;
                    }
                }
                //at the end of the row, if bSame is true, the row is a match
                if(bSame)
                {
                    break;
                }
                //else keep searching
            }
        }
        else if(sMesifBits.eFlag == eSBUS)
        {
            for(row = 0; row < SYS_CODE_ROWS; ++row)
            {
                for(col = 0; col < MAX_STATE_COMBO; ++col)
                {
                    if(sMesifBits.iEventCode[col] == valid_SYS_Codes[row][col])
                    {
                        bSame = TRUE;
                    }
                    else if(valid_SYS_Codes[row][col] == eSB_DONTCARE)
                    {
                        bSame = TRUE;
                    }
                    else
                    {
                        bSame = FALSE;
                        break;
                    }
                }
                //at the end of the row, if bSame is true, the row is a match
                if(bSame)
                {
                    break;
                }
                //else keep searching
            }
        }
        else
        {
            bSame = FALSE;
        }
    }
    else
    {
        bSame = FALSE;
    }

    //if all done and bSame is false, there were no matches found
    //send back an error
    if(!bSame)
    {
        row = eINVALID_COLUMN_ERROR;
    }

    return row;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err UpdateEvents(int *iEventCode, enum eventColumn eCol)
{
    enum mesif_err eError = eNO_ERROR;
    
    if(iEventCode &&
       (eCol >= 0) && (eCol < MAX_STATE_COMBO))
    {
        sMesifBits.iEventCode[eCol] = iEventCode[eCol];
    }
    else
    {
        eError = eINVALID_COLUMN_ERROR;
    }

   return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err CommandMux(enum Mesif_states eCurrent)
{
    enum mesif_err eError = eNO_ERROR;

    switch(sMesifBits.cmd)
    {
        case DATA_READ_REQ:		// Command 0 = read request from L2 data cache
            sMesifBits.eFlag = eCBUS;
            sMesifBits.sCol_done[0] = TRUE;

            switch(eCurrent)
            {
                case eINVALID:
                    eError = actionI_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionI_Memread(sMesifBits.eFlag);
                    }
                    break;
                case eMODIFIED:
                    eError = actionM_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eEXCLUSIVE:
                    eError = actionE_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eSHARED:
                    eError = actionS_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eFORWARD:
                    eError = actionF_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                default:
                    eError = eINVALID_STATE_ERROR;
            }
            break;
        case DATA_WRITE_REQ:		// Command 1 = write request from L2 data cache
            sMesifBits.eFlag = eCBUS;
            sMesifBits.sCol_done[0] = TRUE;

            switch(eCurrent)
            {
                case eINVALID:
                    eError = actionI_Write(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionI_RFO(sMesifBits.eFlag);
                    }
                    break;
                case eMODIFIED:
                    eError = actionM_Write(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eEXCLUSIVE:
                    eError = actionE_Write(sMesifBits.eFlag);;
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eSHARED:
                    eError = actionS_Write(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionS_Invalidate(sMesifBits.eFlag);
                    }
                    break;
                case eFORWARD:
                    eError = actionF_Write(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionF_Invalidate(sMesifBits.eFlag);
                    }
                    break;
                default:
                    eError = eINVALID_STATE_ERROR;
            }
            break;
        case INSTR_READ_REQ:		// Command 2 = read request from L2 instruction cache
            //no write on an instruction cache
            sMesifBits.eFlag = eCBUS;
            sMesifBits.sCol_done[0] = TRUE;

            switch(eCurrent)
            {
                case eINVALID:
                    eError = actionI_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionI_Memread(sMesifBits.eFlag);
                    }
                    break;
                case eMODIFIED:
                    eError = actionM_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eEXCLUSIVE:
                    eError = actionE_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eSHARED:
                    eError = actionS_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eFORWARD:
                    eError = actionF_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                default:
                    eError = eINVALID_STATE_ERROR;
            }
            break;
        case SNOOPED_INVALIDATE:		// Command 3 = snooped invalidate command
            sMesifBits.eFlag = eSBUS;
            sMesifBits.sCol_done[0] = TRUE;

            switch(eCurrent)
            {
                case eINVALID:
                    eError = actionI_Invalidate(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eMODIFIED:
                    eError = actionM_Invalidate(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eEXCLUSIVE:
                    eError = actionE_Invalidate(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eSHARED:
                    eError = actionS_Invalidate(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eFORWARD:
                    eError = actionF_Invalidate(sMesifBits.eFlag);
                    eError = actionF_Forward(sMesifBits.eFlag);
                    sMesifBits.sCol_done[2] = TRUE;
                    break;
                default:
                    eError = eINVALID_STATE_ERROR;
            }
            break;
        case SNOOPED_READ:		// Command 4 = snooped read request
            sMesifBits.eFlag = eSBUS;
            sMesifBits.sCol_done[0] = TRUE;

            switch(eCurrent)
            {
                case eINVALID:
                    eError = actionI_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eMODIFIED:
                    eError = actionM_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionM_Writeback(sMesifBits.eFlag);
                    }
                    break;
                case eEXCLUSIVE:
                    eError = actionE_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionE_Forward(sMesifBits.eFlag);
                    }
                    break;
                case eSHARED:
                    eError = actionS_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eFORWARD:
                    eError = actionF_Read(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionE_Forward(sMesifBits.eFlag);
                    }
                    break;
                default:
                    eError = eINVALID_STATE_ERROR;
            }
            break;
        case SNOOPED_WRITE:		// Command 5 = snooped write request
            sMesifBits.eFlag = eSBUS;
            sMesifBits.sCol_done[0] = TRUE;
            int nothing[MAX_STATE_COMBO] = {eSB_DONTCARE, 0, 0};
            eError = UpdateEvents(nothing, eCOL_1); 
            if(eError == eNO_ERROR)
            {
                eError = action_DoNothing();
            }
            break;
        case SNOOPED_RWIM:		// Command 6 = snooped read with intent to modify
            sMesifBits.eFlag = eSBUS;
            sMesifBits.sCol_done[0] = TRUE;

            switch(eCurrent)
            {
                case eINVALID:
                    eError = actionI_RFO(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eMODIFIED:
                    eError = actionM_RFO(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionM_Writeback(sMesifBits.eFlag);
                    }
                    break;
                case eEXCLUSIVE:
                    eError = actionE_RFO(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionE_Forward(sMesifBits.eFlag);
                    }
                    break;
                case eSHARED:
                    eError = actionS_RFO(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        eError = action_DoNothing();
                    }
                    break;
                case eFORWARD:
                    eError = actionF_RFO(sMesifBits.eFlag);
                    if(eError == eNO_ERROR)
                    {
                        sMesifBits.sCol_done[2] = TRUE;
                        eError = actionF_Forward(sMesifBits.eFlag);
                    }
                    break;
                default:
                    eError = eINVALID_STATE_ERROR;
            }
            break;
        case CLEAR_CACHE:		// Command 8 = clear cache / reset all states
            cleanMesif();
            break;
        case PRINT_VALID_LINES:		// Command 9 = print valid lines' contents and state
            break;
        default:
            eError = eINVALID_COMMAND;
    }
    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
void PrintError(enum mesif_err eError, enum Mesif_states eCurrent)
{
    int index = abs(eError);
    char* strCurrent = GetStateName(eCurrent);
    char* code1 = GetCodeName(sMesifBits.iEventCode[0]);
    char* code2 = GetCodeName(sMesifBits.iEventCode[1]);
    char* code3 = GetCodeName(sMesifBits.iEventCode[2]);

    ++errorCount;

#if DEBUG 
    printf("\n------------------------------------------MESIF ERROR---------\n");

    if(sErrorTable.errMsg[index] && (index < MAX_ERRORS))
    {
        printf("MESIF Error %d: %s\n\n", sErrorTable.errCode[index], sErrorTable.errMsg[index]);
        //printf("\n------------------------------------------MESIF DUMP---------\n");
        //printf("MESIF Error UNKNOWN: Dump Mesif State\n");
        printf("Command: %d, Address: %#x, Set: %d, Line: %d, MESIF Flag: %d\n", 
               sMesifBits.cmd, sMesifBits.address, sMesifBits.set, 
               sMesifBits.line, sMesifBits.eFlag);
        printf("Current State: %s\n",strCurrent);
        printf("Event Code: %s/ %s/ %s, %d %d %d\n", code1, code2, code3,
               sMesifBits.iEventCode[0], sMesifBits.iEventCode[1], sMesifBits.iEventCode[2]);
        printf("\n--------------------------------------------------------------\n");
    }
#endif
}


void printState(enum Mesif_states eCurrent, enum Mesif_states eNext)
{
#if SILENT 
    char* strCurrent = GetStateName(eCurrent);
    char* strNext = GetStateName(eNext);
    char* code1 = GetCodeName(sMesifBits.iEventCode[0]);
    char* code2 = GetCodeName(sMesifBits.iEventCode[1]);
    char* code3 = GetCodeName(sMesifBits.iEventCode[2]);

    printf("\n------------------------------------------MESIF State---------\n");
    printf("Changing MESIF State from %s to %s for:\n", strCurrent, strNext);
    printf("Command: %d, Address: %#x, Set: %d, Line: %d\n",
           sMesifBits.cmd, sMesifBits.address, sMesifBits.set, sMesifBits.line);
    printf("Event Code: %s/ %s/ %s, ", code1, code2, code3);
    printf("\n--------------------------------------------------------------\n");
#endif

#if DEBUG
    printf("\n\n------------------------------------------Transition Count-------------\n");
    printf("ErrorCount: %d\n", errorCount);
    printf("M->M: %d  \tM->E: %d  \tM->S: %d  \tM->I: %d  \tM->F: %d\n",
           fromM.toM, fromM.toE, fromM.toS, fromM.toI, fromM.toF);
    printf("E->M: %d  \tE->E: %d  \tE->S: %d  \tE->I: %d  \tE->F: %d\n",
           fromE.toM, fromE.toE, fromE.toS, fromE.toI, fromE.toF);
    printf("S->M: %d  \tS->E: %d  \tS->S: %d  \tS->I: %d  \tS->F: %d\n",
           fromS.toM, fromS.toE, fromS.toS, fromS.toI, fromS.toF);
    printf("I->M: %d  \tI->E: %d  \tI->S: %d  \tI->I: %d  \tI->F: %d\n",
           fromI.toM, fromI.toE, fromI.toS, fromI.toI, fromI.toF);
    printf("F->M: %d  \tF->E: %d  \tF->S: %d  \tF->I: %d  \tF->F: %d\n",
           fromF.toM, fromF.toE, fromF.toS, fromF.toI, fromF.toF);
    printf("\n-----------------------------------------------------------------------\n");
#endif
}

char *GetStateName(enum Mesif_states eState)
{
    char* str;

    switch(eState)
    {
        case eMODIFIED:
            str = "Modified";
            break;
        case eEXCLUSIVE:
            str = "Exclusive";
            break;
        case eSHARED:
            str = "Shared";
            break;
        case eINVALID:
            str = "Invalid";
            break;
        case eFORWARD:
            str = "Forward";
            break;
        default:
            str = "Unknown State";
    }

    return str;
}


char *GetCodeName(int code)
{
    char* str;

    switch(code)
    {
        case eSB_RFO:
            str = "RFO";
            break;
        case eSB_READ:
            str = "Read";
            break;
        case eSB_HIT:
            str = "Hit";
            break;
        case eSB_HITM:
            str = "HitM";
            break;
        case eSB_WRITEBACK:
            str = "Writeback";
            break;
        case eSB_FORWARD:
            str = "Forward";
            break;
        case eSB_DONTCARE:
            str = "X";
            break;
        case eCB_READ:
            str = "Read";
            break;
        case eCB_WRITE:
            str = "Write";
            break;
        case eCB_NOHIT:
            str = "Hit!";
            break;
        case eCB_HIT:
            str = "Hit";
            break;
        case eCB_MEMREAD:
            str = "Memory Read";
            break;
        case eCB_RFO:
            str = "RFO";
            break;
        case eCB_INVALIDATE:
            str = "Invalidate";
            break;
        case eCB_DONTCARE:
            str = "X";
            break;
        default:
            str = "Unknown Code";
    }

    return str;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum mesif_err actionM_Read(enum mesif_type eFlag)
{
	enum mesif_err eError = eNO_ERROR;
   int iEventCode[] = {0, 0, 0};
	
    if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
          iEventCode[eCOL_1] = eSB_READ;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
	}

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Write(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_WRITE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_RFO(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCOL_3] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
          eCol = eCOL_1;
          iEventCode[eCOL_1] = eSB_RFO;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_NOHIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_HIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_2] = eCB_DONTCARE;
		}
      else if(eFlag == eSBUS)
      {
          iEventCode[eCOL_2] = eSB_HIT;
      }
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_HITM(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
          iEventCode[eCOL_2] = eSB_HITM;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Invalidate(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCol] = eCB_DONTCARE;
      }
      if(eFlag == eSBUS)
      {
          eCol = eCOL_1;
          iEventCode[eCol] = eSB_DONTCARE;
      }
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Memread(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eCBUS)
        {
            iEventCode[eCOL_3] = eCB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents(iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Writeback(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_WRITEBACK;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents(iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Read(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
          iEventCode[eCOL_1] = eSB_READ;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Write(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_WRITE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_RFO(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCOL_3] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
          eCol = eCOL_1;
          iEventCode[eCOL_1] = eSB_RFO;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_NOHIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_HIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_HIT;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_HITM(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Invalidate(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
      {
          eCol = eCOL_1;
          iEventCode[eCol] = eCB_DONTCARE;
      }
      else if(eFlag == eSBUS)
      {
          eCol = eCOL_1;
          iEventCode[eCol] = eSB_DONTCARE;
      }
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Forward(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_FORWARD;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents(iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Memread(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eCBUS)
        {
            iEventCode[eCOL_3] = eCB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents(iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Read(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
          iEventCode[eCOL_1] = eSB_READ;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Write(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eCBUS)
        {
            iEventCode[eCOL_1] = eCB_WRITE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents(iEventCode, eCOL_1);
    }
    return eError;
}

/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_RFO(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCOL_3] = eCB_RFO;
		}
		else if (eFlag == eSBUS)
		{
          eCol = eCOL_1;
          iEventCode[eCOL_1] = eSB_RFO;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_NOHIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_HIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_HITM(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents(iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Invalidate(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCol] = eCB_INVALIDATE;
      }
      else if(eFlag == eSBUS)
      {
          eCol = eCOL_1;
          iEventCode[eCol] = eSB_DONTCARE;
      }
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Forward(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Memread(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eCBUS)
        {
            iEventCode[eCOL_3] = eCB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Writeback(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Read(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_READ;
		}
		else if(eFlag == eSBUS)
		{
          iEventCode[eCOL_1] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Write(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_WRITE;
		}
		else if(eFlag == eSBUS)
		{
          iEventCode[eCOL_1] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_RFO(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCOL_3] = eCB_RFO;
		}
		else if(eFlag == eSBUS)
		{
          eCol = eCOL_1;
          iEventCode[eCOL_1] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_NOHIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_NOHIT;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_HIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_HIT;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_HITM(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if(iEventCode)
	{
		if(eFlag == eSBUS)
		{
			iEventCode[eCOL_2] = eSB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Memread(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_MEMREAD;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_3);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Invalidate(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_1] = eSB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents(iEventCode, eCOL_1);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Forward(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Writeback(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}



/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Read(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
          iEventCode[eCOL_1] = eSB_READ;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_1);
   }

	return eError;
}



/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Write(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_1] = eCB_WRITE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_1);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_RFO(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCOL_3] = eCB_RFO;
		}
		else if (eFlag == eSBUS)
		{
          eCol = eCOL_1;
          iEventCode[eCOL_1] = eSB_RFO;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_NOHIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_HIT(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
          iEventCode[eCOL_2] = eSB_HIT;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_HITM(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_2] = eCB_DONTCARE;
		}
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCOL_2);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Invalidate(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};
    enum eventColumn eCol;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          eCol = eCOL_3;
          iEventCode[eCol] = eCB_INVALIDATE;
      }
      else if(eFlag == eSBUS)
      {
          eCol = eCOL_1;
          iEventCode[eCol] = eSB_DONTCARE;
      }
		else
		{
			eError = eFLAG_ERROR;
		}
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
   }

   if(eError == eNO_ERROR)
   {
       UpdateEvents( iEventCode, eCol);
   }

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Forward(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eSBUS)
        {
            iEventCode[eCOL_3] = eSB_FORWARD;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Memread(enum mesif_type eFlag)
{
    enum mesif_err eError = eNO_ERROR;
    int iEventCode[] = {0, 0, 0};

    if(iEventCode)
    {
        if(eFlag == eCBUS)
        {
            iEventCode[eCOL_3] = eCB_DONTCARE;
        }
        else
        {
            eError = eFLAG_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;
    }

    if(eError == eNO_ERROR)
    {
        UpdateEvents( iEventCode, eCOL_3);
    }

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err action_DoNothing()
{
    enum mesif_err eError = eNO_ERROR;
    int dontcare = 0;
    int nothing[MAX_STATE_COMBO];

    if(sMesifBits.eFlag == eCBUS)
    {
        dontcare = eCB_DONTCARE;
        nothing[eCOL_2] = eCB_DONTCARE;
        nothing[eCOL_3] = eCB_DONTCARE;
    }
    else if(sMesifBits.eFlag == eSBUS)
    {
        dontcare = eSB_DONTCARE;
        nothing[eCOL_2] = eSB_DONTCARE;
        nothing[eCOL_3] = eSB_DONTCARE;
    }
    else
    {
        eError = eFLAG_ERROR;
    }

    sMesifBits.sCol_done[1] = TRUE;
    sMesifBits.sCol_done[2] = TRUE;


    if(eError == eNO_ERROR)
    {
        eError = UpdateEvents(nothing, eCOL_2);
    }

    if(eError == eNO_ERROR)
    {
        eError = UpdateEvents(nothing, eCOL_3);
    }

    return eError;
}


