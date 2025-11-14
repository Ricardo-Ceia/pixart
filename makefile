CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SDL_CFLAGS = $(shell sdl2-config --cflags) $(shell pkg-config --cflags SDL2_ttf)
SDL_LIBS = $(shell sdl2-config --libs) $(shell pkg-config --libs SDL2_ttf)

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/grid.c $(SRC_DIR)/pixel.c $(SRC_DIR)/renderer.c $(SRC_DIR)/fps.c $(SRC_DIR)/colorpicker.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
EXECUTABLE = $(BUILD_DIR)/pixel_art

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(SDL_LIBS) -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run
