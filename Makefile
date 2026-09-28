# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -I"D:/Downloads/raylib-6.0_win64_mingw-w64/raylib-6.0_win64_mingw-w64/include"
LDFLAGS = -L"D:/Downloads/raylib-6.0_win64_mingw-w64/raylib-6.0_win64_mingw-w64/lib"

# Target executable name
TARGET = main.exe

# Special libraries to link
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lm

# Source files
SRCS = src/settings.c src/enemy_ship.c src/savegame.c src/highscores.c src/menu.c src/main.c src/asteroids.c src/bullets.c src/ship.c src/utils.c

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Build and run the project
run: all
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)