/* ==================================================================================

ECE 485 / Fall 2014 / Cache simulator project
Team: 
		Carmen Ciobanu
		Deborah Denhart
		Jeremiah Franke
		
==================================================================================

File: 			mesif.h
Date:			11/16/2014
Author:			Deborah Denhart
Description: 	This file contains:
				- 
	
================================================================================== */

#define MAX_STATE_COMBO 3;

enum Mesif_states { eINVALID  = 0,					eMODIFIED = eINVALID +1,					eSHARED = eMODIFIED +1, 					eEXCLUSIVE = eSHARED + 1, 					eFORWARD = eEXCLUSIVE +1, 					eMAX_STATES = eFORWARD +1 };enum bus_ops { eREAD = 1, 
			   eWRITE = eREAD +1, 
			   eINVALIDATE = eWRITE +1,
			   eRWIM = eINVALIDATE +1, 
			   eB_MAX_EVENTS = eRWIM +1 };


enum snoop_res { eNOHIT = 0,
				eHIT = eNOHIT +1, 
				eHITM = eHIT +1, 
				eS_MAX_EVENTS = eHITM +1 };

//mesif error codes
enum mesif_err { eNO_ERROR = 0,
				 eINVALID_STATE_ERROR = -100,
				 eBUS_ERROR = -200,
				 eSNOOP_ERROR = -300 };


struct sMesif {
	enum Mesif_states current_state;
	enum bus_ops bus[MAX_STATE_COMBO];
	enum snoop_res snoop[MAX_STATE_COMBO];
	enum mesif_err eERROR;
};


enum bus_ops nextBusEvent(enum Mesif_states current_state);
enum bus_ops getBusEvent();
enum snoop_res nextSnoopEvent(enum Mesif_states current_state);
enum snoop_res getSnoopEvent(unsigned int Address);
void busStateSelect( enum Mesif_states eCurrent, enum bus_ops *eBusNext);
void snoopStateSelect( enum Mesif_states eCurrent, enum snoop_res *eSnoopNext);
void BusOperation(enum bus_ops BusOp, unsigned int Address, enum snoop_res *SnoopResult);

//function prototypes for each action 
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




