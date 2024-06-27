CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs"
INCLUDE_PATH_MACOS = -I"./libs" -I/opt/homebrew/include -I/opt/homebrew/include/SDL2
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
LINKER_FLAGS_MACOS =  -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OUT_PATH = ./dist/gameengine

build:
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o $(OUT_PATH)
#	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH_MACOS) $(SRC_FILES) $(LINKER_FLAGS_MACOS) -o $(OUT_PATH)
run:
	$(OUT_PATH)
clean:
	rm $(OUT_PATH)