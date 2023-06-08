CC = g++
CFLAGS = -Wall -g
OS_SRC = os/src
OS_INCLUDE = os/include

EXECUTABLE = "bin/Spaceship game"

.PHONY: default
default: makedir main cleanobj

makedir:
	mkdir -p bin

main: cpu.o main_class.o main.o debug.o system.o thread.o semaphore.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main_class.o main.o cpu.o debug.o system.o thread.o semaphore.o

cpu.o: $(OS_SRC)/cpu.cc $(OS_INCLUDE)/cpu.h $(OS_INCLUDE)/traits.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/cpu.cc 

main_class.o: $(OS_SRC)/main_class.cc $(OS_INCLUDE)/main_class.h $(OS_INCLUDE)/cpu.h $(OS_INCLUDE)/traits.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/main_class.cc

main.o: $(OS_SRC)/main.cc $(OS_INCLUDE)/main_class.h $(OS_INCLUDE)/cpu.h $(OS_INCLUDE)/system.h $(OS_INCLUDE)/semaphore.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/main.cc 

debug.o: $(OS_SRC)/debug.cc $(OS_INCLUDE)/debug.h $(OS_INCLUDE)/traits.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/debug.cc

system.o: $(OS_SRC)/system.cc $(OS_INCLUDE)/traits.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/system.cc

thread.o: $(OS_SRC)/thread.cc $(OS_INCLUDE)/thread.h $(OS_INCLUDE)/traits.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/thread.cc
	
semaphore.o: $(OS_SRC)/semaphore.cc $(OS_INCLUDE)/semaphore.h $(OS_INCLUDE)/traits.h
	$(CC) $(CFLAGS) -c $(OS_SRC)/semaphore.cc

.PHONY: clean
clean:
	rm -f *.o "bin/Spaceship game"

.PHONY: cleanobj
cleanobj:
	rm -f *.o
