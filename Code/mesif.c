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
Description: 	This file contains:
				- 
	
================================================================================== */

#include <stdio.h>
#include "mesif.h"


enum Mesif_states current_state;
enum snoop_res snoop_event;
enum bus_ops bus_event;

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
void busStateSelect( enum bus_ops eBusCurrent, enum bus_ops *eBusNext)
{
	//case statement
}

/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
void snoopStateSelect( enum snoop_res eSnoopCurrent, enum snoop_res *eSnoopNext)
{
	//case statement
}

/* ===============================================================================
<method name>

@input: 

@output: 

================================================================================== */
void BusOperation(enum bus_ops BusOp, unsigned int Address, enum snoop_res *SnoopResult)
{
	#ifndef DEBUG
		printf(“BusOp:%d,Address:%h,SnoopResult:%d\n”,*SnoopResult);
	#endif


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

void actionI_Read();
void actionI_Write();
void actionI_Invalidate();
void actionI_RWIM();
void actionI_NOHIT();
void actionI_HIT();
void actionI_HITM();

void actionF_Read();
void actionF_Write();
void actionF_Invalidate();
void actionF_RWIM();
void actionF_NOHIT();
void actionF_HIT();
void actionF_HITM();

void action_doNothing();
