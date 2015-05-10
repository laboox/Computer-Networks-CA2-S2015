/**
* File "ServiceManager.cpp"
* Created by Sina on Fri May  8 19:31:06 2015.
*/

#include "ServiceManager.h"

void ServiceManager::sendFile(string path, string addr){
}

void ServiceManager::run(){
    while(true);
}

void ServiceManager::init(int portnum){
   port = portnum;
   int n;
   unsigned int length;
   struct sockaddr_in from;
   struct hostent *hp;
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) Exeption("build sock error!");

   server.sin_family = AF_INET;
   hp = gethostbyname("localhost");
   if (hp==0) Exeption("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(portnum);
   length=sizeof(struct sockaddr_in);
}

void ServiceManager::connectServer(){
    Packet p;
    p.setType(SETUPSERVICE);
    p.send(sock, port);
    return;
}
