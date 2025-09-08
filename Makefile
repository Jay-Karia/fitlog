# The name of our final program
TARGET_NAME = fitlog

# Directories
DIST_DIR = dist
TARGET = $(DIST_DIR)/$(TARGET_NAME)

# The C compiler and flags
# -Iinclude tells the compiler to look in the 'include' folder for header files
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

# The source file for the main function
SRC = src/main.c

# Default rule: build the program
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(SRC)
	@echo "Compiling..."
	@mkdir $(DIST_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Done! Run with ./$(TARGET)"

# Command to run the program
run:
	./$(TARGET)

# A rule to clean up the compiled files
clean:
	@echo "Cleaning up..."
ifeq ($(OS),Windows_NT)
	-if exist $(DIST_DIR) rd /s /q $(DIST_DIR)
else
	-rm -rf $(DIST_DIR)
endif

# Phony targets don't represent actual files
.PHONY: all clean run

