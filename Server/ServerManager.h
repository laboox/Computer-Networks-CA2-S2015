/**
* File "ServerManager.h"
* Created by Sina on Sat May  9 13:48:35 2015.
*/

#pragma once

#include "primary_header.h"
#include "Packet.h"

class ServerManager{
public:
   ServerManager(int port); 
   void run();
   void connect(int port);
   void sendError(string ,address);
   string getServiceList(struct sockaddr_in);
   string getAllServiceList();
private:
    int sockSW;
    int sockSP;
    int port;
    vector<struct sockaddr_in> services;
    map<char, struct sockaddr_in*> toService;
};
