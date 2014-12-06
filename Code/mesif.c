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
#include "mesif.h"
#include "CacheSimulator.h"


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
void UpdateMesif(unsigned int address, unsigned int cmd, unsigned int set, unsigned int line)
{
    enum mesif_err eError = eNO_ERROR;
    enum messif_state eNext = eMAX_STATES;
    enum messif_state eCurrent = eMAX_STATES;
    enum mesif_type eFlag;
    int iEventCode[MAX_STATE_COMBO] = {0, 0, 0};


    //commands

    //if snoop cmds


    eCurrent = GetMesifState(set, line);
    eError = eventCodeCheck(eFlag, iEventCode, eCurrent, &eNext);

    if(eError == eNO_ERROR)
    {
        eError = SetMesifState(eNext, set, line);
    }


    if(eError != eNO_ERROR)
    {
        //print error
    }
}


/* ===============================================================================
BusOperation:	Used to simulate a bus operation and to capture the 
				snoop results of last level caches of other processors

@input:			enum cpu_bus BusOp 
				unsigned int Address 
				enum system_bus *SnoopResult

@output:		None

================================================================================== */
void BusOperation(enum cpu_bus BusOp, unsigned int Address, enum system_bus *SnoopResult)
{
	#ifndef SILENT
		printf("BusOp:%d,Address:%h,SnoopResult:%d\n",*SnoopResult);
	#endif
}


/* ===============================================================================
GetSnoopResult: Used to simulate the reporting of snoop results by other caches

@input:			unsigned int Address

@output:		enum system_bus

================================================================================== */
enum system_bus GetSnoopResult(unsigned int Address)
{
    enum system_bus eSystemEvent;



    return eSystemEvent;
}


/* ===============================================================================
PutSnoopResult:	Used to report the result of our snooping bus 
				operations by other caches 

@input:			unsigned int Address
				enum system_bus SnoopResult

@output:		None

================================================================================== */
void PutSnoopResult(unsigned int Address, enum system_bus SnoopResult)
{
	#ifndef SILENT 
		printf(“SnoopResult: Address %h, SnoopResult: %d\n”, Address,SnoopResult); 
	#endif 
}


/* ==================================================================================
	Function name:	GetMesifState
 	Arguments:		unsigned int set = number of the set containing the desired line
 	 	 	 	 	unsigned int line = number of the line we want to get the MESIF bits for
	Returns:		void
	Description:	Read the line MESIF state
   ================================================================================== */
