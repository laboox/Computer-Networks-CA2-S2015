include ../Makefile

all: Switch.out
Switch.out: main.o
	$(CC) *.o $(CLIBS) -o Switch.out 
main.o: main.cpp Switch.o
	$(CC) $(CFLAGS) -c main.cpp
Switch.o: Switch.cpp Switch.h
	$(CC) $(CFLAGS) -c Switch.cpp
Switch.o: Switch.cpp Switch.h
	$(CC) $(CFLAGS) -c Switch.cpp
	
clean:
	rm *.o *.out
