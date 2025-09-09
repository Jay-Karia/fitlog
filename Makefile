# --- Shell Configuration ---
# Force make to use the 'sh' shell, which understands commands like 'mkdir -p'
# This is crucial for cross-platform compatibility on Windows.
SHELL := sh.exe

# --- Configuration ---
TARGET_NAME = fitlog

# --- Directories ---
DIST_DIR = dist
OBJ_DIR = obj
SRC_DIR = src
# The final executable path
TARGET = $(DIST_DIR)/$(TARGET_NAME)

# --- Compiler and Flags ---
# -Iinclude tells the compiler to look in the 'include' folder for header files
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# --- File Discovery ---
# Automatically find all .c files in the src directory and its subdirectories using wildcard.
# This is more portable than using the 'find' command.
SRCS := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*/*.c)
# Generate corresponding object file names, placing them in the OBJ_DIR
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- Main Rules ---

# Default rule: build the program
all: $(TARGET)

# Rule to link the final executable from all the object files
$(TARGET): $(OBJS)
	@echo "Linking..."
	-@mkdir -p $(DIST_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build complete! Run with ./$(TARGET) or 'make run'"

# --- Compilation Rule ---

# Pattern rule to compile any .c file from the src tree into a .o file in the obj tree
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	-@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Utility Rules ---

# Command to run the program
run: all
	@echo "Running..."
	./$(TARGET)

# A rule to clean up all compiled files
clean:
	@echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
	-if exist $(DIST_DIR) rd /s /q $(DIST_DIR)
	-if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)
else
	-rm -rf $(DIST_DIR) $(OBJ_DIR)
endif

# Phony targets don't represent actual files
.PHONY: all clean run

