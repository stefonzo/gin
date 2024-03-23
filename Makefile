# Makefile for compiling C++ code with SDL2, SDL2_ttf, SDL2_image, and GLM

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -I/usr/include/SDL2 -I/home/stefonzo/glm-master/glm # Adjust the path to GLM headers

# Linker flags
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image

# Source directory
SRC_DIR = src

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SRC_FILES))

# Executable name
EXECUTABLE = test

# Build rule
debug: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Object files rule
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)
