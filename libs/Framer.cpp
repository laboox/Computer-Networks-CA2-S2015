/**
* File "Framer.cpp"
* Created by Sina on Fri May  8 18:20:06 2015.
*/

#include "Framer.h"

void sendFrame(char* data, int count, address source, address dest, int sock, int port){
    int base=0;
    unsigned char packetNumber=0;
    char* first = data;
    while(base < count){
        char inPacketData[23] = {0};
        Packet p;
        p.setType(DATA);
        p.setDest(dest);
        p.setSource(source);

        if(count-base>DATA_LEN-1){
            inPacketData[0] = packetNumber;
            char* startOfData = inPacketData+1;
            memcpy(startOfData, first, DATA_LEN);
            p.setData(first, DATA_LEN);
            p.putCrc();
            first+=DATA_LEN-1;
            base+=DATA_LEN-1;
        }
        else {
            inPacketData[0] = packetNumber;
            char* startOfData = inPacketData+1;
            memcpy(startOfData, first, count-base );
            p.setData(first, count-base);
            p.putCrc();
            first+=count-base;
            base+=count-base;
        }
        p.send(sock, port);
    }
}

void sendFrame(char* data, int count, address source, address dest, int sock, 
    struct sockaddr_in* go){
    int base=0;
    unsigned char packetNumber=0;
    char* first = (char*)data;
    while(base < count){
        char inPacketData[23] = {0};
        Packet p;
        p.setType(DATA);
        p.setDest(dest);
        p.setSource(source);

        if(count-base>DATA_LEN-1){
            inPacketData[0] = packetNumber;
            char* startOfData = inPacketData+1;
            memcpy(startOfData, first, DATA_LEN);
            p.setData(first, DATA_LEN);
            p.putCrc();
            first+=DATA_LEN-1;
            base+=DATA_LEN-1;
        }
        else {
            inPacketData[0] = packetNumber;
            char* startOfData = inPacketData+1;
            memcpy(startOfData, first, count-base );
            p.setData(first, count-base);
            p.putCrc();
            first+=count-base;
            base+=count-base;
        }
        p.send(sock, go);
    }
}

void reciveFrame( char* data, int sock, struct sockaddr_in* from ) {
    char* first = data;
    int count=0;

    while(1){
        char temparr[30] = {0};
        char* temp = temparr;
        Packet p;
        p.recive(sock, from);
        int n = p.getData(temp);
        if(count!=temp[0])
            throw Exeption("error reciving frame");
        temp = (char*)temp + 1;
        memcpy(first, temp, DATA_LEN-1);
        first += n-1;
        if(n<DATA_LEN)
            break;
    }
}

