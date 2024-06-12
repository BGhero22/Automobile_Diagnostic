
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "CanTp.h"
#include "CanTpCbk.h"
#include "CanTpPrv.h"
#include "TickTimer.h"
#include "SocketCan.h"
#include "Obd2.h"
#include<stddef.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#define STATES 7
#define CHUNKSIZE 8
/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
CanTpDataType CanTpData;
/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void idleState()
{
CanTpData.state=Done;
}

void readyToSendState()
{
    // Implement actions for Ready_To_Send state
   

    if (CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent > 8)
    {
        CanTpData.state = First_Frame;
    }
    else
    {
        CanTpData.state = Single_Frame;
    }
  
}

void firstFrameState()
{
    // Implement actions for First_Frame state
   
    uint8_t tempBuff[CHUNKSIZE];

    CanTpData.sf.frames = (CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent - (CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent % 8)) / 8; // founding number of messages
    // send first frame;

    for (int x = 0; x < CHUNKSIZE; x++)
    {
        tempBuff[x] = CanTpData.sf.messageForSending[x];
    }
    sendChunkToCan(tempBuff, CHUNKSIZE, CanTpData.sf.id);
    CanTpData.state = Flow_Control_Wait;

   
}

void singleFrameState()
{

    
    // Implement actions for Single_Frame state
    sendChunkToCan(CanTpData.sf.messageForSending, CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent, CanTpData.sf.id);
    CanTpData.state = Done;
    releaseBuffer();
    
}

void flowControlWaitState()
{
    
    // Implement actions for Flow_Control_Wait state

  

    while (CanTpData.sf.stateOfTimer != TickTimer_Expired )
    {
        if (CanTpData.sf.fcState != Error)
        {

            CanTpData.state = Consecutive_Frame;
            CanTpData.sf.stateOfTimer = TickTimer_Uninitialized;

            break;
        }
    }

    if (CanTpData.sf.fcState == Error)
    {
        CanTpData.state = Done;

         releaseBuffer();
    }

    
}

void consecutiveFrameState()
{
    

    // Implement actions for Consecutive_Frame state
    switch (CanTpData.sf.fcState)
    {
    case Continue:
        continueStateOfFlowControl();

        break;
    case Wait:
        // Waiting
        break;

    case Overflow:
        for (int i = 0; i < CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent; i++)
        {
            CanTpData.sf.messageForSending[i] = 0u;
            CanTpData.state = Done;
            // callback with correct func
            releaseBuffer();
        }
        break;

    case Error:
        for (int i = 0; i < CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent; i++)
        {
            CanTpData.sf.messageForSending[i] = 0u;
        }
        // callback with correct func
        CanTpData.state = Done;

        releaseBuffer();
        break;

    default:
        break;
    }
    
}

void doneState()
{
   
   

    // Implement actions for Done state
    CanTpData.sf.messageForSending = NULL;
    CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent = 0;
    CanTpData.sf.currentCFIndex = 0;
    CanTpData.sf.remainingChunksFlag = 0;
    CanTpData.sf.lastSendChunk = 1;
    CanTpData.sf.frames = 0;
    CanTpData.sf.counter = 0;
    CanTpData.sf.fcState = Error;
    CanTpData.sf.fc.bs = -1;
    CanTpData.sf.fc.stMin = -1;
    CanTpData.sf.fc.fs = -1;
    CanTpData.state = Idle;  
    
    
   
}
void CanTp_Init(void)
{

    
    

        CanTpData.sf.messageForSending = NULL;
        CanTpData.sf.currentCFIndex = 0;
        CanTpData.sf.remainingChunksFlag = 0;
        CanTpData.sf.lastSendChunk = 1;
        CanTpData.sf.frames = 0;
        CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent = 0;
        CanTpData.sf.counter = 0;
        CanTpData.sf.fcState = Error;
        CanTpData.sf.fc.bs = -1;
        CanTpData.sf.fc.stMin = -1;
        CanTpData.sf.fc.fs = -1;
        CanTpData.sf.stateOfTimer = TickTimer_Uninitialized;
        CanTpData.sf.stateOfStMinTimer = TickTimer_Uninitialized;
        
       
        
        CanTpData.rf.counter = 1;
        CanTpData.rf.FirstFrameDataLength = 0;
        CanTpData.rf.lastIndexOfPreviousChunk = 0;
        CanTpData.rf.SingleFrameDataLength = 0;
        CanTpData.rf.fc.bs = 8;
        CanTpData.rf.fc.stMin = 95;
        CanTpData.rf.fc.fs = 48;
        CanTpData.rf.currentCFIndex = 0;
        CanTpData.rf.flowControl[0] = CanTpData.rf.fc.fs;
        CanTpData.rf.flowControl[1] = CanTpData.rf.fc.bs;
        CanTpData.rf.flowControl[2] = CanTpData.rf.fc.stMin;
        CanTpData.rf.stateOfTimer = TickTimer_Uninitialized;
        CanTpData.rf.stateOfStMinTimer = TickTimer_Uninitialized;
        CanTpData.rf.timer.inverseStartTs = 0;
        CanTpData.dataToSendRequest=false;
        CanTpData.state = Idle;
    
    
        
    
}

