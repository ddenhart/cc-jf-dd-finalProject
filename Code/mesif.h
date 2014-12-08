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

#define CPU_CODE_ROWS 6
#define SYS_CODE_ROWS 7
#define MAX_ERRORS  10
#define MAX_STATE_COMBO 3

#define errRow0 "no error"
#define errRow1 "the state machine has an unassigned state"
#define errRow2 "the bus has an unassigned sender"
#define errRow3 "the column combination of events is unassigned"
#define errRow4 "there is an unassigned cpu bus event"
#define errRow5 "there is an unassigned system bus event"
#define errRow6 "a null pointer was found"
#define errRow7 "out of range command"
#define errRow8 "the snoop result is not valid"
#define errRow9 "the bus operation is not valid"

// Bus operation types
typedef enum cpu_bus { 
        eCB_READ = 10, // Bus Read
        eCB_WRITE = eCB_READ + 1, // Bus Write

        eCB_NOHIT = eCB_WRITE + 1,
        eCB_HIT = eCB_NOHIT + 1,

        eCB_MEMREAD = eCB_HIT + 1,
        eCB_RFO = eCB_MEMREAD + 1,
        eCB_INVALIDATE = eCB_RFO +1, // Bus Invalidate

        eCB_DONTCARE = eCB_INVALIDATE + 1,
        eCB__MAX_EVENTS = eCB_DONTCARE + 1
}cpu_bus_t;


// Snoop result types
typedef enum system_bus
{
        eSB_RFO = 20,
        eSB_READ = eSB_RFO + 1,
	
        eSB_HIT = eSB_READ +1, // Hit
        eSB_HITM = eSB_HIT + 1, // Hit to a modified line

        eSB_WRITEBACK = eSB_HITM + 1,
        eSB_FORWARD = eSB_WRITEBACK + 1,

        eSB_DONTCARE = eSB_FORWARD + 1,
        eSB_MAX_EVENTS = eSB_DONTCARE + 1
}system_bus_t;


typedef enum eventColumn
{
    eCOL_1 = 0,
    eCOL_2 = eCOL_1 + 1,
    eCOL_3 = eCOL_2 + 1,
    eCOL_MAX = eCOL_3 +1
}eventColumn_t;

typedef enum mesif_type
{
        eCBUS = 0,
        eSBUS = 1 
}mesif_type_t;

unsigned int valid_CPU_Codes[][eCOL_MAX] = {
        {eCB_READ, eCB_HIT, eCB_MEMREAD},           //0:I->F
        {eCB_READ, eCB_NOHIT, eCB_MEMREAD},         //1:I->E
        {eCB_WRITE, eCB_DONTCARE, eCB_RFO},         //2:I->M, F->M, S->M, F->M
        {eCB_READ, eCB_DONTCARE, eCB_DONTCARE},     //3:M->M, F->F, S->S, E->E
        {eCB_WRITE, eCB_DONTCARE, eCB_INVALIDATE},   //4:F->M, S->M
        {eCB_WRITE, eCB_DONTCARE, eCB_DONTCARE}    //5:M->M, E->M
};

typedef enum CPU_Rows
{
        eCROW_I_F = 0,
        eCROW_I_E = 1,
        eCROW_I_M_XF_M_XS_M_XF_M = 2,
        eCROW_M_M_XF_F_XS_S_XE_E = 3,
        eCROW_F_M_XS_M = 4,
        eCROW_M_M_XE_M = 5
}CPU_Rows_t;

unsigned int valid_SYS_Codes[][eCOL_MAX] = {
        {eSB_RFO, eSB_HIT, eSB_FORWARD},            //1:F->I,E->I
        {eSB_RFO, eSB_HITM, eSB_WRITEBACK},         //3:M->I
        {eSB_RFO, eSB_DONTCARE, eSB_DONTCARE},      //0:S->I
        {eSB_READ, eSB_HIT, eSB_FORWARD},           //2:F->S,E->S
        {eSB_READ, eSB_HITM, eSB_WRITEBACK},        //4:M->S
        {eSB_READ, eSB_DONTCARE, eSB_DONTCARE},      //5:S->S   
        {eSB_DONTCARE, eSB_DONTCARE, eSB_DONTCARE}  //6:I->I
};

