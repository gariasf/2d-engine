build:
	g++ -Wall -std=c++17 -I"./libs" src/*.cpp -lSDL2 -llua5.3 -o ./dist/gameengine
run:
	./dist/outgameengine
clean:
	rm ./dist/gameengine