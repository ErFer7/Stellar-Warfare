CC = g++
CFLAGS = -Wall -g

EXECUTABLE = bin/INE5412_OS

default: makedir main

makedir:
	mkdir -p bin

.PHONY: main
main: cpu.o main_class.o main.o debug.o system.o thread.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main_class.o main.o cpu.o debug.o system.o thread.o

cpu.o: src/cpu.cc src/traits.h src/cpu.h
	$(CC) $(CFLAGS) -c src/cpu.cc 

main_class.o: src/main_class.cc src/main_class.h src/cpu.h src/traits.h
	$(CC) $(CFLAGS) -c src/main_class.cc

main.o: src/main.cc src/main_class.h src/cpu.h src/system.h
	$(CC) $(CFLAGS) -c src/main.cc 

debug.o: src/debug.cc src/debug.h src/traits.h
	$(CC) $(CFLAGS) -c src/debug.cc

system.o: src/system.cc src/traits.h
	$(CC) $(CFLAGS) -c src/system.cc

thread.o: src/thread.cc src/thread.h src/traits.h
	$(CC) $(CFLAGS) -c src/thread.cc

clean:
	rm -f *.o bin/INE5412_OS