typedef enum SYS_Rows
{
        eSROW_F_I_XE_I = 0,
        eSROW_M_I = 1,
        eSROW_S_I = 2,
        eSROW_F_S_XE_S = 3,
        eSROW_M_S = 4,
        eSROW_S_S = 5,
        eSROW_I_I = 6
}SYS_Rows_t;

//MESIF Error codes
typedef enum mesif_err
{
    eNO_ERROR = 0,
    eINVALID_STATE_ERROR = eNO_ERROR -1,      //the state machine has an unassigned state
    eFLAG_ERROR = eINVALID_STATE_ERROR -1,    //the bus has an unassigned sender
    eINVALID_COLUMN_ERROR = eFLAG_ERROR -1,   //the column combination of events is unassigned
    eCBUS_ERROR = eINVALID_COLUMN_ERROR -1,   //there is an unassigned cpu bus event
    eSBUS_ERROR = eCBUS_ERROR -1,             //there is an unassigned system bus event 
    eSYNTAX_NULL_ERROR = eSBUS_ERROR -1,      //a null pointer was found
    eINVALID_COMMAND = eSYNTAX_NULL_ERROR -1, //out of range command 
    eINVALID_SNOOP = eINVALID_COMMAND -1,     //the snoop result is not valid
    eINVALID_BUS_OP = eINVALID_SNOOP -1       //the bus operation is not valid
}mesif_err_t;


//MESIF Error table
struct err_codeToMessage {
    int errCode[MAX_ERRORS];
    char *errMsg[MAX_ERRORS];
}sErrorTable;

//MESIF params
struct mesif_params
{
    unsigned int address;   //the incoming address
    unsigned int cmd;   //the incoming command
    unsigned int set;   //the affected set in the cache
    unsigned int line;  //the affected line in the cache
    enum mesif_type_t eFlag;  //either from the cpu or the system
    int iEventCode[MAX_STATE_COMBO];    //a list of events that determine the next state
    int sCol_done[MAX_STATE_COMBO];  //keeps track of if events have occured
}sMesifBits;

struct mesif_stats
{
    int toM;
    int toE;
    int toS;
    int toI;
    int toF;
};

struct mesif_stats fromM;
struct mesif_stats fromE;
struct mesif_stats fromS;
struct mesif_stats fromI;
struct mesif_stats fromF;

int errorCount;
void cleanMesif();
void InitError();
enum mesif_err_t eventCodeCheck(enum Mesif_states_t eCurrent, enum Mesif_states_t *eNext);
//Read the line MESIF state
enum Mesif_states_t GetCurrentState();
int compareCodes();
enum mesif_err_t GetHitorM(enum Mesif_states_t eCurrent);
enum Mesif_states_t GetHitMiss(int found, enum Mesif_states_t eCurrent);
enum mesif_err_t  StateSelect(int iStateRow, enum Mesif_states_t eCurrent, enum Mesif_states_t *eNext);
enum mesif_err_t SetMesifState(enum Mesif_states_t eState);
enum mesif_err_t UpdateEvents(int *iEventCode, enum eventColumn_t eCol);
enum mesif_err_t CommandMux(enum Mesif_states_t eCurrent);
void PrintError(enum mesif_err_t eError, enum Mesif_states_t eCurrent);
void printForward(unsigned int Address);
void printWriteback(unsigned int Address);
void printState(enum Mesif_states_t eCurrent, enum Mesif_states_t eNext);
//char *GetStateName(enum Mesif_states_t eState);
char *GetCodeName(int code);

