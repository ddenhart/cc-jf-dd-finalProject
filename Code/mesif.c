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
BusOperation:	Used to simulate a bus operation and to capture the 
				snoop results of last level caches of other processors

@input:			enum proc_res BusOp 
				unsigned int Address 
				enum snoop_res *SnoopResult

@output:		None

================================================================================== */
void BusOperation(enum proc_res BusOp, unsigned int Address, enum snoop_res *SnoopResult)
{
	#ifndef SILENT
		printf("BusOp:%d,Address:%h,SnoopResult:%d\n",*SnoopResult);
	#endif
}


/* ===============================================================================
GetSnoopResult: Used to simulate the reporting of snoop results by other caches

@input:			unsigned int Address

@output:		enum snoop_res

================================================================================== */
enum snoop_res GetSnoopResult(unsigned int Address)
{

}


/* ===============================================================================
PutSnoopResult:	Used to report the result of our snooping bus 
				operations by other caches 

@input:			unsigned int Address
				enum snoop_res SnoopResult

@output:		None

================================================================================== */
void PutSnoopResult(unsigned int Address, enum snoop_res SnoopResult)
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

}


/* ===============================================================================
proc_res nextBusEvent: this function...

@input: enum Mesif_states 
		the current state of mesif that the line is in

@output: enum proc_res
		 the n
================================================================================== */
enum proc_res nextBusEvent( enum Mesif_states current_state )
{     enum proc_res bus_event = getBusEvent();

	if (((bus_event >= 0) && (bus_event < ePR__MAX_EVENTS))		&& ((current_state >= 0) && (current_state < ePR__MAX_EVENTS)))
	{
		busStateSelect(current_state, &bus_event);
	} 
	else 
	{
		// invalid event/state    }	return bus_event;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum proc_res getBusEvent()
{

}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum snoop_res nextSnoopEvent( enum Mesif_states current_state){    enum snoop_res snoop_event;

	if (((snoop_event >= 0) && (snoop_event < eSR_MAX_EVENTS))		&& ((current_state >= 0) && (current_state < eSR_MAX_EVENTS)))
	{
		snoopStateSelect(current_state, &snoop_event );
	} 
	else 
	{
		// invalid event/state    }	return snoop_event;}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum snoop_res getSnoopEvent()
{

}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum proc_res busStateSelect( enum Mesif_states eCurrent, int *iEventCode )
{
	enum proc_res eProcNext = ePR__MAX_EVENTS;

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
				eProcNext = ePR__MAX_EVENTS;


	return eProcNext;
}

/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum snoop_res snoopStateSelect(enum Mesif_states eCurrent, int *iEventCode)
{
	enum snoop_res eSnoopNext = eSR_MAX_EVENTS;

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
				eSnoopNext = eSR_MAX_EVENTS;

	return eSnoopNext;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_READ;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_READ;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_WRITE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_RFO;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[1] = eSR_HITM;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_READ;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_READ;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_WRITE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_RFO;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[1] = eSR_HIT;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_READ;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_READ;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_WRITE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_INVALIDATE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_RFO;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_RFO;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[1] = eSR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[1] = eSR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[1] = eSR_DONTCARE;
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
		if(eFlag == eProc)
		{
			iEventCode[0] = ePR_READ;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[0] = eSR_DONTCARE;
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
		if(eFlag == eProc)
		{
			iEventCode[0] = ePR_WRITE;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[0] = eSR_DONTCARE;
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
		if(eFlag == eSnoop)
		{
			iEventCode[0] = eSR_DONTCARE;
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
		if(eFlag == eSnoop)
		{
			iEventCode[0] = eSR_DONTCARE;
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
		if(eFlag == eProc)
		{
			iEventCode[1] = ePR_NOHIT;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[1] = eSR_DONTCARE;
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
		if(eFlag == eProc)
		{
			iEventCode[1] = ePR_HIT;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[1] = eSR_DONTCARE;
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
		if(eFlag == eSnoop)
		{
			iEventCode[1] = eSR_DONTCARE;
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
		if(eFlag == eProc)
		{
			iEventCode[2] = ePR_MEMREAD;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[2] = eSR_DONTCARE;
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
		if(eFlag == eProc)
		{
			iEventCode[2] = ePR_RFO;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[2] = eSR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_READ;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_READ;
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
		if (eFlag == eProc)
		{
			iEventCode[0] = ePR_WRITE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_INVALIDATE;
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
		if (eFlag == eProc)
		{
			iEventCode[2] = ePR_RFO;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[0] = eSR_RFO;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
		}
		else if (eFlag == eSnoop)
		{
			iEventCode[1] = eSR_HIT;
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
		if (eFlag == eProc)
		{
			iEventCode[1] = ePR_DONTCARE;
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
