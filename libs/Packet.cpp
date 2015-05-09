/**
* File "Packet.cpp"
* Created by Sina on Thu May  7 18:41:24 2015.
*/

#include "Packet.h" 

void Packet::setType(PacketType pt){
    type = bitset<16>(pt);
}

void Packet::setData(char data[23], unsigned int size){
    length = bitset<40>(size);
    this->data.reset();
    for ( int i=0; i< size*8; i++ ) {
        this->data[i] = data[i/8] & 1<<(i%8);
    }
}

int Packet::getData(char* data){
    int size = length.to_ulong();
    memset(data, 0, size);
    for ( int i=0; i< size*8; i++ ) {
        data[i/8] |= (this->data[i])<<(i%8);
    }
}

void Packet::putCrc() {
    char packchar[72] = {0};
    crc.reset();
    Crc32 crcgen;

    getMessageByPacket(packchar);
    crcgen.AddData((unsigned char*)packchar, 66);
    crc = bitset<48>((crcgen.getCrc32()));
}

int Packet::checkCrc() {
    char packchar[72] = {0};
    Crc32 crcgen;

    getMessageByPacket(packchar);
    crcgen.AddData((unsigned char*)packchar, 66);
    bitset<48> crccheck(crcgen.getCrc32());
    return crccheck.to_ulong() == crc.to_ulong();
}

bitset<528> Packet::getPacketBitsetKam() {
    bitset<528> ret;
    bitset<528> temp;
    ret.reset();
    temp.reset();
    
    temp = bitset<528>(type.to_string());
    ret |= temp;

    temp = bitset<528>(dest.to_string());
    ret |= (temp << 16);

    temp = bitset<528>(source.to_string());
    ret |= (temp << 144);
    
    temp = bitset<528>(ttl.to_string());
    ret |= (temp << 272);
    
    temp = bitset<528>(length.to_string());
    ret |= (temp << 304);
    
    temp = bitset<528>(data.to_string());
    ret |= (temp << 344);
    
    return ret;
}

bitset<576> Packet::getPacketBitset(){
    bitset <576> ret;
    bitset <576> temp;
    ret.reset();
    temp.reset();
    ret = bitset<576>(getPacketBitsetKam().to_string());
    temp = bitset<576>(crc.to_string());
    ret |= temp << 528;
    return ret;
}

void Packet::getMessageByPacket ( char* pack ){
    bitset<576> packSet;
    packSet = getPacketBitset();
    for( int i=0; i<576; i++ ) {
        pack[i/8] |= (packSet[i] << (i%8));
    }
}

void Packet::getPacketByMessage ( char* pack ){
    type.reset();
    dest.reset();
    source.reset();
    ttl.reset();
    length.reset();
    data.reset();
    crc.reset();
    for ( int i=0; i<576; i++ ) {
        if(i/8 < 2){
            type[(i-0)%16] = pack[i/8] & (1<<(i%8)) ;
        }
        else if( (2 <= i/8) && (i/8 < 18)){
            dest[(i-16)%128] = pack[i/8] & (1<<(i%8)) ;
        }
        else if( (18 <= i/8) && (i/8 < 34)){
            source[(i-144)%128] = pack[i/8] & (1<<(i%8)) ;
        }
        else if( (34 <= i/8) && ( i/8 < 38) ){
            ttl[(i-272)%32] = pack[i/8] & (1<<(i%8)) ;
        }
        else if( (38 <= i/8) && ( i/8 < 43) ){
            length[(i-304)%40] = pack[i/8] & (1<<(i%8)) ;
        }
        else if( (43 <= i/8) && (i/8 < 66) ){
            data[(i-344)%184] = pack[i/8] & (1<<(i%8)) ;
        }
        else if( (66 <= i/8) && ( i/8 < 72) ){
            crc[(i-528)%48] = pack[i/8] & (1<<(i%8)) ;
        }
    }
}

void Packet::decTtl(){
    if(ttl.to_ulong()>0)
        ttl = bitset<32>(ttl.to_ulong()-1);
}

void Packet::send(int sockfd) {
    char packbuf[72] = {0};
    getMessageByPacket(packbuf);
    int n = sendto(sockfd, packbuf, 72, 0,NULL,0);
    return; 
}
