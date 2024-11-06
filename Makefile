# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2
DEBUG_FLAGS := -g -DDEBUG

# Directories
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

# Source and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target executable
TARGET := $(BUILD_DIR)/isomake

# Default target
.PHONY: all
all: dirs $(TARGET)

# Create necessary directories
.PHONY: dirs
dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

# Build the target executable
$(TARGET): $(OBJ)
	@echo "Linking $@..."
	@$(CC) $(OBJ) -o $@
	@echo "Build complete! Binary located at: $(TARGET)"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Clean build files
.PHONY: clean
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete!"

# Install the executable to system
.PHONY: install
install: all
	@echo "Installing ISOMake..."
	@install -m 755 $(TARGET) /usr/local/bin
	@echo "Installation complete!"

# Uninstall the executable from system
.PHONY: uninstall
uninstall:
	@echo "Uninstalling ISOMake..."
	@rm -f /usr/local/bin/isomake
	@echo "Uninstallation complete!"

# Help target
.PHONY: help
help:
	@echo "ISOMake Makefile Help"
	@echo "--------------------------------------------------------------------------------"
	@echo "Available targets:"
	@echo "  all      : Build the ISOMake executable (default)"
	@echo "  debug    : Build with debug symbols and DEBUG flag"
	@echo "  clean    : Remove all build files"
	@echo "  install  : Install ISOMake to /usr/local/bin"
	@echo "  uninstall: Remove ISOMake from /usr/local/bin"
	@echo "  help     : Display this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make        - Build the executable"
	@echo "  make debug  - Build with debug information"
	@echo "  make clean  - Clean build files"
	@echo "  make install - Install to system"
	@echo ""
	@echo "The compiled binary will be located at: $(TARGET)"
	@echo "--------------------------------------------------------------------------------"

# Additional development helpers
.PHONY: format
format:
	@echo "Formatting source code..."
	@find $(SRC_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i
	@echo "Formatting complete!"

.PHONY: check
check:
	@echo "Running static analysis..."
	@cppcheck --enable=all $(SRC_DIR)
	@echo "Static analysis complete!"

# Version information
.PHONY: version
version:
	@echo "ISOMake v1.0"
	@echo "Compiled with $(CC) $(CFLAGS)"