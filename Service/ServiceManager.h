/**
* File "ServiceManager.h"
* Created by Sina on Fri May  8 19:31:10 2015.
*/

#pragma once

#include "Packet.h"
#include "Framer.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



class ServiceManager {
public:
    void sendFile(string ,string);
    void run();
    void init(int, string);
    void connectServer();
    string getList();
    void sendError(string message);
private:
    int port;
    struct sockaddr_in server;
    int sock;
    string path;
    address source;
};
