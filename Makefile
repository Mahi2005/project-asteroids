# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# Target executable name
TARGET = main

# Special libraries to link (e.g., -lm for math)
LIBS = -lm -l raylib

# Source files
SRCS = src/main.c src/asteroids.c src/bullets.c src/ship.c src/utils.c

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
