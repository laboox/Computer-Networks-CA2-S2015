include ../Makefile

all: service_provider.out
service_provider.out: main.o
	$(CC) *.o $(CLIBS) -o service_provider.out 
main.o: main.cpp ServiceManager.o
	$(CC) $(CFLAGS) -c main.cpp
ServiceManager.o: ServiceManager.cpp ServiceManager.h
	$(CC) $(CFLAGS) -c ServiceManager.cpp
	
clean:
	rm *.o *.out
