/**
* File "Packet.h"
* Created by Sina on Thu May  7 18:41:22 2015.
*/

#pragma once

#include "primary_header.h"
#include "crc.h"

#define  ADDR_LEN 128
#define DATA_LEN 23
#define INIT_TTL 20

enum PacketType{
    CONTROL,
    SETADDR,
    DATA,
    CONNECT
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

    Packet() {  }
    Packet( char* ss ) { setPacketFromArray(ss); }
private:
    bitset<16> type;
    bitset<128> dest;
    bitset<128> source;
    bitset<32>  ttl;
    bitset<40> length;
    bitset<184> data;
    bitset<48> crc;

};
