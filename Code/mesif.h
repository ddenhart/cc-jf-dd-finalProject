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

//MESIF states
//Invalid was chosen to be first in the enum list so that it could be also used
//to initialize the bits
enum Mesif_states { eINVALID  = 0, // Cache line is invalid					eMODIFIED = eINVALID +1, // Only this cache has the line and the memory is stale					eEXCLUSIVE = eMODIFIED +1, // Only this cache has the line and the memory is up to date 					eSHARED = eEXCLUSIVE +1, // At least two other caches have the line and memory is up to date					eFORWARD = eSHARED + 1, // Cache line is in the shared state and this is the forwarding processor					eMAX_STATES = eFORWARD +1 };
// Bus operation typesenum bus_ops { eINVALIDATE = 10, // Bus Invalidate
			   eREAD = eINVALIDATE +1, // Bus Read
			   eWRITE = eREAD +1, // Bus Write
			   eRWIM = eWRITE +1,  // Bus read with intend to modify
			   eNOHIT = eNOHIT +1,
			   eHIT = eRWIM +1,
			   eMEMREAD = eHIT +1,
			   eRFO = eMEMREAD +1,
			   eDONTCARE = eRFO +1,
			   eB_MAX_EVENTS = eDONTCARE +1 };

// Snoop result types
enum snoop_res { eHIT = 20, // Hit
				eHITM = eHIT +1, // Hit to a modified line
				eRFO = eHITM +1,
				eREAD = eRFO +1,
				eWRITEBACK = eREAD +1,
				eFORWARD = eWRITEBACK +1,
				eDONTCARE = eFORWARD +1,
				eS_MAX_EVENTS = eDONTCARE +1 };

//MESIF Error codes
enum mesif_err { eNO_ERROR = 0,
				 eINVALID_STATE_ERROR = -100,
				 eFLAG_ERROR = -110,
				 eBUS_ERROR = -200,
				 eSNOOP_ERROR = -300,
				 eSYNTAX_NULL_ERROR = -400 };

enum mesif_type{ eBus = 0,
				 eSnoop = 1 };

struct sMesif {
	enum Mesif_states current_state;
	enum bus_ops bus[eB_MAX_EVENTS];
	enum snoop_res snoop[eS_MAX_EVENTS];
	enum mesif_err eERROR;
};

// Used to simulate a bus operation and to capture the 
// snoop results of last level caches of other processors
void BusOperation(enum bus_ops BusOp, unsigned int Address, enum snoop_res *SnoopResult);

// Used to simulate the reporting of snoop results by other caches
enum snoop_res GetSnoopResult(unsigned int Address);

// Used to report the result of our snooping bus operations by other caches  
void PutSnoopResult(unsigned int Address, enum snoop_res SnoopResult); 

// Used to simulate communication to our upper level cache 
void MessageToL2Cache(enum bus_ops BusOp, unsigned int Address);

//Read the line MESIF state
unsigned int GetMesifState(unsigned int set, unsigned int line);

// Updates the mesif state
void updateMesifState(unsigned int set, unsigned int line);

// Get the current state of a line
enum Mesif_states nextCPUState(enum Mesif_states eState);

// Get the current state of a line
enum Mesif_states nextSnoopState(enum Mesif_states eState);

enum bus_ops nextBusEvent(enum Mesif_states current_state);

// Get the latest bus event
enum bus_ops getBusEvent(unsigned int Address);

enum snoop_res nextSnoopEvent(enum Mesif_states current_state);

// Get the latest snoop event
enum snoop_res getSnoopEvent(unsigned int Address);


enum bus_ops busStateSelect( enum Mesif_states eCurrent);
enum snoop_res snoopStateSelect( enum Mesif_states eCurrent);


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




