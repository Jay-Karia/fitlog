TARGET_NAME = fitlog

DIST_DIR = dist
OBJ_DIR = obj
CORE_DIR = core
UTILS_DIR = utils
TARGET = $(DIST_DIR)/$(TARGET_NAME).exe

CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRCS := $(wildcard *.c) $(wildcard $(CORE_DIR)/*.c) $(wildcard $(UTILS_DIR)/*.c)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	@sh -c "mkdir -p $(DIST_DIR)"
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete! Run with ./$(TARGET) or 'make run'"

$(OBJ_DIR)/%.o: %.c
	@echo "Compiling $<..."
	@sh -c "mkdir -p $(@D)"
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "Running..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	@sh -c "rm -rf $(DIST_DIR) $(OBJ_DIR)"

.PHONY: all clean run