void CanTp_Run(void)
{
    switch (CanTpData.state)
    {
     case Idle:
        idleState();
        break;
    case Ready_To_Send:
        readyToSendState();
        break;
    case First_Frame:
        firstFrameState();
        break;
    case Consecutive_Frame:
        consecutiveFrameState();
        break;
    case Flow_Control_Wait:
        flowControlWaitState();
        break;
    case Single_Frame:
        singleFrameState();
        break;
    case Done:
        doneState();
        break;
    default:
        // Handle unknown state
        break;
    }
}

void sendPDU(uint8_t buff[], int len, uint8_t* responseArr, uint32_t id) 
{

    // check if component is init
   
        CanTpData.sf.messageForSending = buff;
        CanTpData.sf.id=id;
        CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent = len;
        responseArr=CanTpData.rf.messageReceived;
        CanTpData.state = Ready_To_Send;
        
    
}


void continueStateOfFlowControl()
{
    uint8_t tempBuff[CHUNKSIZE];
    int p;

    if (CanTpData.sf.frames > 1)
    {
        sendingCFs();
        
    }
    else
    {
        TickTimer_SetTimer(&CanTpData.sf.timer, 1000, TickTimer_DelayMode);
        bool readyFlag  = false;

        while (CanTpData.sf.stateOfTimer != TickTimer_Expired )
        {
            if (readyFlag == true)
            {
                CanTpData.sf.stateOfTimer = TickTimer_Uninitialized;
                break;
            }
            TickTimer_SetTimer(&CanTpData.sf.stMinTimer, CanTpData.sf.fc.stMin, TickTimer_DelayMode);
            while (CanTpData.sf.stateOfStMinTimer != TickTimer_Expired)
            {
                TickTimer_CheckTimer(&CanTpData.sf.stMinTimer, &CanTpData.sf.stateOfStMinTimer);
            }

            for (p = 0; p < (CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent % 8); p++)
            {
                tempBuff[p] = CanTpData.sf.messageForSending[(CanTpData.sf.counter * 8) + p];
            }

            TickTimer_CheckTimer(&CanTpData.sf.timer, &CanTpData.sf.stateOfTimer);
            if (CanTpData.sf.stateOfTimer == TickTimer_Expired)
            {
                // NUll the buff
                // reset the parameters to done
                for (int i = 0; i < CHUNKSIZE; i++)
                {
                    tempBuff[i] = 0;
                }
                CanTpData.sf.errorFlag = true;
                break;
            }
            sendChunkToCan(tempBuff, p, CanTpData.sf.id);
            readyFlag = true;
        }
        CanTpData.sf.remainingChunksFlag = 0;
        CanTpData.sf.frames = 0;
        CanTpData.state = Done;
        releaseBuffer(); // wrong func

        // callback with correct func
    }
    
}
void sendingCFs()
{
    CanTpData.sf.errorFlag = false;
    int p;
    uint8_t tempBuff[CHUNKSIZE];
    bool readyFlag = false;
    if (CanTpData.sf.frames > CanTpData.sf.fc.bs)
    {
        /*
        Sending max possible number of frames
        */
        for (CanTpData.sf.counter = CanTpData.sf.lastSendChunk; CanTpData.sf.counter < CanTpData.sf.fc.bs; CanTpData.sf .counter++)
        {
            TickTimer_SetTimer(&CanTpData.sf.timer, 1000, TickTimer_DelayMode);
            
            while (CanTpData.sf.stateOfTimer != TickTimer_Expired)
            {

                if (readyFlag == true)
                {
                    CanTpData.sf.stateOfTimer = TickTimer_Uninitialized;
                    break;
                }

                TickTimer_SetTimer(&CanTpData.sf.stMinTimer, CanTpData.sf.fc.stMin, TickTimer_DelayMode);
                while (CanTpData.sf.stateOfStMinTimer != TickTimer_Expired)
                {
                    TickTimer_CheckTimer(&CanTpData.sf.stMinTimer, &CanTpData.sf.stateOfStMinTimer);
                }

                for (int i = 0; i < CHUNKSIZE; i++)
                {
                    tempBuff[i] = CanTpData.sf.messageForSending[i + (CanTpData.sf.counter * 8)];
                }
                TickTimer_CheckTimer(&CanTpData.sf.timer, &CanTpData.sf.stateOfTimer);
                if (CanTpData.sf .stateOfTimer == TickTimer_Expired)
                {
                    // NUll the buff
                    // reset the parameters to done

                    CanTpData.sf.counter = CanTpData.sf .fc.bs + 1;
                    CanTpData.sf.errorFlag = true;
                    break;
                }
                sendChunkToCan(tempBuff, 8, CanTpData.sf .id);
                
            }
        }
            readyFlag = true;
        if (CanTpData.sf.errorFlag == false)
    {
            CanTpData.sf.remainingChunksFlag = 1;
            CanTpData.sf.lastSendChunk = CanTpData.sf .counter;
            CanTpData.sf.frames -= CanTpData.sf .fc.bs;
            CanTpData.state = Flow_Control_Wait;
    }        
        
        else
        {
            for (int i = 0; i < CHUNKSIZE; i++)
            {
                tempBuff[i] = 0;
            }
            CanTpData.sf.remainingChunksFlag = 0;
            CanTpData.sf.frames = 0;
            CanTpData.state = Done;
            releaseBuffer();
        }
    
    
}

    else
    {

        /*

        Sending the fully frames of the last part of the message
        */
        for (CanTpData.sf.counter = 1; CanTpData.sf.counter < CanTpData.sf.frames; CanTpData.sf.counter++)
        {

            TickTimer_SetTimer(&CanTpData.sf.stMinTimer, CanTpData.sf.fc.stMin, TickTimer_DelayMode);
            while (CanTpData.sf.stateOfStMinTimer != TickTimer_Expired)
            {
                TickTimer_CheckTimer(&CanTpData.sf.stMinTimer, &CanTpData.sf.stateOfStMinTimer);
            }

            TickTimer_SetTimer(&CanTpData.sf.timer, 1000, TickTimer_DelayMode);

            while ((CanTpData.sf.stateOfTimer) != TickTimer_Expired )
            { // CanTpData.sf .errorFlag = true;
                if (readyFlag == true)
                {
                    CanTpData.sf.stateOfTimer = TickTimer_Uninitialized;
                    break;
                }

                for (int i = 0; i < CHUNKSIZE; i++)
                {
                    tempBuff[i] = CanTpData.sf.messageForSending[i + (CanTpData.sf.counter * 8)];
                }

                TickTimer_CheckTimer(&CanTpData.sf.timer, &CanTpData.sf.stateOfTimer);
                if (CanTpData.sf.stateOfTimer == TickTimer_Expired)
                {
                    // NUll the buff
                    // reset the parameters to done
                    for (int i = 0; i < CHUNKSIZE; i++)
                    {
                        tempBuff[i] = 0;
                    }
                    CanTpData.sf.counter = CanTpData.sf.frames + 1;
                    CanTpData.sf.errorFlag = true;
                    break;
                }

                sendChunkToCan(tempBuff, CHUNKSIZE, CanTpData.sf .id);
               
                CanTpData.sf.errorFlag = false;
            }
        }

        /*
        Sending the last frame with remaining data

        */
        if (CanTpData.sf.errorFlag == false)
        {
            TickTimer_SetTimer(&CanTpData.sf.timer, 1000, TickTimer_DelayMode);

            while (CanTpData.sf.stateOfTimer != TickTimer_Expired)
            {
                if (readyFlag == true)
                {
                    CanTpData.sf.stateOfTimer = TickTimer_Uninitialized;
                    break;
                }
                for (p = 0; p < (CanTpData.sf.lengthOfMessageGivenToSendByOtherComponent % 8); p++)
                {
                    tempBuff[p] = CanTpData.sf.messageForSending[(CanTpData.sf .counter * 8) + p];
                }

                TickTimer_CheckTimer(&CanTpData.sf.timer, &CanTpData.sf.stateOfTimer);
                if (CanTpData.sf.stateOfTimer == TickTimer_Expired)
                {
                    // NUll the buff
                    // reset the parameters to done
                    for (int i = 0; i < CHUNKSIZE; i++)
                    {
                        tempBuff[i] = 0;
                    }
                    CanTpData.sf.errorFlag = true;
                    // add error code
                    break;
                }
                sendChunkToCan(tempBuff, p, CanTpData.sf .id);
                
            }
        }

        readyFlag = true;
        CanTpData.sf.remainingChunksFlag = 0;
        CanTpData.sf.frames = 0;
        CanTpData.state = Done;

        releaseBuffer();
    }

 
}

