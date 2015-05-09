/**
* File "main.cpp"
* Created by Sina on Sat May  9 13:48:16 2015.
*/

#include "ServerManager.h"

int main(int argc, char *argv[]){
    if(argc < 2)
    {
        cout<< "Usage: ./Server.out #Port_Number" <<endl;
        exit(0);
    }
    ServerManager server(atoi(argv[1]));
    server.run();
}
