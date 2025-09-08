TARGET_NAME = fitlog

DIST_DIR = dist
OBJ_DIR = obj
SRC_DIR = src
TARGET = $(DIST_DIR)/$(TARGET_NAME)

CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	-@mkdir $(DIST_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete! Run with ./$(TARGET) or 'make run'"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	-@mkdir $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "Running..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
	-if exist $(DIST_DIR) rd /s /q $(DIST_DIR)
	-if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)
else
	-rm -rf $(DIST_DIR) $(OBJ_DIR)
endif

.PHONY: all clean run

