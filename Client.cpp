

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include"Client.hpp"
#include<cstring>
#include<cstdlib>
#include<iostream>

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

///method that initialize the client and start method for periodicaly transmitting data to server
void Client::init() {

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(1337);
    clientAddr.sin_addr.s_addr = inet_addr("192.168.7.2");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        perror("socket");
   

    connfd = connect(sockfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    if (connfd < 0)
        perror("connect");

    connRunning = true;
  
}


void Client::responseRecieved() {

    int bytesReceived = recv(sockfd, responseBuf, sizeof(responseBuf), 0);

    if (bytesReceived <= 0)
    {
        perror("recv");
        connRunning = false;
    }
    else{
     printTheResponse();   
    }
    
}


///this method serves for printing the information on screen
void Client::printTheResponse() {

	
			std::cout << "Data: " <<std::endl ;
            for(int i=0;i<255;i++){
                    if(responseBuf[i]!=0){
                        std::cout<<responseBuf[i]<<std::endl ;
                    }
            }
            

}


void Client::readRealTimeData(int pid) 
 {
    
strcpy(data.Req,"Current");
    data.pid=pid;
 send(sockfd, &data.Req, sizeof(data.Req), 0);
 send(sockfd, &data.pid, sizeof(data.pid), 0);
 }

void Client::readDTC()
{
    
    strcpy(data.Req,"Read");
    send(sockfd, &data.Req, sizeof(data.Req), 0);
}
void Client::clearDtc()
{
    
    strcpy(data.Req,"Clear");
    send(sockfd, &data.Req, sizeof(data.Req), 0);
}

void Client::closeConection()
{
    
    strcpy(data.Req,"End");
    send(sockfd, &data.Req, sizeof(data.Req), 0);
}



void Client::userInput() {
	std::cout << "Press 1 to Read ReaTimeData" << std::endl << "Press 2 to Read Error Codes" << std::endl <<  "Press 3 to Clear Error Codes" << std::endl <<"Press 4 to close current connection" << std::endl;

	while (true) {
		int n{ 0 },p;
		std::cin >> n;
		switch (n) {
		case 1:
        std::cout<<"Enter pid"<<std::endl;
        std::cin>>p;
        readRealTimeData(p);
		break;
		case 2:
			readDTC();
            break;
		case 3:
			 clearDtc();
             break;
        case 4:
                closeConection();
		default:
			break;
		}
	}

}

int main(){

    Client client;
    client.init();
    client.userInput();

    return 0;
}