void singleFrameReceive(uint8_t buff[8])
{
   

    if ((buff[0] & 0x0Fu) == 0u)
    { // first case sf_dl is in byte 1
        CanTpData.rf.SingleFrameDataLength = buff[1];
        for (int i = 2; i < CanTpData.rf.SingleFrameDataLength; i++)
        { // single frame
            CanTpData.rf.messageReceived[i] = buff[i];
        }
        //id check
        ResponseGetFromCar();
        CanTpData.state = Done;
       
    }
    else
    {
        CanTpData.rf.SingleFrameDataLength = buff[0] & 0x0Fu;
        if (CanTpData.rf.SingleFrameDataLength > 8u) // len is over max possible for this type of frame
        {
             // thirdCase
             CanTpData.state = Done;
        }
        else
        {
            // secondCase sf_dl is in low nibble of byte 0
            for (int i = 1; i < CanTpData.rf.SingleFrameDataLength; i++)
            { // single frame
                CanTpData.rf.messageReceived[i] = buff[i];
            }
           ResponseGetFromCar();
           CanTpData.state = Done;
        }
    }

}

void firstFrameReceived(uint8_t buff[8], int len, uint8_t id)
{
    
    if (len < 8 || (buff[0] & 0x0Fu) == 0u)
    {                   // errorCases
         // wrong type or buffer overflow
    }

    else
    {
        CanTpData.rf.FirstFrameDataLength = ((((uint16_t)buff[0]) & 0x0Fu) << 8 || (buff[1])); // combine low nibble byte0 and byte1
        if (CanTpData.rf.FirstFrameDataLength > (sizeof(CanTpData.rf.messageReceived) / sizeof(CanTpData.rf.messageReceived[0])))
        { // too short message buffer

            CanTpData.rf.flowControl[0] = 50u;
            sendChunkToCan(CanTpData.rf.flowControl, 3, id);
            
        }
        else
        {
            { // normal case

                for (int i = 2; i < 8; i++)

                    CanTpData.rf.messageReceived[i] = buff[i];
            }

            CanTpData.rf.lastIndexOfPreviousChunk += 6;

            
            sendChunkToCan(CanTpData.rf .flowControl, 3, id);

            
        }
    }

    
}

