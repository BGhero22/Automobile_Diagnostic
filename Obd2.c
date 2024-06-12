/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Obd2.h"
#include "Obd2_Prv.h"
#include "CanTP.h"
#include "Server.hpp"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define OBD2_FUNCTIONAL_BROADCAST_ID 0x7df
#define OBD2_FUNCTIONAL_RESPONSE_START 0x7e8

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Static function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/
void Obd2_idleState()
{
Obd_data.state=Obd2_Done;
}

void Obd2_doneState()
{
Obd_data.state=Obd2_Idle;
}

void readRealTimeData(uint8_t pid)
{
Obd_data.request.can_id=OBD2_FUNCTIONAL_BROADCAST_ID;
Obd_data.request.mode= OBD2_MODE_POWERTRAIN_DIAGNOSTIC_REQUEST;
Obd_data.request.has_pid=true;
Obd_data.request. pid=pid;
Obd_data.request.pid_length=1;
Obd_data.request.payload_length=3;
Obd_data.sendArr[0]=2;
Obd_data.sendArr[1]=OBD2_MODE_POWERTRAIN_DIAGNOSTIC_REQUEST;
Obd_data.sendArr[2]=pid;
Obd_data.response.payload_length=1;
Obd_data.state=Obd2_RequestReceived;
    
}
void readDTC()
{
Obd_data.request.can_id=OBD2_FUNCTIONAL_BROADCAST_ID;
Obd_data.request.mode=OBD2_MODE_EMISSIONS_DTC_REQUEST;
Obd_data.request.has_pid==false;
Obd_data.request.payload_length=2;
Obd_data.sendArr[0]=1;
Obd_data.sendArr[1]=OBD2_MODE_EMISSIONS_DTC_REQUEST;
Obd_data.state=Obd2_RequestReceived;
}
void clearDtc()
{
Obd_data.request.can_id=OBD2_FUNCTIONAL_BROADCAST_ID;
Obd_data.request.mode=OBD2_MODE_EMISSIONS_DTC_CLEAR;
Obd_data.request.has_pid=false;
Obd_data.request.payload_length=2;
Obd_data.sendArr[0]=1;
Obd_data.sendArr[1]=OBD2_MODE_EMISSIONS_DTC_CLEAR;
Obd_data.state=Obd2_RequestReceived;
}
void sendRequest()
{
 sendPDU(Obd_data.sendArr,Obd_data.request.payload_length,Obd_data.receiveArr,OBD2_FUNCTIONAL_BROADCAST_ID);
 Obd_data.state=Obd2_RequestSend;
}
void receiveResponse()
{
    if(ResponseFlag==true){
       Obd_data.state=Obd2_ResponseReceived; 
    }

}

void ResponseGetFromCar()
{
    ResponseFlag=true;
}

void decodeResponse()
{
    int t=0;
    Obd_data.response.can_id=Obd_data.receiveArr[0];
    Obd_data.response.payload_length=Obd_data.receiveArr[1];
    if(Obd_data.request.mode==OBD2_MODE_POWERTRAIN_DIAGNOSTIC_REQUEST){
        t=2;
        
    }
    else{
        t=3;
    }

    for(int i=0;i<Obd_data.response.payload_length;i++){
        Obd_data.response.payload[i]=Obd_data.receiveArr[t];
        t++;
    }
   
    getResponse(Obd_data.response.payload,Obd_data.response.payload_length);
    
    //sent back to TCP server;
}

void OBD_Init(){


    Obd_data.request.can_id=(uint32_t)0;
    Obd_data.request.has_pid=false;
    Obd_data.request.mode=(uint8_t)0;
    Obd_data.request.payload_length=(uint8_t)0;
    Obd_data.request.pid=(uint16_t)0;
    Obd_data.request.pid_length=0;
    Obd_data.response.can_id=(uint32_t)0;
    Obd_data.response.has_pid=false;
    Obd_data.response.mode=(uint8_t)0; 
    Obd_data.response.payload_length=(uint8_t)0;
    Obd_data.response.pid=(uint16_t)0;
}

void OBD_Run(){


    switch (Obd_data.state)
    {
     case Obd2_Idle:
        Obd2_idleState();
        break;
    case Obd2_RequestReceived:
        sendRequest();
        break;
    case Obd2_RequestSend:
        //Wait for response
        receiveResponse();
        break;
    case Obd2_ResponseReceived:
        decodeResponse();
        break;
    case Obd2_Done:
        Obd2_doneState();
        break;
    default:
        break; 
    


    }
}
