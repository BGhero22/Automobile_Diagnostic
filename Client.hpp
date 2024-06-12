#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_


/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>

#include<vector>
/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/


struct Data{
char Req[255];
bool hasPid;
int pid;
};

class Client {
public:
	void init();///method that initialize the client and start method for periodicaly transmitting data to server
	void responseRecieved();///in this method is check if server is send response after information had been sended
    void readRealTimeData(int pid) ;
    void readDTC();
    void clearDtc();
    int userInput();
    void printTheResponse();
	void closeConection();
	Data data;
private:

	int sockfd, connfd;
	 struct sockaddr_in clientAddr;
	 char responseBuf[1024];
	  bool flag=true;
	 bool connRunning;
};
#endif // !_CLIENT_HPP_
