#ifndef CAN_TP_DEFS_H
#define CAN_TP_DEFS_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "/mnt/c/Diplomna/TickTimer/TickTimerTypes.h"
#include <stdbool.h>
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef enum Flow_Control_enum
{
    Continue,
    Wait,
    Overflow,
    Error
} FLow_Control_State;

typedef struct Flow_Control_Frame_Struct{
int fs;
int bs;
int stMin;
}Flow_Control_Frame;

typedef enum State_Enum{
Idle,
Ready_To_Send,
First_Frame,
Consecutive_Frame,
Flow_Control_Wait,
Single_Frame,
Response_Wait,
ResponseReceived,
Done
}State;


typedef struct Sending_Frame_Struct{
uint32_t id;
uint8_t *messageForSending;
bool remainingChunksFlag;
int lastSendChunk;
int frames;
int lengthOfMessageGivenToSendByOtherComponent;
int counter;
TickTimer_TimerType timer;
TickTimer_TimerType stMinTimer;
TickTimer_TimerStateType stateOfStMinTimer;
TickTimer_TimerStateType stateOfTimer;
FLow_Control_State fcState;
Flow_Control_Frame fc;
int currentCFIndex;
bool errorFlag;
}Sending_Frame;

typedef struct Received_Frame_Struct{
 uint32_t id;
uint8_t messageReceived[64];
uint8_t flowControl[3] ;
uint16_t FirstFrameDataLength;
uint8_t SingleFrameDataLength;
int currentCFIndex;
int lastIndexOfPreviousChunk;
Flow_Control_Frame fc;
int counter;
TickTimer_TimerType timer;
TickTimer_TimerStateType stateOfTimer;
TickTimer_TimerType stMinTimer;
TickTimer_TimerStateType stateOfStMinTimer;
int maxWaitToSend;
int waitCounter;
}
Received_Frame;

typedef struct
{
   State state;
   Sending_Frame sf;
   Received_Frame rf;
   bool dataToSendRequest;
   
} CanTpDataType;

 


#endif // CAN_TP_DEFS_H