# .PHONY: build
# build:
# 	clang++ -Wall -Wextra -std=c++17 ./src/ex3d/*.cpp ./src/*.cpp -I ./include -L ./lib -lGLEW -lglfw3 -lassimp.5 -framework IOKit -framework Cocoa -framework OpenGL -o demo.out
# run:
# 	DYLD_LIBRARY_PATH="./lib" ./demo.out
# clean:
# 	rm -f ./demo.out

# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17 -g -ferror-limit=1 -Wfatal-errors -I ./include
LDFLAGS = -L ./lib -lGLEW -lglfw3 -lassimp.5 -framework IOKit -framework Cocoa -framework OpenGL
#-fsanitize=address -fsanitize=undefined

# Folders and files
SRC_DIR = ./src
EX3D_DIR = $(SRC_DIR)/ex3d
OBJ_DIR = ./build

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(EX3D_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp)) \
            $(patsubst $(EX3D_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(EX3D_DIR)/*.cpp))

# Output file
TARGET = demo.out

# Default build target
.PHONY: build
build: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(EX3D_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
.PHONY: run
run: build
	DYLD_LIBRARY_PATH="./lib" ./$(TARGET)

# Clean the build folder
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
