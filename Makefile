# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c17 -g

# Output directories
BIN_DIR = bin
OBJ_DIR = build

# Source files
SRC_DIR = src
SRC = $(wildcard src/*.c)

# Object files
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Output binary
BIN = $(BIN_DIR)/clox

.PHONY: all test clean

# Default target
all: clean $(BIN) test

# Link object files into final binary
$(BIN): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(BIN)

# Compile each C file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run test
test: $(BIN)
	./$(BIN)

# Clean build artifacts via CLI parameter
clean:
	@if [ -f $(BIN) ]; then								\
		rm -rf $(OBJ_DIR) $(BIN_DIR) compile_commands.json;	\
	fi

