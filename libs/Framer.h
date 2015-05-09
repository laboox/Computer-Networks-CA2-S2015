/**
* File "Framer.h"
* Created by Sina on Fri May  8 18:20:11 2015.
*/

#pragma once

#include "primary_header.h"
#include "Packet.h"


void sendFrame(const char* data, int count, bitset<ADDR_LEN> source, bitset<ADDR_LEN> dest);
