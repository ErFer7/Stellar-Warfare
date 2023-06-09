CC = g++
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system -lm -lpng
CFLAGS = -g -Wall
OS_SRC_DIR = os/src
OS_LIB_DIR = os/include
GAME_SRC_DIR = src
GAME_LIB_DIR = include
OBJ_DIR = obj
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
$(OBJ_DIR)/cpu.o: $(OS_SRC_DIR)/cpu.cc $(OS_LIB_DIR)/cpu.h $(OS_LIB_DIR)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/debug.o: $(OS_SRC_DIR)/debug.cc $(OS_LIB_DIR)/debug.h $(OS_LIB_DIR)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/semaphore.o: $(OS_SRC_DIR)/semaphore.cc $(OS_LIB_DIR)/semaphore.h $(OS_LIB_DIR)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/system.o: $(OS_SRC_DIR)/system.cc $(OS_LIB_DIR)/traits.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/thread.o: $(OS_SRC_DIR)/thread.cc $(OS_LIB_DIR)/thread.h $(OS_LIB_DIR)/traits.h
	$(COMPILE_OBJ)

# Game
$(OBJ_DIR)/main.o: $(GAME_SRC_DIR)/main.cc $(GAME_LIB_DIR)/game.h $(OS_LIB_DIR)/system.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/bullet.o: $(GAME_SRC_DIR)/bullet.cc $(GAME_LIB_DIR)/bullet.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/enemy.o: $(GAME_SRC_DIR)/enemy.cc $(GAME_LIB_DIR)/enemy.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/entity.o: $(GAME_SRC_DIR)/entity.cc $(GAME_LIB_DIR)/entity.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/game.o: $(GAME_SRC_DIR)/game.cc $(GAME_LIB_DIR)/game.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/input.o: $(GAME_SRC_DIR)/input.cc $(GAME_LIB_DIR)/input.h $(GAME_LIB_DIR)/game.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/player.o: $(GAME_SRC_DIR)/player.cc $(GAME_LIB_DIR)/player.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/renderer.o: $(GAME_SRC_DIR)/renderer.cc $(GAME_LIB_DIR)/renderer.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/scene.o: $(GAME_SRC_DIR)/scene.cc $(GAME_LIB_DIR)/scene.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/space_matrix.o: $(GAME_SRC_DIR)/space_matrix.cc $(GAME_LIB_DIR)/space_matrix.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/spaceship.o: $(GAME_SRC_DIR)/spaceship.cc $(GAME_LIB_DIR)/spaceship.h
	$(COMPILE_OBJ)

$(OBJ_DIR)/user_interface.o: $(GAME_SRC_DIR)/user_interface.cc $(GAME_LIB_DIR)/user_interface.h
	$(COMPILE_OBJ)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/* "bin/Spaceship game"

