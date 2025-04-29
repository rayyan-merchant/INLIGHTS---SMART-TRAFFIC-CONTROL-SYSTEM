# CC = gcc
# CFLAGS = -Wall -std=c99 -Iheaders
# OBJ_DIR = obj
# SRC_DIR = src
# BIN_DIR = bin

# # Source files
# SRC = $(SRC_DIR)/main.c $(SRC_DIR)/display.c $(SRC_DIR)/dataset_reader.c
# OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/display.o $(OBJ_DIR)/dataset_reader.o
# BIN = $(BIN_DIR)/main

# # Targets
# all: $(BIN)

# $(BIN): $(OBJ)
# 	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJ_DIR)/*.o $(BIN)

# Compiler and flags
CC       := gcc
CFLAGS   := -Wall -std=c99 -Iheaders
LDFLAGS  := -pthread

# Directories
SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin

# Executable name
TARGET   := traffic_sim

# All your source files
SRCS := \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/display.c \
    $(SRC_DIR)/dataset_reader.c \
    $(SRC_DIR)/data_reader.c \
    $(SRC_DIR)/thread_manager.c \
    $(SRC_DIR)/traffic.c \
    $(SRC_DIR)/utils.c \
    $(SRC_DIR)/logger.c \
    $(SRC_DIR)/controller.c

# Convert source paths to object paths
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: setup_dirs $(BIN_DIR)/$(TARGET)

# Link step
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# Compile step
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure directories exist
.PHONY: setup_dirs
setup_dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Clean up
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(TARGET)


