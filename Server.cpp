

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Server.hpp"
#include "/mnt/c/Diplomna/OBDII/Obd2.h"
#include <chrono>
#include <thread>
#include <cstring>
#include <algorithm>
#include <utility>
#include <mutex>
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Implementation
***********************************************************************************************************************/


	
void Server::serverInit() {


	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(1337);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	int option = 1;
	if (setsockopt(this->serverfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(int)) != 0)
	{
		perror("setsockopt");
	}
	binding();
	listening();

	
}

void Server::binding() {
	if (bind(serverfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		perror("bind");
	
}
///this method listen for conections
void Server::listening() {
	if (listen(serverfd,10 ) < 0) {
		perror("listen");
	}

	
}

///method which is responsible for accepting new conections
void Server::accepting() {
	int size = sizeof(serverAddr);
	clientfd = accept(serverfd, (struct sockaddr*)&serverAddr, (socklen_t*)&size);

	if (serverfd < 0)
		perror("accept");

	
	

	connRunning = true;
}



///start method is call after server is initializedand by itself it  accept new conections through other method and begin to exchange information with it

void Server::serverStart() {
	

	
		accepting();
		while (connRunning) {


			 bool tmp = receiveInfo();
			


		}

		
	

}



///this method recieve information and check if it is correct then give it to other methods for processing
bool Server::receiveInfo() {
	bool tmp = true;

	if (recv(clientfd, &data.Req, sizeof(data.Req), 0) == 0)
	{
		strcpy(responseBuff,"Wrong request");
		sendResponse();
	}	
	else{

		if(data.Req=="Read")
		{
			readDTC();
			
			
		}
		else if(data.Req== "Clear")
		{
			clearDtc();
			
		}
		else if(data.Req=="Current")
		{
			recv(clientfd, &data.pid, sizeof(data.pid), 0);
				if ( data.pid!= 0)
					{
						readRealTimeData(data.pid);
					}
			
		}
		else if(data.Req=="End")
		{
			closeConn();
		}
		}
	
	return tmp;
}

///method which sends acknoledge response
 void Server::sendResponse() {

	send(clientfd,responseBuff, sizeof(responseBuff), 0);

}


void getResponse(uint8_t buff[],int len)
{
	Server server;

	for (int i=0;i<len;i++){
	server.responseBuff[i]=buff[i];
	}
	server.sendResponse();
}

///close conection method
void Server::closeConn() {

connRunning=false;

}
