build:
	g++ -Wall -std=c++17 -I"./libs" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3 -o ./dist/gameengine
run:
	./dist/gameengine
clean:
	rm ./dist/gameengine