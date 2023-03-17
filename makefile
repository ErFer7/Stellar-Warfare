CC := g++
CFLAGS := -O3
DBGFLAGS := -g

BIN_PATH := bin
SRC_PATH := src
DBG_PATH := debug

TARGET_NAME := INE5412OS
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))

default: makedir all

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $@

$(TARGET_DEBUG): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(DBGFLAGS) -o $@

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: clean
clean:
	@echo CLEAN $(TARGET)
	@echo CLEAN $(TARGET_DEBUG)
	@rm -f $(TARGET)
	@rm -f $(TARGET_DEBUG)
