#include "CanTP.h"
#include "Obd2.h"
#include "SocketCan.h"
#include "Server.hpp"


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
