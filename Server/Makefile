include ../Makefile

all: server.out
server.out: main.o
	$(CC) *.o $(CLIBS) -o server.out 
main.o: main.cpp ServerManager.o
	$(CC) $(CFLAGS) -c main.cpp
ServerManager.o: ServerManager.cpp ServerManager.h
	$(CC) $(CFLAGS) -c ServerManager.cpp
	
clean:
	rm *.o *.out
