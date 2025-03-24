CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra

# Source and header files
SRC_DIR = src
HEADER_DIR = header
SRCS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(HEADER_DIR)/*.h)
OBJS = $(SRCS:.c=.o)
EXEC = main

# Default target
all: $(EXEC)

# Linking the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compiling object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean executables and object files
clean:
	rm -f $(OBJS) $(EXEC)

# Run the assembler
run: all
	./$(EXEC)