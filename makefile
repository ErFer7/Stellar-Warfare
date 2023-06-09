CC = g++
CFLAGS = -Wall -g
OS_SRC = os/src
OS_LIB = os/include
GAME_SRC = src
GAME_LIB = include
OBJ_DIR = obj
SRC := $(wildcard $(OS_SRC)/*.cc)
OBJ := $(patsubst $(OS_SRC)/%,$(OBJ_DIR)/%,$(SRC:.cc=.o))
COMPILE_OBJ = $(CC) $(CFLAGS) -c $< -o $@

EXECUTABLE = "bin/Spaceship game"

.PHONY: default
default: makedir main

makedir:
	mkdir -p bin
	mkdir -p $(OBJ_DIR)

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ)

$(OBJ_DIR)/cpu.o: $(OS_SRC)/cpu.cc $(OS_LIB)/cpu.h $(OS_LIB)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/main_class.o: $(OS_SRC)/main_class.cc $(OS_LIB)/main_class.h $(OS_LIB)/cpu.h $(OS_LIB)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/main.o: $(OS_SRC)/main.cc $(OS_LIB)/main_class.h $(OS_LIB)/cpu.h $(OS_LIB)/system.h $(OS_LIB)/semaphore.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/debug.o: $(OS_SRC)/debug.cc $(OS_LIB)/debug.h $(OS_LIB)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/system.o: $(OS_SRC)/system.cc $(OS_LIB)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/thread.o: $(OS_SRC)/thread.cc $(OS_LIB)/thread.h $(OS_LIB)/traits.h
	$(COMPILE_OBJ)
	
$(OBJ_DIR)/semaphore.o: $(OS_SRC)/semaphore.cc $(OS_LIB)/semaphore.h $(OS_LIB)/traits.h
	$(COMPILE_OBJ)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/* "bin/Spaceship game"

