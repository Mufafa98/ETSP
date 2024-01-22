# Compiler and flags
CC := g++
CFLAGS := -Wall -pthread -std=c++17

# Libraries to link against
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Source code directories
SRC_DIR := ./src
INC_DIR := ./include

# Object and executable directories
OBJ_DIR := ./bin/obj
EXE_DIR := ./bin/exe

# Target executable name
TARGET := ETSP

# Find all source files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate a list of object files in the object directory based on source files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Target to build the executable and run it
all: build run

# Target to build the executable
build: $(EXE_DIR)/$(TARGET)

# Rule to link object files into the final executable
$(EXE_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(EXE_DIR)
	@$(CC) $(CFLAGS) $(LIBS) -I$(INC_DIR) $^ -o $@

# Rule to compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Target to run the executable
run:
	@./$(EXE_DIR)/$(TARGET)

# Target to clean up generated files
clean:
	@rm -rf $(OBJ_DIR) $(EXE_DIR)

# Target to count lines in .cpp and .hpp files
linecount:
	@echo "Lines in .cpp and .hpp files:"
	@find $(SRC_DIR) $(INC_DIR) -name "*.cpp" -or -name "*.hpp" | xargs wc -l

# Declare targets that are not files to avoid conflicts with file names
.PHONY: all build run clean linecount
