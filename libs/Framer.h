/**
* File "Framer.h"
* Created by Sina on Fri May  8 18:20:11 2015.
*/

#pragma once

#include "primary_header.h"
#include "Packet.h"

void sendFrame(const char* data, int count, address source, address dest, int sock, int port);

void sendFrame(const char* data, int count, address source, address dest, int sock, 
    struct sockaddr_in* go);

void reciveFrame( char* data, int sock, struct sockaddr_in* from );

void reciveFrame( char* data, int sock, int port ); 
