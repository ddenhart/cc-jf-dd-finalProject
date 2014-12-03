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
Description: 	This file contains:
				- 
	
================================================================================== */

#include <stdio.h>
#include "mesif.h"
#include "CacheSimulator.h"


//enum Mesif_states current_state;
//enum snoop_res snoop_event;
//enum bus_ops bus_event;


/* ===============================================================================
BusOperation:	Used to simulate a bus operation and to capture the 
				snoop results of last level caches of other processors

@input:			enum bus_ops BusOp 
				unsigned int Address 
				enum snoop_res *SnoopResult

@output:		None

================================================================================== */
void BusOperation(enum bus_ops BusOp, unsigned int Address, enum snoop_res *SnoopResult)
{
	#ifndef DEBUG
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
	#ifndef DEBUG 
		printf(“SnoopResult: Address %h, SnoopResult: %d\n”, Address,SnoopResult); 
	#endif 
}


/* ===============================================================================
MessageToL2Cache:	Used to simulate communication to our upper level cache

@input:				enum bus_ops BusOp 
					unsigned int Address

@output:			None

================================================================================== */
void MessageToL2Cache(enum bus_ops BusOp, unsigned int Address)
{
	#ifndef DEBUG 
		printf(“L2: %d %h\n”, BusOp, Address); 
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
void updateMesifState()
{

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
bus_ops nextBusEvent: this function...

@input: enum Mesif_states 
		the current state of mesif that the line is in

@output: enum bus_ops
		 the n
================================================================================== */
enum bus_ops nextBusEvent( enum Mesif_states current_state )
{     enum bus_ops bus_event = getBusEvent();

	if (((bus_event >= 0) && (bus_event < eS_MAX_EVENTS))		&& ((current_state >= 0) && (current_state < eS_MAX_EVENTS)))
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
enum bus_ops getBusEvent()
{

}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum snoop_res nextSnoopEvent( enum Mesif_states current_state){    enum snoop_res snoop_event;

	if (((snoop_event >= 0) && (snoop_event < eS_MAX_EVENTS))		&& ((current_state >= 0) && (current_state < eS_MAX_EVENTS)))
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
enum bus_ops busStateSelect( enum Mesif_states eCurrent, int *iEventCode )
{
	enum bus_ops eBusNext = eB_MAX_EVENTS;

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


	return eBusNext;
}

/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
enum snoop_res snoopStateSelect(enum Mesif_states eCurrent)
{
	enum snoop_res eSnoopNext = eS_MAX_EVENTS;

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


	return eSnoopNext;
}


/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
void actionM_Read();
void actionM_Write();
void actionM_Invalidate();
void actionM_RWIM();
void actionM_NOHIT();
void actionM_HIT();
void actionM_HITM();

void actionE_Read();
void actionE_Write();
void actionE_Invalidate();
void actionE_RWIM();
void actionE_NOHIT();
void actionE_HIT();
void actionE_HITM();

void actionS_Read();
void actionS_Write();
void actionS_Invalidate();
void actionS_RWIM();
void actionS_NOHIT();
void actionS_HIT();
void actionM_HITM();


enum mesif_err actionI_Read(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[0] = eREAD;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[0] = eDONTCARE;
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


enum mesif_err actionI_Write(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[0] = eWRITE;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[0] = eDONTCARE;
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


enum mesif_err actionI_Invalidate(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eSnoop)
		{
			iEventCode[0] = eDONTCARE;
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


enum mesif_err actionI_RWIM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eSnoop)
		{
			iEventCode[0] = eDONTCARE;
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


enum mesif_err actionI_NOHIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[1] = eNOHIT;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[1] = eDONTCARE;
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


enum mesif_err actionI_HIT(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[1] = eHIT;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[1] = eDONTCARE;
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


enum mesif_err actionI_HITM(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[1] = eDONTCARE;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[1] = eDONTCARE;
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


enum mesif_err actionI_MEMREAD(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[2] = eMEMREAD;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[2] = eDONTCARE;
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


enum mesif_err actionI_RFO(enum mesif_type eFlag, int *iEventCode)
{
	enum mesif_err eError = eNO_ERROR;

	if(iEventCode)
	{
		if(eFlag == eBus)
		{
			iEventCode[2] = eRFO;
		}
		else if(eFlag == eSnoop)
		{
			iEventCode[2] = eDONTCARE;
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



void actionF_Read();
void actionF_Write();
void actionF_Invalidate();
void actionF_RWIM();
void actionF_NOHIT();
void actionF_HIT();
void actionF_HITM();

void action_doNothing();
