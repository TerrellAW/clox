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

.PHONY: default all test clean bear

# Default target
default: clean $(BIN)

# Run all targets
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
	@if command -v rlwrap >/dev/null 2>&1; then		\
		rlwrap ./$(BIN);							\
	else											\
		./$(BIN);									\
	fi

# Clean build artifacts and configuration
clean:
	@if [ -f $(BIN) ]; then								\
		rm -rf $(OBJ_DIR) $(BIN_DIR) compile_commands.json;	\
	fi

# Make with bear configuration tool
bear:
	@if [ command -v bear >/dev/null 2>&1 ]; then	\
		echo "Bear clang configuration generator not installed. Try installing it from your package manager or GitHub: https://github.com/rizsotto/bear";			  \
	else											\
		make clean && bear -- make;					\
	fi