void consecutiveFrameReceived(uint8_t buff[8], int len, uint8_t id)
{
    

    // consecutive message
    
    TickTimer_CheckTimer(&CanTpData.rf.timer, &CanTpData.rf.stateOfTimer);
    if (CanTpData.sf .stateOfTimer == TickTimer_Expired)
    { // null the parameters
        
        CanTpData.rf.counter = 0;
        CanTpData.rf.FirstFrameDataLength = 0;
        CanTpData.rf.lastIndexOfPreviousChunk = 0;
        CanTpData.rf.SingleFrameDataLength = 0;
        
    }
    else
    {
        if (buff != NULL)
        {
            TickTimer_CheckTimer(&CanTpData.rf.stMinTimer, &CanTpData.rf .stateOfStMinTimer);
            if (CanTpData.sf.stateOfStMinTimer == TickTimer_Expired || CanTpData.sf.stateOfStMinTimer == TickTimer_Uninitialized)
            {
                  receiveSuccessfulCF(buff, len, id);
            }
            else
            {
               
                    CanTpData.rf .flowControl[0] = 49;
                    sendChunkToCan(CanTpData.rf.flowControl, 3, id);
                    
                
                
            }
        }
        else
        { // null the parameters
            CanTpData.rf.counter = 0;
            CanTpData.rf.FirstFrameDataLength = 0;
            CanTpData.rf.lastIndexOfPreviousChunk = 0;
            CanTpData.rf.SingleFrameDataLength = 0;
            
        }
    }
    // when to stop start the timer
    if (CanTpData.rf.lastIndexOfPreviousChunk + len < CanTpData.rf.FirstFrameDataLength)
    {
        TickTimer_SetTimer(&CanTpData.rf.timer, 1000, TickTimer_DelayMode);
    }
    else{
        ResponseGetFromCar();
    }
}