enum mesif_state GetMesifState(unsigned int set, unsigned int line)
{
		return cachePtr[set].setPtr[line].mesifBits;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum mesif_err SetMesifState(enum Mesif_states eState, unsigned int set, unsigned int line)
{
    enum mesif_err eError = eNO_ERROR;

    if(cachePtr)
    {
        if(cachePtr[set].setPtr)
        {
            cachePtr[set].setPtr[line].mesifBits = eState;
        }
        else
        {            eError = eSYNTAX_NULL_ERROR;
        }
    }
    else
    {
        eError = eSYNTAX_NULL_ERROR;    }        return eError;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum mesif_err StateSelect(enum mesif_type eFlag, int iStateRow, enum Mesif_states eCurrent, enum Mesif_states *eNext)
{
    enum mesif_err eError = eNO_ERROR;

    switch(eCurrent)
    {
        case eINVALID:
            if(eFlag == eCBUS)
            {
                if(eCROW_I_M_XF_M_XS_M_XF_M == iStateRow)
                {
                    eNext = eMODIFIED;
                }
                else if(eCROW_I_E == iStateRow)
                {
                    eNext = eEXCLUSIVE;
                }
                else if(eCROW_I_F == iStateRow)
                {
                    eNext = eFORWARD;
                }
                else
                {                    eError = eCBUS_ERROR;
                }
            }            else if(eFlag ==eSBUS)
            {
                eNext = eINVALID;
            }
            else
            {
                eError = eFLAG_ERROR;            }            break;
        case eMODIFIED:
            if(eFlag == eCBUS)
            {                eNext = eMODIFIED;            }
            else if(eFlag ==eSBUS)
            {
                if(eSROW_M_I == iStateRow)
                {
                    eNext = eINVALID;
                }
                else if(eSROW_M_S == iStateRow)
                { 
                    eNext = eSHARED;
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
            if(eFlag == eCBUS)
            {
                if(eCROW_M_M_XF_F_XS_S_XE_E == iStateRow)
                {
                    eNext = eEXCLUSIVE;
                }
                else if(eCROW_M_M_XE_M == iStateRow)
                {
                    eNext = eMODIFIED;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(eFlag ==eSBUS)
            {
                if(eSROW_E_I == iStateRow)
                {
                    eNext = eINVALID;
                }
                else if(eSROW_E_S == iStateRow)
                {
                    eNext = eSHARED;
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
            if(eFlag == eCBUS)
            {
                if(eCROW_I_M_XF_M_XS_M_XF_M == iStateRow)
                {
                    eNext = eMODIFIED;
                }
                else if(eCROW_M_M_XF_F_XS_S_XE_E == iStateRow)
                {
                    eNext = eSHARED;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(eFlag ==eSBUS)
            {
                if(eSROW_S_I == iStateRow)
                {
                    eNext = eINVALID;
                }
                else if(eSROW_S_S == iStateRow)
                {
                    eNext = eSHARED;
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
            if(eFlag == eCBUS)
            {
                if(eCROW_M_M_XF_F_XS_S_XE_E == iStateRow)
                {
                    eNext = eFORWARD;
                }
                else if(eCROW_F_M_XS_M == iStateRow)
                {
                    eNext = eMODIFIED;
                }
                else
                {
                    eError = eCBUS_ERROR;
                }
            }
            else if(eFlag ==eSBUS)
            {
                if(eSROW_F_I == iStateRow)
                {
                    eNext = eINVALID;
                }
                else if(eSROW_F_S == iStateRow)
                {
                    eNext = eSHARED;
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
enum mesif_err eventCodeCheck(enum mesif_type eFlag, int *iEventCode, enum Mesif_states eCurrent, enum Mesif_states *eNext)
{
    enum mesif_err eError = eNO_ERROR;
    int iRow = 0;

    iRow = compareCodes(eFlag, iEventCode);

    if(iRow >= 0)
    {
        //use row and flag to get the next state
        eError = StateSelect(eFlag, iRow, eCurrent, &eNext);
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
int compareCodes(enum mesif_type eFlag, int *iEventCode)
{
    int col, row;
    int bSame = FALSE;

    if(iEventCode)
        for(row = 0; row < CPU_CODE_ROWS; ++row)
        {
            if(eFlag == eCBUS)
            {
                for(col = 0; col < MAX_STATE_COMBO; ++col)
                {
                    if(iEventCode[col])
                    {
                        if(iEventCode[col] == valid_CPU_Codes[col][row])
                        {
                            bSame = TRUE;
                        }
                        else if(valid_CPU_Codes[col][row] == eCB_DONTCARE)
                        {
                            bSame = TRUE;
                        }
                    }
                    else
                    {
                        bSame = FALSE;
                        break;
                    }
                }
            }
            else if(eFlag == eSBUS)
            {
                for(col = 0; col < SYS_CODE_ROWS; ++col)
                {
                    if(iEventCode[col])
                    {
                        if(iEventCode[col] == valid_SYS_Codes[col][row])
                        {
                            bSame = TRUE;
                        }
                        else if(valid_SYS_Codes[col][row] == eCB_DONTCARE)
                        {
                            bSame = TRUE;
                        }
                    }
                    else
                    {
                        bSame = FALSE;
                        break;
                    }
                }
            }
            else
            {
                bSame = FALSE;
                break;
            }

            //at the end of the row, if bSame is true, the row is a match
            if(bSame)
            {
                break;
            }
            //else keep searching
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
enum mesif_err actionM_Read(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Write(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_RFO(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_NOHIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_HIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_HITM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Invalidate(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Memread(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionM_Writeback(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Read(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Write(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_RFO(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_NOHIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_HIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_HITM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Invalidate(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Forward(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionE_Memread(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Read(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Write(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_RFO(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_RFO;
		}
		else if (eFlag == eSBUS)
		{
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_NOHIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_HIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_HITM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Invalidate(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_INVALIDATE;
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Forward(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Memread(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionS_Writeback(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Read(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Write(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_RFO(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_RFO;
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_NOHIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_HIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_HITM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Memread(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Forward(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionI_Writeback(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}



/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Read(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}



/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Write(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_RFO(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_RFO;
		}
		else if (eFlag == eSBUS)
		{
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_NOHIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_HIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_HITM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Invalidate(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
          iEventCode[eCOL_3] = eCB_INVALIDATE;
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

	return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Forward(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err actionF_Memread(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;

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

    return eError;
}


