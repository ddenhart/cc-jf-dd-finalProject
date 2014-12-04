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
Modified:		Deborah Denhart		12/02/14
Description: 	This file contains:
				- 
	
================================================================================== */

#define MAX_STATE_COMBO 3;

// Bus operation types
enum proc_res { ePR_READ = 10, // Bus Read
				ePR_WRITE = ePR_READ + 1, // Bus Write

				ePR_NOHIT = ePR_WRITE + 1,
				ePR_HIT = ePR_NOHIT + 1,

				ePR_MEMREAD = ePR_HIT + 1,
				ePR_RFO = ePR_MEMREAD + 1,
				ePR_INVALIDATE = ePR_RFO +1, // Bus Invalidate

				ePR_DONTCARE = ePR_INVALIDATE + 1,
				ePR__MAX_EVENTS = ePR_DONTCARE + 1
};


// Snoop result types
enum snoop_res {eSR_RFO = 20,
				eSR_READ = eSR_RFO + 1,
	
				eSR_HIT = eSR_READ +1, // Hit
				eSR_HITM = eSR_HIT + 1, // Hit to a modified line

				eSR_WRITEBACK = eSR_HITM + 1,
				eSR_FORWARD = eSR_WRITEBACK + 1,

				eSR_DONTCARE = eSR_FORWARD + 1,
				eSR_MAX_EVENTS = eSR_DONTCARE + 1
};

//MESIF Error codes
enum mesif_err { eNO_ERROR = 0,
				 eINVALID_STATE_ERROR = -100,
				 eFLAG_ERROR = -110,
				 eProc_ERROR = -200,
				 eSNOOP_ERROR = -300,
				 eSYNTAX_NULL_ERROR = -400 };

enum mesif_type{ eProc = 0,
				 eSnoop = 1 };

struct sMesif {
	enum Mesif_states current_state;
	enum proc_res bus[eSR_MAX_EVENTS];
	enum snoop_res snoop[eSR_MAX_EVENTS];
	enum mesif_err eERROR;
};

// Used to simulate a bus operation and to capture the 
// snoop results of last level caches of other processors
void BusOperation(enum proc_res BusOp, unsigned int Address, enum snoop_res *SnoopResult);

// Used to simulate the reporting of snoop results by other caches
enum snoop_res GetSnoopResult(unsigned int Address);

// Used to report the result of our snooping bus operations by other caches  
void PutSnoopResult(unsigned int Address, enum snoop_res SnoopResult); 

//Read the line MESIF state
unsigned int GetMesifState(unsigned int set, unsigned int line);

// Get the current state of a line
enum Mesif_states nextCPUState(enum Mesif_states eState);

// Get the current state of a line
enum Mesif_states nextSnoopState(enum Mesif_states eState);

enum proc_res nextBusEvent(enum Mesif_states current_state);

// Get the latest bus event
enum proc_res getBusEvent();

enum snoop_res nextSnoopEvent(enum Mesif_states current_state);

// Get the latest snoop event
enum snoop_res getSnoopEvent();


enum proc_res busStateSelect(enum Mesif_states eCurrent, int *iEventCode);
enum snoop_res snoopStateSelect(enum Mesif_states eCurrent, int *iEventCode);


//function prototypes for each action 
enum mesif_err actionM_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_RWIM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_MEMREAD(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionE_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_RWIM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_MEMREAD(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionS_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_RWIM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_MEMREAD(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionI_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_RWIM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_MEMREAD(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionF_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_RWIM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_MEMREAD(enum mesif_type eFlag, int *iEventCode);

void action_doNothing();




