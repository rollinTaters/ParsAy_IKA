ONNXROOT := ./3rdparties/onnxruntime-linux-x64-1.21.0
CXX := g++

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

TARGET := $(BIN_DIR)/app
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CXXFLAGS := -Wall -Wextra -std=c++17
LIBS := -L$(ONNXROOT)/lib `pkg-config --cflags --libs opencv4` -lQt6Core -lQt6Gui -lQt6Widgets -lonnxruntime -I$(ONNXROOT)/include


all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(LIBS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


run: $(TARGET)
	LD_LIBRARY_PATH=$(ONNXROOT)/lib:$$LD_LIBRARY_PATH ./$(TARGET)

test: $(TARGET)
	./$(TARGET) test 


# Clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)




