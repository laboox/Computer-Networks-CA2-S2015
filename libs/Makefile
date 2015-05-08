include ../Makefile

mylib.a: Packet.o
	ar -cvr -o mylib.a Packet.o
Packet.o: Packet.cpp Packet.h crc.h
	g++ -c Packet.cpp
clean: 
	rm *.o *.a
