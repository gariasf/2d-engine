build:
	g++ -Wall -std=c++17 -I"./libs" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3 -o ./dist/gameengine
#macos	g++ -Wall -std=c++17 -I"./libs" -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 src/*.cpp -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -o ./dist/gameengine
run:
	./dist/gameengine
clean:
	rm ./dist/gameengine