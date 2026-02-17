# --- Compiler and Flags ---
CXX         := g++
CXXFLAGS    := -std=c++20 -Wall -Wextra -Iinclude -Ilib/raylib/src
LDFLAGS     := -Lbin -static-libgcc -static-libstdc++ -Wl,-Bdynamic -lraylib -Wl,-rpath,'$$ORIGIN'
WINCXX      := x86_64-w64-mingw32-g++
WIN_LDFLAGS := -Lbin -static-libgcc -static-libstdc++ -Wl,-Bdynamic -lraylib -Wl,-Bstatic -lpthread

# --- Directories ---
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
WIN_OBJ_DIR := $(OBJ_DIR)/win

# --- Target name ---
TARGET_NAME := rocky-the-pet-rock

# --- Files ---
SOURCES  	:= $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*.cxx)
OBJECTS  	:= $(SOURCES:$(SRC_DIR)/%=$(OBJ_DIR)/%.o)
WIN_OBJECTS := $(SOURCES:$(SRC_DIR)/%=$(WIN_OBJ_DIR)/%.o)

# --- Compile modes ---
.PHONY: all debug release clean

all: debug

# Debug config
debug: TARGET := $(BIN_DIR)/$(TARGET_NAME)_debug_exe
debug: CXXFLAGS += -g -O0 -fsanitize=address
debug: LDFLAGS  += -fsanitize=address
debug: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Debug Build Success!"

# Release config
release: TARGET := $(BIN_DIR)/$(TARGET_NAME)_exe
release: CXXFLAGS += -O2
release: LDFLAGS  += -s
release: $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build Success!"

# Release windows config
windows: CXX := $(WINCXX)
windows: CXXFLAGS += -O2
windows: $(WIN_LDFLAGS) += -s
windows: $(WIN_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(WINCXX) $(WIN_OBJECTS) -o $(BIN_DIR)/$(TARGET_NAME).exe $(WIN_LDFLAGS)
	@echo "Windows Build Success!"

# --- Build Rules for Objects ---

# Standard Linux Rules
$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.cc.o: $(SRC_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.cxx.o: $(SRC_DIR)/%.cxx
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Specific Windows Rules
$(WIN_OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(WINCXX) $(CXXFLAGS) -c $< -o $@

$(WIN_OBJ_DIR)/%.cc.o: $(SRC_DIR)/%.cc
	@mkdir -p $(dir $@)
	$(WINCXX) $(CXXFLAGS) -c $< -o $@

$(WIN_OBJ_DIR)/%.cxx.o: $(SRC_DIR)/%.cxx
	@mkdir -p $(dir $@)
	$(WINCXX) $(CXXFLAGS) -c $< -o $@


# --- Cleanup ---
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/win/*.o
	rm -f $(BIN_DIR)/*exe
	@echo "Cleanup done!"