#ifndef CAN_TP_H
#define CAN_TP_H


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "CanTpDefs.h"
#include<stdint.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

extern void CanTp_Init( void );
extern void CanTp_Run ( void );

void sendPDU(uint8_t buff[], int len, uint8_t* responseArr, uint32_t id);
void receivePDU(uint8_t buff[],int len,uint8_t id);
#endif // CAN_TP_H