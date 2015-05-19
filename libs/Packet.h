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


#define ADDR_LEN 128
#define DATA_LEN 23
#define INIT_TTL 20
#define MSG_LEN 72

typedef bitset<ADDR_LEN> address;

enum PacketType{
    UPDATE,
    SET_ADDR,
    REQ_ADDR,
    DATA,
    CONNECT,
    DISCONNECT,
    SETUPSERVICE,
    ERROR,
    GET_SERVICES_LIST,
    REQ_READ,
    REQ_WRITE,
    SEND_FILE,
    APPEND_FILE,
    REQ_SERVER
};

class Packet {
public:
    void setType(PacketType);
    void setDest(address dest) { this->dest = dest; }
    void setSource(address source) { this->source = source; }
    void setTtl(unsigned int ttl) { this->ttl = bitset<32>(ttl); }
    void setData(const char data[23], unsigned int); 
    void setData(string data);
    PacketType getType() { return (PacketType)type.to_ulong(); }
    address getDest() { return dest; }
    address getSource() { return source; }
    unsigned int getTtl() { return ttl.to_ulong(); }
    unsigned int getLength() { return length.to_ulong(); }
    int getData(char*);
    string getDataStr();
    void decTtl();

    void putCrc();
    int checkCrc();

    bitset<528> getPacketBitsetKam();
    bitset<576> getPacketBitset();
    void getMessageByPacket ( char* );
    void getPacketByMessage ( char* );

    void send(int sock, int port);
    void send(int sock, struct sockaddr_in* to_sockaddr);
    void recive(int sock, struct sockaddr_in* from_sockadrr);

    Packet();
    Packet(char* ss);
private:
    bitset<16> type;
    bitset<128> dest;
    bitset<128> source;
    bitset<32>  ttl;
    bitset<40> length;
    bitset<184> data;
    bitset<48> crc;

};
