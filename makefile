CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra

# Directories
SRC_DIR = src
HEADER_DIR = header
BUILD_DIR = build

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HEADER_DIR)/*.h)

# Object files and executable inside build/
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
EXEC = $(BUILD_DIR)/main

# Default target
all: $(EXEC)

# Linking the executable
$(EXEC): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compiling object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean executables and object files
clean:
	rm -rf $(BUILD_DIR)

# Run the assembler
run: all
	./$(EXEC)