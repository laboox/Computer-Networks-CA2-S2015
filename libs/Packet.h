/**
* File "Packet.h"
* Created by Sina on Thu May  7 18:41:22 2015.
*/

#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "crc.h"
#include "primary_header.h"


#define  ADDR_LEN 128
#define DATA_LEN 23
#define INIT_TTL 20
#define MSG_LEN 72

enum PacketType{
    UPDATE,
    SETADDR,
    REQADDR,
    DATA,
    CONNECT,
    SETUPSERVICE,
    GET_SERVICES_LIST
};

class Packet {
public:
    void setType(PacketType);
    void setDest(bitset<128> dest) { this->dest = dest; }
    void setSource(bitset<128> source) { this->source = source; }
    void setTtl(unsigned int ttl) { this->ttl = bitset<32>(ttl); }
    void setData(char data[23], unsigned int); 
    PacketType getType() { return (PacketType)type.to_ulong(); }
    bitset<128> getDest() { return dest; }
    bitset<128> getSource() { return source; }
    unsigned int getTtl() { return ttl.to_ulong(); }
    unsigned int getLength() { return length.to_ulong(); }
    int getData(char*);
    void decTtl();

    void putCrc();
    int checkCrc();

    bitset<528> getPacketBitsetKam();
    bitset<576> getPacketBitset();
    void getMessageByPacket ( char* );
    void getPacketByMessage ( char* );

    void send(int sock, int port);
    void recive(int sock, struct sockaddr_in* from_sockadrr);

    Packet();
    Packet( char* ss );
private:
    bitset<16> type;
    bitset<128> dest;
    bitset<128> source;
    bitset<32>  ttl;
    bitset<40> length;
    bitset<184> data;
    bitset<48> crc;

};
