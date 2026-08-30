# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Target executable name
TARGET = main

# Special libraries to link (e.g., -lm for math)
LIBS = -lm -l raylib

# Source files
SRCS = main.c

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Build and run the project
run: all
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)
