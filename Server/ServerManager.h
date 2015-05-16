/**
* File "ServerManager.h"
* Created by Sina on Sat May  9 13:48:35 2015.
*/

#pragma once

#include "primary_header.h"
#include "Packet.h"
#include "Firewall.h"

class ServerManager{
public:
   ServerManager(int port); 
   void run();
   void connect(int port, struct sockaddr_in*);
   void sendError(string ,address);
   string getServiceList(struct sockaddr_in);
   string getAllServiceList();
private:
    int sockSW;
    int sockSP;
    int port;
    int sw_port;
    vector<struct sockaddr_in> services;
    map<char, struct sockaddr_in*> toService;
    address addr;
    Firewall firewall;
};
