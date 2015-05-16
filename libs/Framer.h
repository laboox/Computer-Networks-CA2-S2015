/**
* File "Framer.h"
* Created by Sina on Fri May  8 18:20:11 2015.
*/

#pragma once

#include "primary_header.h"
#include "Packet.h"

bool isFileExist(string path);

string readAllFile(string path);

void sendFrame(char* data, int count, address source, address dest, int sock, int port);

void sendFrame(char* data, int count, address source, address dest, int sock, 
    struct sockaddr_in* go);

void reciveFrame( char* data, int sock, struct sockaddr_in* from );

void reciveFrame( char* data, int sock, int port ); 

string reciveFrame(int sock, struct sockaddr_in* from);
