# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I src

# Source and build directories
SRC_DIR := src
OBJ_DIR := obj
OBJ_DIR_PARSING := obj/parsing
BIN := westshell

# Source files and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN)

# Linking
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR_PARSING)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# Run the program
run: $(BIN)
	./$(BIN)

.PHONY: all clean run
