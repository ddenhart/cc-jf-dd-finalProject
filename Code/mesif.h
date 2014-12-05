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
#define CPU_CODE_ROWS 8;
#define SYS_CODE_ROWS 9;

// Bus operation types
enum cpu_bus { 
        eCB_READ = 10, // Bus Read
        eCB_WRITE = eCB_READ + 1, // Bus Write

        eCB_NOHIT = eCB_WRITE + 1,
        eCB_HIT = eCB_NOHIT + 1,

        eCB_MEMREAD = eCB_HIT + 1,
        eCB_RFO = eCB_MEMREAD + 1,
        eCB_INVALIDATE = eCB_RFO +1, // Bus Invalidate

        eCB_DONTCARE = eCB_INVALIDATE + 1,
        eCB__MAX_EVENTS = eCB_DONTCARE + 1
};


// Snoop result types
enum system_bus {
        eSB_RFO = 20,
        eSB_READ = eSB_RFO + 1,
	
        eSB_HIT = eSB_READ +1, // Hit
        eSB_HITM = eSB_HIT + 1, // Hit to a modified line

        eSB_WRITEBACK = eSB_HITM + 1,
        eSB_FORWARD = eSB_WRITEBACK + 1,

        eSB_DONTCARE = eSB_FORWARD + 1,
        eSB_MAX_EVENTS = eSB_DONTCARE + 1
};


enum eventColumn
{
    eCOL_1 = 0,
    eCOL_2 = eCOL_1 + 1,
    eCOL_3 = eCOL_2 + 1,
    eCOL_MAX = eCOL_3 +1
};


//MESIF Error codes
enum mesif_err { 
        eNO_ERROR = 0,
	     eINVALID_STATE_ERROR = -100,    //the state machine has an unassigned state
	     eFLAG_ERROR = -110,             //the bus has an unassigned sender
        eINVALID_COLUMN_ERROR = -120,   //the column combination of events is unassigned
	     eCBUS_ERROR = -200,             //there is an unassigned cpu bus event
	     eSBUS_ERROR = -300,             //there is an unassigned system bus event 
	     eSYNTAX_NULL_ERROR = -400       //a null pointer was found
};

enum mesif_type{ 
        eCBUS = 0,
        eSBUS = 1 };

unsigned int valid_CPU_Codes[CPU_CODE_ROWS][eCOL_MAX] = {
        {eCB_READ, eCB_HIT, eCB_MEMREAD},
        {eCB_READ, eCB_NOHIT, eCB_MEMREAD},
        {eCB_WRITE, eCB_DONTCARE, eCB_RFO},
        {eCB_WRITE, eCB_DONTCARE, eCB_DONTCARE},
        {eCB_READ, eCB_DONTCARE, eCB_DONTCARE}, 
        {eCB_WRITE, eCB_DONTCARE, eCB_RFO},
        {eCB_WRITE, eCB_DONTCARE, eCB_INVALIDATE},
        {eCB_WRITE, eCB_DONTCARE, eCB_DONTCARE}
};

unsigned int valid_SYS_Codes[SYS_CODE_ROWS][eCOL_MAX] = {
        {eSB_DONTCARE, eSB_DONTCARE, eSB_DONTCARE},
        {eSB_RFO, eSB_DONTCARE, eSB_DONTCARE},
        {eSB_RFO, eSB_HIT, eSB_FORWARD},
        {eSB_READ, eSB_HIT, eSB_FORWARD},
        {eSB_RFO, eSB_HITM, eSB_WRITEBACK},
        {eSB_READ, eSB_HITM, eSB_WRITEBACK},
        {eSB_RFO, eSB_HIT, eSB_FORWARD},
        {eSB_READ, eSB_HIT, eSB_FORWARD},
        {eSB_READ, eCB_DONTCARE, eCB_DONTCARE}
};


struct sMesif {
	enum Mesif_states current_state;
	enum cpu_bus bus[eSB_MAX_EVENTS];
	enum system_bus snoop[eSB_MAX_EVENTS];
	enum mesif_err eERROR;
};


// Used to simulate a bus operation and to capture the 
// snoop results of last level caches of other processors
void BusOperation(enum cpu_bus BusOp, unsigned int Address, enum system_bus *SnoopResult);

// Used to simulate the reporting of snoop results by other caches
enum system_bus GetSnoopResult(unsigned int Address);

// Used to report the result of our snooping bus operations by other caches  
void PutSnoopResult(unsigned int Address, enum system_bus SnoopResult); 

//Read the line MESIF state
unsigned int GetMesifState(unsigned int set, unsigned int line);

// Get the current state of a line
enum Mesif_states nextCPUState(enum Mesif_states eState);

// Get the current state of a line
enum Mesif_states nextSnoopState(enum Mesif_states eState);

enum cpu_bus nextBusEvent ( enum Mesif_states current_state );

// Get the latest bus event
enum cpu_bus getBusEvent ( );

enum system_bus nextSnoopEvent ( enum Mesif_states current_state );

// Get the latest snoop event
enum system_bus getSnoopEvent();


int busStateSelect(enum mesif_type eFlag, int iRow);
enum system_bus snoopStateSelect(enum mesif_type eFlag, int iRow);

enum mesif_err eventCodeCheck(enum mesif_type eFlag, int *iEventCode);
int compareCodes(enum mesif_type eFlag, int *iEventCode);

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





