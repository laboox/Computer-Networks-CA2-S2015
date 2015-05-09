/**
* File "main.cpp"
* Created by Sina on Fri May  8 11:37:14 2015.
*/

#include "Packet.h"

int main() {
    Packet p;
    char data[23] = "salam";
    char packchar[72] = {0};
    p.setType(DATA);
    p.setDest(bitset<128>(127));
    p.setSource(bitset<128>(128));
    p.setTtl(120);
    p.setData(data, 6);
    
    p.putCrc();

    p.getPacketByteArray (packchar);
    

    Packet test;
    char packtest[72] = {0};
    test.setPacketFromArray(packchar);
    test.getPacketByteArray(packtest);

    

    return 0;
}
