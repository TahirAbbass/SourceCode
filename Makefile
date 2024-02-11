CC := g++
CFLAGS := -std=c++17 -pthread
LFLAGS := -lsqlite3 -lgtest -lgmock
TARGET := chat_server
TEST_TARGET := MyTests

INC_DIR := inc
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

INC := -I$(INC_DIR)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(LFLAGS)

$(BUILD_DIR):
	mkdir -p $@


TEST_SRC := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ := $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRC)) $(BUILD_DIR)/chat_server.o $(BUILD_DIR)/database_manager.o

$(BUILD_DIR)/$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(CFLAGS) $(INC) $^ -o $@ $(LFLAGS)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(SRC_DIR)/chat_server.cpp $(SRC_DIR)/database_manager.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@ $(LFLAGS) -lgtest

test: $(BUILD_DIR)/$(TEST_TARGET)
	./$(BUILD_DIR)/$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