void flowControlReceived(uint8_t buff[8])
{
    

    // flow control

    if ((buff[0] & 0x0Fu) > 2u)
    {
        /*invalid
    the message
    transmission shall be aborted and the network layer shall make an N_USData.confirm service call with
    the parameter <N_Result> = N_INVALID_FS to the adjacent upper layer
    */
        
    }
    else if (((buff[2] > 0x7Fu) && (buff[2] < 0xF1u)))
    {
        CanTpData.sf.fc.fs = buff[0] & 0x0Fu;
        CanTpData.sf.fc.bs = buff[1];
        CanTpData.sf.fc.stMin = 0x7Fu;
        
    }
    else if (buff[2] > 0xF9u)
    {
        CanTpData.sf.fc.fs = buff[0] & 0x0Fu;
        CanTpData.sf.fc.bs = buff[1];
        CanTpData.sf.fc.stMin = 0x7Fu;
        
    }
    else
    {
        CanTpData.sf.fc.fs = buff[0] & 0x0Fu;
        CanTpData.sf.fc.bs = buff[1];
        CanTpData.sf.fc.stMin = buff[2];
        
    }

    switch (CanTpData.sf.fc.bs)
    {
    case 0:
        CanTpData.sf.fcState = Continue;
        break;

    case 1:
        
         if (CanTpData.rf.waitCounter < CanTpData.rf.maxWaitToSend)
                {CanTpData.sf.fcState = Wait;
                
                    CanTpData.rf .waitCounter += 1;
                }
                else
                {
                
                    CanTpData.rf.counter = 0;
                    CanTpData.rf.FirstFrameDataLength = 0;
                    CanTpData.rf.lastIndexOfPreviousChunk = 0;
                    CanTpData.rf.SingleFrameDataLength = 0;
                     CanTpData.sf.fcState = Error;
                    // abort the transmission
                }
        break;

    case 2:
        CanTpData.sf.fcState = Overflow;
        break;

    case -1:
        CanTpData.sf.fcState = Error;
        break;

    default:
        break;
    }
    
}

void receivePDU(uint8_t buff[8], int len, uint8_t id)
{

    int type = (buff[0] >> 4);
    

   
        if (id == CanTpData.rf.id)
        {

            switch (type)
            {
            case 0: // single frame
                 singleFrameReceive(buff);
                break;
            case 1: // first frame

                 firstFrameReceived(buff, len, id);

                break;
            case 2:
                 consecutiveFrameReceived(buff, len, id);

                break;

            case 3:
                 flowControlReceived(buff);
                break;

            default:
                break;
            }
        }
    
    
}

void receiveSuccessfulCF(uint8_t buff[8], int len, int id)
{
    
    CanTpData.rf .maxWaitToSend = 0;
    if ((buff[0] & 0x0Fu) - CanTpData.rf.currentCFIndex == 1u)
    { // normal case
        for (int j = CanTpData.rf.lastIndexOfPreviousChunk + 1; j < (CanTpData.rf.lastIndexOfPreviousChunk + len); j++)
        {

            CanTpData.rf.messageReceived[j] = buff[CanTpData.rf.counter];
            CanTpData.rf.counter++;
        }
        CanTpData.rf.currentCFIndex += 1;
        if (CanTpData.rf.currentCFIndex == CanTpData.rf.fc.bs && CanTpData.rf.lastIndexOfPreviousChunk + len < CanTpData.rf.FirstFrameDataLength)
        {
            sendChunkToCan(CanTpData.rf.flowControl, 3, id);
        }
       
        TickTimer_SetTimer(&CanTpData.rf.stMinTimer, 1000, TickTimer_DelayMode);
        
    }
    else
    { // wrong sequence number
        for (int n = 0; n < len; n++)
        {
            buff[n] = 0u;
        }

        for (int nm = 0; nm < CanTpData.rf.currentCFIndex; nm++)
        {
            CanTpData.rf.messageReceived[nm] = 0u;
        }

    
    }
    
}

void sendChunkToCan(uint8_t buff[],int len,uint32_t id)
{
    sendRequestToCar(buff,len,id);
    
}

void releaseBuffer()
{
     CanTpData.sf.messageForSending=NULL;
}
