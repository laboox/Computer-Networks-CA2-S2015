/**
* File "Framer.cpp"
* Created by Sina on Fri May  8 18:20:06 2015.
*/

void sendFrame(const char* data, int count, bitset<ADDR_LEN> source, bitset<ADDR_LEN> dest){
    int base=0;
    unsigned char packetNumber=0;
    char* first = data;
    while(base < count){
        char inPacketData[23] = {0};
        Packet p;
        p.setType(DATA);
        p.setDest(dest);
        p.setSource(source);
        p.setTtl(INIT_TTL);

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
        //TODO: send packet
    }
}
