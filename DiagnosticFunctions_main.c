#include "/mnt/c/Diplomna/CanTP/CanTP.h"
#include "/mnt/c/Diplomna/OBDII/Obd2.h"
#include "/mnt/c/Diplomna/SocketCan/SocketCan.h"
#include "/mnt/c/Diplomna/TCPServer/Server.hpp"


int main(){
    
   

    SocketCan_init();
    OBD_Init();
     CanTp_Init();
    Server server;
    server.serverInit();
   

while(1){
    OBD_Run();
    CanTp_Run ();
    SocketCan_Run();
    server.serverStart();
     
}
    
    return 0;
}