//function prototypes for each action 
enum mesif_err_t actionM_Read(enum mesif_type_t eFlag);
enum mesif_err_t actionM_Write(enum mesif_type_t eFlag);
enum mesif_err_t actionM_RFO(enum mesif_type_t eFlag);
enum mesif_err_t actionM_NOHIT(enum mesif_type_t eFlag);
enum mesif_err_t actionM_HIT(enum mesif_type_t eFlag);
enum mesif_err_t actionM_HITM(enum mesif_type_t eFlag);
enum mesif_err_t actionM_Invalidate(enum mesif_type_t eFlag);
//enum mesif_err_t actionM_Forward(enum mesif_type_t eFlag);
enum mesif_err_t actionM_Memread(enum mesif_type_t eFlag);
enum mesif_err_t actionM_Writeback(enum mesif_type_t eFlag);

enum mesif_err_t actionE_Read(enum mesif_type_t eFlag);
enum mesif_err_t actionE_Write(enum mesif_type_t eFlag);
enum mesif_err_t actionE_RFO(enum mesif_type_t eFlag);
enum mesif_err_t actionE_NOHIT(enum mesif_type_t eFlag);
enum mesif_err_t actionE_HIT(enum mesif_type_t eFlag);
enum mesif_err_t actionE_HITM(enum mesif_type_t eFlag);
enum mesif_err_t actionE_Invalidate(enum mesif_type_t eFlag);
enum mesif_err_t actionE_Forward(enum mesif_type_t eFlag);
enum mesif_err_t actionE_Memread(enum mesif_type_t eFlag);
//enum mesif_err_t actionE_Writeback(enum mesif_type_t eFlag);

enum mesif_err_t actionS_Read(enum mesif_type_t eFlag);
enum mesif_err_t actionS_Write(enum mesif_type_t eFlag);
enum mesif_err_t actionS_RFO(enum mesif_type_t eFlag);
enum mesif_err_t actionS_NOHIT(enum mesif_type_t eFlag);
enum mesif_err_t actionS_HIT(enum mesif_type_t eFlag);
enum mesif_err_t actionS_HITM(enum mesif_type_t eFlag);
enum mesif_err_t actionS_Invalidate(enum mesif_type_t eFlag);
enum mesif_err_t actionS_Forward(enum mesif_type_t eFlag);
enum mesif_err_t actionS_Memread(enum mesif_type_t eFlag);
enum mesif_err_t actionS_Writeback(enum mesif_type_t eFlag);

enum mesif_err_t actionI_Read(enum mesif_type_t eFlag);
enum mesif_err_t actionI_Write(enum mesif_type_t eFlag);
enum mesif_err_t actionI_RFO(enum mesif_type_t eFlag);
enum mesif_err_t actionI_NOHIT(enum mesif_type_t eFlag);
enum mesif_err_t actionI_HIT(enum mesif_type_t eFlag);
enum mesif_err_t actionI_HITM(enum mesif_type_t eFlag);
enum mesif_err_t actionI_Invalidate(enum mesif_type_t eFlag);
enum mesif_err_t actionI_Forward(enum mesif_type_t eFlag);
enum mesif_err_t actionI_Memread(enum mesif_type_t eFlag);
enum mesif_err_t actionI_Writeback(enum mesif_type_t eFlag);

enum mesif_err_t actionF_Read(enum mesif_type_t eFlag);
enum mesif_err_t actionF_Write(enum mesif_type_t eFlag);
enum mesif_err_t actionF_RFO(enum mesif_type_t eFlag);
enum mesif_err_t actionF_NOHIT(enum mesif_type_t eFlag);
enum mesif_err_t actionF_HIT(enum mesif_type_t eFlag);
enum mesif_err_t actionF_HITM(enum mesif_type_t eFlag);
enum mesif_err_t actionF_Invalidate(enum mesif_type_t eFlag);
enum mesif_err_t actionF_Forward(enum mesif_type_t eFlag);
enum mesif_err_t actionF_Memread(enum mesif_type_t eFlag);
enum mesif_err_t actionF_Writeback(enum mesif_type_t eFlag);

enum mesif_err_t action_DoNothing();





