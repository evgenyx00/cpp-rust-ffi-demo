# Makefile for building C++ Rust FFI demo
# Alternative to CMake for simpler builds

# Compiler settings
CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
# Note: We don't use -I for includes to avoid header search path conflicts
# The source file uses relative paths instead
INCLUDES :=

# Rust library settings
RUST_LIB_DIR := rust-lib/target/release
RUST_LIB := $(RUST_LIB_DIR)/librust_lib.a

# Platform-specific settings
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    LDFLAGS := -L$(RUST_LIB_DIR) -lrust_lib -framework Security -framework CoreFoundation -lpthread -ldl
else ifeq ($(UNAME_S),Linux)
    LDFLAGS := -L$(RUST_LIB_DIR) -lrust_lib -lpthread -ldl -lm
else
    LDFLAGS := -L$(RUST_LIB_DIR) -lrust_lib -lws2_32 -luserenv -lbcrypt
endif

# Source and output
SRC := cpp-app/main.cpp
TARGET := demo

.PHONY: all clean rust-lib run

all: $(TARGET)

# Build Rust library
rust-lib:
	@echo "Building Rust library..."
	cd rust-lib && cargo build --release

# Build C++ executable
$(TARGET): rust-lib $(SRC)
	@echo "Building C++ application..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(LDFLAGS) -o $(TARGET)

# Run the demo
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET)
	cd rust-lib && cargo clean

# Help target
help:
	@echo "Available targets:"
	@echo "  make         - Build the demo"
	@echo "  make run     - Build and run the demo"
	@echo "  make clean   - Clean all build artifacts"
	@echo "  make rust-lib - Build only the Rust library"

