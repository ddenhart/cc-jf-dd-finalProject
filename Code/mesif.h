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

#define MAX_STATE_COMBO 3
#define CPU_CODE_ROWS 6
#define SYS_CODE_ROWS 9

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

unsigned int valid_CPU_Codes[][eCOL_MAX] = {
        {eCB_READ, eCB_HIT, eCB_MEMREAD},           //0:I->F
        {eCB_READ, eCB_NOHIT, eCB_MEMREAD},         //1:I->E
        {eCB_WRITE, eCB_DONTCARE, eCB_RFO},         //2:I->M, F->M, S->M, F->M
        {eCB_WRITE, eCB_DONTCARE, eCB_DONTCARE},    //3:M->M, E->M
        {eCB_READ, eCB_DONTCARE, eCB_DONTCARE},     //4:M->M, F->F, S->S, E->E
        {eCB_WRITE, eCB_DONTCARE, eCB_INVALIDATE}   //5:F->M, S->M
};

enum CPU_Rows {
        eCROW_I_F = 0,
        eCROW_I_E = 1,
        eCROW_I_M_XF_M_XS_M_XF_M = 2,
        eCROW_M_M_XE_M = 3,
        eCROW_M_M_XF_F_XS_S_XE_E = 4,
        eCROW_F_M_XS_M = 5
};

unsigned int valid_SYS_Codes[][eCOL_MAX] = {
        {eSB_DONTCARE, eSB_DONTCARE, eSB_DONTCARE}, //0:I->I
        {eSB_RFO, eSB_DONTCARE, eSB_DONTCARE},      //1:S->I
        {eSB_RFO, eSB_HIT, eSB_FORWARD},            //2:F->I
        {eSB_READ, eSB_HIT, eSB_FORWARD},           //3:F->S
        {eSB_RFO, eSB_HITM, eSB_WRITEBACK},         //4:M->I
        {eSB_READ, eSB_HITM, eSB_WRITEBACK},        //5:M->S
        {eSB_RFO, eSB_HIT, eSB_FORWARD},            //6:E->I
        {eSB_READ, eSB_HIT, eSB_FORWARD},           //7:E->S
        {eSB_READ, eCB_DONTCARE, eCB_DONTCARE}      //8:S->S
};

enum SYS_Rows {
        eSROW_I_I = 0,
        eSROW_S_I = 1,
        eSROW_F_I = 2,
        eSROW_F_S = 3,
        eSROW_M_I = 4,
        eSROW_M_S = 5,
        eSROW_E_I = 6,
        eSROW_E_S = 7,
        eSROW_S_S = 8
};


// Used to simulate a bus operation and to capture the 
// snoop results of last level caches of other processors
void BusOperation(enum cpu_bus BusOp, unsigned int Address, enum system_bus *SnoopResult);

// Used to simulate the reporting of snoop results by other caches
enum system_bus GetSnoopResult(unsigned int Address);

// Used to report the result of our snooping bus operations by other caches  
void PutSnoopResult(unsigned int Address, enum system_bus SnoopResult); 

enum mesif_err eventCodeCheck(enum mesif_type eFlag, int *iEventCode, enum Mesif_states eCurrent, enum Mesif_states *eNext);
//Read the line MESIF state
enum Mesif_states GetMesifState(unsigned int set, unsigned int line);
int compareCodes(enum mesif_type eFlag, int *iEventCode);
enum mesif_err  StateSelect(enum mesif_type eFlag, int iStateRow, enum Mesif_states eCurrent, enum Mesif_states *eNext);
enum mesif_err SetMesifState(enum Mesif_states eState, unsigned int set, unsigned int line);

//function prototypes for each action 
enum mesif_err actionM_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_Invalidate(enum mesif_type eFlag, int *iEventCode);
//enum mesif_err actionM_Forward(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_Memread(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionM_Writeback(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionE_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_Forward(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionE_Memread(enum mesif_type eFlag, int *iEventCode);
//enum mesif_err actionE_Writeback(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionS_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Forward(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Memread(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionS_Writeback(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionI_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_HITM(enum mesif_type eFlag, int *iEventCode);
//enum mesif_err actionI_Invalidate(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_Forward(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_Memread(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionI_Writeback(enum mesif_type eFlag, int *iEventCode);

enum mesif_err actionF_Read(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_Write(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_RFO(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_NOHIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_HIT(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_HITM(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_Invalidate(enum mesif_type eFlag, int *iEventCode);
//enum mesif_err actionF_Forward(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_Memread(enum mesif_type eFlag, int *iEventCode);
enum mesif_err actionF_Writeback(enum mesif_type eFlag, int *iEventCode);





