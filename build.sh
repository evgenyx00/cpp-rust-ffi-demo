#!/bin/bash
# Build script for C++ Rust FFI Demo

set -e  # Exit on error

echo "=== C++ Rust FFI Demo Build Script ==="
echo "Project location: $(pwd)"
echo ""

# Initialize git if not already initialized
if [ ! -d ".git" ]; then
    echo "Initializing git repository..."
    git init
    git add .
    git commit -m "Initial commit: C++ to Rust FFI demo project"
    echo "✅ Git repository initialized"
else
    echo "✅ Git repository already exists"
fi
echo ""

# Build Rust library
echo "Building Rust library..."
cd rust-lib
cargo build --release
cd ..
echo "✅ Rust library built successfully"
echo ""

# Build with CMake
echo "Building C++ application with CMake..."
# Clean old build directory if CMake cache exists with wrong paths
if [ -f "build/CMakeCache.txt" ]; then
    echo "Cleaning stale CMake cache..."
    rm -rf build
fi
mkdir -p build
cd build
cmake ..
cmake --build .
echo "✅ C++ application built successfully"
echo ""

# Run tests
echo "Running Rust tests..."
cd ../rust-lib
cargo test
cd ..
echo "✅ Tests passed"
echo ""

# Run demo
echo "Running demo..."
echo "================================"
./build/demo
echo "================================"
echo ""

echo "🎉 Build and verification complete!"
echo ""
echo "Project is ready to use at: $(pwd)"
echo ""
echo "Next steps:"
echo "  - Run the demo: ./build/demo"
echo "  - Run tests: cd rust-lib && cargo test"
echo "  - Git status: git status"

