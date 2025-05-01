# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -Iheaders
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/display.c $(SRC_DIR)/data_reader.c \
       $(SRC_DIR)/logger.c $(SRC_DIR)/traffic.c $(SRC_DIR)/thread_manager.c \
       $(SRC_DIR)/utils.c $(SRC_DIR)/controller.c

# Object files
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/display.o $(OBJ_DIR)/data_reader.o \
       $(OBJ_DIR)/logger.o $(OBJ_DIR)/traffic.o $(OBJ_DIR)/thread_manager.o \
       $(OBJ_DIR)/utils.o $(OBJ_DIR)/controller.o

# Executable
TARGET = $(BIN_DIR)/main

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/display.o: $(SRC_DIR)/display.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/data_reader.o: $(SRC_DIR)/data_reader.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/logger.o: $(SRC_DIR)/logger.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/traffic.o: $(SRC_DIR)/traffic.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/thread_manager.o: $(SRC_DIR)/thread_manager.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/controller.o: $(SRC_DIR)/controller.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean

# CC = gcc
# CFLAGS = -Wall -std=c99 -Iheaders
# OBJDIR = obj
# SRCDIR = src
# BINDIR = bin

# all: $(BINDIR)/main

# $(BINDIR)/main: $(OBJDIR)/main.o $(OBJDIR)/display.o $(OBJDIR)/data_reader.o
# 	$(CC) $(CFLAGS) $^ -o $@ -lpthread

# $(OBJDIR)/main.o: $(SRCDIR)/main.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJDIR)/display.o: $(SRCDIR)/display.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJDIR)/data_reader.o: $(SRCDIR)/data_reader.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJDIR)/*.o $(BINDIR)/main

