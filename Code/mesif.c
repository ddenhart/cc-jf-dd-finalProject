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
int GetSnoopResult(unsigned int Address)
{

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
unsigned int GetMesifState(unsigned int set, unsigned int line)
{
		return cachePtr[set].setPtr[line].mesifBits;
}


/* ===============================================================================
updateMesifState: Updates the mesif state

@input: 

@output: 

================================================================================== */
// Get the current state of a line
enum Mesif_states nextSnoopState(enum Mesif_states eState)
{
    enum mesif_state eCurrentState = GetMesifState();
}


/* ===============================================================================
cpu_bus nextBusEvent: this function...

@input: enum Mesif_states 
		the current state of mesif that the line is in

@output: enum cpu_bus
		 the n
================================================================================== */
enum cpu_bus nextBusEvent( enum Mesif_states current_state )
{     enum cpu_bus bus_event = getBusEvent();
    /*
	if (((bus_event >= 0) && (bus_event < eCB__MAX_EVENTS))		&& ((current_state >= 0) && (current_state < eCB__MAX_EVENTS)))
	{
		//busStateSelect(current_state, &bus_event);
	} 
	else 
	{
		// invalid event/state   }*/	return bus_event;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum cpu_bus getBusEvent()
{

}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum system_bus nextSnoopEvent( enum Mesif_states current_state){    enum system_bus snoop_event;
    /*
	if (((snoop_event >= 0) && (snoop_event < eSB_MAX_EVENTS))		&& ((current_state >= 0) && (current_state < eSB_MAX_EVENTS)))
	{
		snoopStateSelect(current_state, &snoop_event );
	} 
	else 
	{
		// invalid event/state    }*/	return snoop_event;}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum system_bus getSnoopEvent()
{

}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
int busStateSelect( enum Mesif_states eCurrent, int *iEventCode )
{
	enum cpu_bus eCBUSNext = eCB__MAX_EVENTS;

	switch(eCurrent)
			case eINVALID:
				  //k;
			case eMODIFIED:
				 // k;
			case eEXCLUSIVE:
				  //k;
			case eSHARED:
				  //k;
			case eFORWARD:
				  //k;
			default:
				eCBUSNext = eCB__MAX_EVENTS;


	return eCBUSNext;
}

/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum system_bus snoopStateSelect(enum mesif_type eFlag, int iRow)
{
	

	switch(eCurrent)
			case eINVALID:
				  //k;
			case eMODIFIED:
				  //k;
			case eEXCLUSIVE:
				  //k;
			case eSHARED:
				  //k;
			case eFORWARD:
				  //;
			default:
				eSBUSNext = eSB_MAX_EVENTS;

	return eSBUSNext;
}


/* ===============================================================================
<method name>

@input:

@output:

================================================================================== */
enum mesif_err eventCodeCheck(enum mesif_type eFlag, int *iEventCode)
{
    enum mesif_err eError = eNO_ERROR;
    int iRow = 0;

    iRow = compareCodes(eFlag, iEventCode);

    if(iRow >= 0)
    {
        //use row and flag to get the next state
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
                    if((iEventCode[col]) || (iEventCode[col] == valid_CPU_Codes[col][row]))
                    {
                        bSame = TRUE;
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
                    if((iEventCode[col]) || (iEventCode[col] == valid_SYS_Codes[col][row]))
                    {
                        bSame = TRUE;
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
			iEventCode[0] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_READ;
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
			iEventCode[0] = eCB_WRITE;
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
			iEventCode[2] = eCB_DONTCARE;
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
enum mesif_err actionM_RWIM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_RFO;
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
			iEventCode[1] = eCB_DONTCARE;
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
			iEventCode[1] = eCB_DONTCARE;
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
			iEventCode[1] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[1] = eSB_HITM;
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
			iEventCode[0] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_READ;
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
			iEventCode[0] = eCB_WRITE;
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
			iEventCode[2] = eCB_DONTCARE;
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
enum mesif_err actionE_RWIM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[2] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_RFO;
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
			iEventCode[1] = eCB_DONTCARE;
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
			iEventCode[1] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[1] = eSB_HIT;
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
			iEventCode[1] = eCB_DONTCARE;
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
			iEventCode[0] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_READ;
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
			iEventCode[0] = eCB_WRITE;
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
			iEventCode[2] = eCB_INVALIDATE;
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
enum mesif_err actionS_RWIM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[2] = eCB_RFO;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_RFO;
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
			iEventCode[1] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[1] = eSB_DONTCARE;
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
			iEventCode[1] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[1] = eSB_DONTCARE;
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
			iEventCode[1] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[1] = eSB_DONTCARE;
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
			iEventCode[0] = eCB_READ;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[0] = eSB_DONTCARE;
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
			iEventCode[0] = eCB_WRITE;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[0] = eSB_DONTCARE;
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
enum mesif_err actionI_Invalidate(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eSBUS)
		{
			iEventCode[0] = eSB_DONTCARE;
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
enum mesif_err actionI_RWIM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eSBUS)
		{
			iEventCode[0] = eSB_DONTCARE;
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
			iEventCode[1] = eCB_NOHIT;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[1] = eSB_DONTCARE;
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
			iEventCode[1] = eCB_HIT;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[1] = eSB_DONTCARE;
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
			iEventCode[1] = eSB_DONTCARE;
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
enum mesif_err actionI_MEMREAD(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eCBUS)
		{
			iEventCode[2] = eCB_MEMREAD;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[2] = eSB_DONTCARE;
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
			iEventCode[2] = eCB_RFO;
		}
		else if(eFlag == eSBUS)
		{
			iEventCode[2] = eSB_DONTCARE;
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
			iEventCode[0] = eCB_READ;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_READ;
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
			iEventCode[0] = eCB_WRITE;
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
			iEventCode[2] = eCB_INVALIDATE;
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
enum mesif_err actionF_RWIM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		if (eFlag == eCBUS)
		{
			iEventCode[2] = eCB_RFO;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[0] = eSB_RFO;
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
			iEventCode[1] = eCB_DONTCARE;
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
			iEventCode[1] = eCB_DONTCARE;
		}
		else if (eFlag == eSBUS)
		{
			iEventCode[1] = eSB_HIT;
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
			iEventCode[1] = eCB_DONTCARE;
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
enum mesif_err action_doNothing(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if (iEventCode)
	{
		//no action taken
	}
	else
	{
		eError = eSYNTAX_NULL_ERROR;
	}

	return eError;
}
