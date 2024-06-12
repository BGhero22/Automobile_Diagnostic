/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "SocketCan.h"
#include "CanTP.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
SocketCanData SocketData;
/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/

void SocketCan_init()
{
	
	strcpy(SocketData.ifr.ifr_name, "can1");

	ioctl(SocketData.s, SIOCGIFINDEX, &SocketData.ifr);
	memset(&SocketData.addr, 0, sizeof(SocketData.addr));
	SocketData.addr.can_family = AF_CAN;
	SocketData.addr.can_ifindex = SocketData.ifr.ifr_ifindex;
	if ((SocketData.s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("Socket");
		
	
	}


if (bind(SocketData.s, (struct sockaddr *)&SocketData.addr, sizeof(SocketData.addr)) < 0)
{
        perror("Bind");

}

if (close(SocketData.s) < 0)
{
        perror("Close");

}

}
void sendRequestToCar(uint8_t *payload, uint8_t len, uint32_t id)
{
		uint8_t *tmpArr=payload;
        SocketData.frame.can_id = id;
        SocketData.frame.can_dlc = len;
	for(int i=0;i<len;i++){
		SocketData.frame.data[i]=tmpArr[i];
	}
         

        if (write(SocketData.s, &SocketData.frame, sizeof( SocketData.frame)) != sizeof( SocketData.frame))
        {
                perror("Write");

        }
}

void ReadResponseFromCar()

{
        int nbytes = 0;
        nbytes = read(SocketData.s, &SocketData.frame, sizeof(SocketData.frame));
        if (nbytes > 0)
        {
                receivePDU(SocketData.frame.data, SocketData.frame.can_dlc, SocketData.frame.can_id);
        }
}
void SocketCan_Run()
{

        ReadResponseFromCar();
}
