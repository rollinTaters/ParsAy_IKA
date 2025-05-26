SFML_dir_linux := ../SFML/SFML-2.6.x-linux
SFML_libs := -lsfml-system -lsfml-network

CXX := g++

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj
COMMON_OBJ_DIR := ../common_code/obj

LINUX_LDFLAGS := -Wl,-rpath=./lib:$(SFML_dir_linux)/lib -I$(SFML_dir_linux)/include -L$(SFML_dir_linux)/lib $(SFML_libs)

CXXFLAGS := -Wall -Wextra -std=c++17
LIBS := `pkg-config --cflags --libs opencv4` -I./../common_code/src -I$(SFML_dir_linux)/include -L$(SFML_dir_linux)/lib $(SFML_libs) -lavcodec -lavformat -lavutil -lswscale -lopencv_core -lopencv_highgui -lopencv_imgcodecs


COMMON_OBJS := $(wildcard $(COMMON_OBJ_DIR)/*.o)

# Source files (excluding backups and deprecated files)
SOURCES := $(filter-out \
    $(SRC_DIR)/deprecated_main.cpp \
    $(SRC_DIR)/#% \
    $(SRC_DIR)/%~ \
    , $(wildcard $(SRC_DIR)/*.cpp))

# Identify specific sources for app and test
APP_SRC := $(SRC_DIR)/main.cpp
TEST_SRC := $(SRC_DIR)/receivedTest.cpp
SHARED_SRC := $(filter-out $(APP_SRC) $(TEST_SRC), $(SOURCES))

APP_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(APP_SRC) $(SHARED_SRC))
TEST_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRC) $(SHARED_SRC))

APP_TARGET := $(BIN_DIR)/app
TEST_TARGET := $(BIN_DIR)/test

.PHONY: all clean run test

all: $(APP_TARGET) $(TEST_TARGET)

$(APP_TARGET): $(APP_OBJS) $(COMMON_OBJS) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LIBS) $(LINUX_LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS) $(COMMON_OBJS) | $(BIN_DIR)
	$(CXX) $^ -o $@ $(LIBS) $(LINUX_LDFLAGS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -c $< -o $@ $(LINUX_LDFLAGS)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

run: $(APP_TARGET)
	./$(APP_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

