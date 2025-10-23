# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2

# Project directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR_WIN := $(HOME)/raylib_tech/raylib/raylib

# Cross-compiler for Windows
CXX_WIN := x86_64-w64-mingw32-g++
CXXFLAGS_WIN := -I$(LIB_DIR_WIN)/include -Iinclude -std=c++17 --static -Wall -Wextra -O2 -static-libgcc -static-libstdc++ -mwindows
# Other shit for omp
CXXFLAGS_WIN += -I/home/linuxbrew/.linuxbrew/opt/libomp/include
LDFLAGS_WIN += -L/home/linuxbrew/.linuxbrew/opt/libomp/lib -lomp


# Debug flags
DEBUG_FLAGS := -fsanitize=address,undefined -g

# Raylib configuration (system installation)
RAYLIB_FLAGS_LINUX := $(shell pkg-config --libs --cflags raylib)
RAYLIB_FLAGS_WIN := -L$(LIB_DIR_WIN) -lraylib -lopengl32 -lgdi32 -lwinmm

# Project name
PROJECT_NAME := rocky_the_pet

# Output executable names
TARGET_LINUX := $(BIN_DIR)/$(PROJECT_NAME)
TARGET_WINDOWS := $(BIN_DIR)/$(PROJECT_NAME).exe
TARGET_LINUX_DEBUG := $(BIN_DIR)/$(PROJECT_NAME)_debug

# Find all .cpp and .cxx files
SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cxx)
# Generate corresponding object files in obj/
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS)) $(patsubst $(SRC_DIR)/%.cxx, $(OBJ_DIR)/%.o, $(OBJS))


# Default rule: Build debug
all: clean debug

# === RELEASE ===
release: linux windows
# === Linux Build ===
linux: $(TARGET_LINUX)

$(TARGET_LINUX): $(OBJS) | $(BIN_DIR)
	@echo "Linking Linux executable..."
	$(CXX) $(CXXFLAGS) $^ -o $@ $(RAYLIB_FLAGS_LINUX)

# === Windows Cross-Compile ===
windows: $(TARGET_WINDOWS)

$(TARGET_WINDOWS): $(SRCS)
	@echo "Cross-compiling Windows executable..."
	$(CXX_WIN) $(CXXFLAGS_WIN) $^ -o $@ $(RAYLIB_FLAGS_WIN)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


# === DEBUG MODE ===
debug: $(TARGET_LINUX_DEBUG)

$(TARGET_LINUX_DEBUG): $(OBJS) | $(BIN_DIR)
	@echo "Linking Linux executable with debugging flags..."
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $^ -o $@ $(RAYLIB_FLAGS_LINUX)

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/$(PROJECT_NAME)
	rm -rf $(BIN_DIR)/$(PROJECT_NAME)_debug
	rm -rf $(BIN_DIR)/$(PROJECT_NAME).exe

# Phony targets
.PHONY: all linux windows clean debug release
