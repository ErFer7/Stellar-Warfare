CC = g++
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -lm -lpng
CFLAGS = -g -Wall
OS_SRC_DIR = os/src
OS_LIB_DIR = os/include
GAME_SRC_DIR = src
GAME_LIB_DIR = include
OBJ_DIR = obj
LIB = $(wildcard $(OS_LIB_DIR)/*.h) $(wildcard $(GAME_LIB_DIR)/*.h)
OS_SRC := $(wildcard $(OS_SRC_DIR)/*.cc)
GAME_SRC := $(wildcard $(GAME_SRC_DIR)/*.cc)
OBJ := $(patsubst $(OS_SRC_DIR)/%, $(OBJ_DIR)/%,$(OS_SRC:.cc=.o)) $(patsubst $(GAME_SRC_DIR)/%, $(OBJ_DIR)/%,$(GAME_SRC:.cc=.o))
COMPILE_OBJ = $(CC) $(CFLAGS) -c $< -o $@

EXECUTABLE = "bin/Spaceship game"

.PHONY: default
default: makedir main

makedir:
	mkdir -p bin
	mkdir -p $(OBJ_DIR)

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJ) $(LDLIBS)

# OS
$(OBJ_DIR)/%.o: $(OS_SRC_DIR)/%.cc $(LIB)
	$(COMPILE_OBJ)

# Game
$(OBJ_DIR)/%.o: $(GAME_SRC_DIR)/%.cc $(LIB)
	$(COMPILE_OBJ)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/* "bin/Spaceship game"
