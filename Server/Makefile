include ../Makefile

all: server.out
server.out: main.o
	$(CC) *.o $(CLIBS) -o server.out 
main.o: main.cpp ServerManager.o
	$(CC) $(CFLAGS) -c main.cpp
ServerManager.o: ServerManager.cpp ServerManager.h Firewall.o
	$(CC) $(CFLAGS) -c ServerManager.cpp
Firewall.o: Firewall.cpp Firewall.h
	$(CC) $(CFLAGS) -c Firewall.cpp

clean:
	rm *.o *.out
