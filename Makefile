build:
	clang++ -Wall -Wextra -std=c++17 ./src/*.cpp -I ./include -L ./lib -lGLEW -lglfw3 -lassimp.5 -framework IOKit -framework Cocoa -framework OpenGL -o demo.out
run:
	DYLD_LIBRARY_PATH="./lib" ./demo.out
clean:
	rm -f ./demo.out
