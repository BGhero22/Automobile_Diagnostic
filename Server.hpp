/***
Class server implement in itself all functionalities of the station which recieve
and collect information from air qualitiy stations.
When the object of this class is intialized and started
it begins to listen for conections from quality stations and keep them and their
data in list so it can update data from that stations on exact period of time 
or listen if extraoridany event happened and being indicated for this 

***/

#ifndef _SERVER_HPP_
#define _SERVER_HPP_


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include<stdint.h>
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/

typedef struct{
char Req[255];
bool hasPid;
int pid;
}Data_Server;


	extern "C" void getResponse(uint8_t buff[],int len);



class Server {


public:

	Data_Server data;


	void serverInit();///method who intialize the parameters of server and after that calls the start method of the server
	char responseBuff[255];
	void sendResponse();///method which sends acknoledge response
	void closeConn();///close conection method
	void binding();
	void listening();///this method listen for conections
	void accepting();///method which is responsible for accepting new conections
	void serverStart();///start method is call after server is initializedand by itself it  accept new conections through other method and beign to exchange information with it			
	bool receiveInfo();///this method recieve information and check if it is correct then give it to other methods for processing
	
	void userInput();///this method simulates user interface for server
	
	int serverfd, clientfd;
	char addr[INET_ADDRSTRLEN];
	struct sockaddr_in serverAddr;
	bool connRunning;
	
};


#endif // !_SERVER_HPP_