#ifndef CAN_TP_PRV_H
#define CAN_TP_PRV_H



/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/
void dataProcessing();
void readyToSendState();
void firstFrameState();
void singleFrameState();
void flowControlWaitState();
void consecutiveFrameState();
void doneState();
void continueStateOfFlowControl();
void sendingCFs();
void receiveSuccessfulCF(uint8_t buff[8], int len,int id);
void singleFrameReceive(uint8_t buff[8]);
void firstFrameReceived(uint8_t buff[8], int len, uint8_t id);
void consecutiveFrameReceived(uint8_t buff[8], int len, uint8_t id);
void flowControlReceived(uint8_t buff[8]);
#endif // CAN_TP_PRV_H