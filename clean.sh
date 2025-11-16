#!/bin/bash
# Clean script for C++ Rust FFI Demo

echo "=== Cleaning Build Artifacts ==="
echo ""

# Clean CMake build directory
if [ -d "build" ]; then
    echo "Removing CMake build directory..."
    rm -rf build
    echo "✅ CMake build cleaned"
else
    echo "✅ No CMake build directory to clean"
fi
echo ""

# Clean Rust build artifacts
if [ -d "rust-lib/target" ]; then
    echo "Removing Rust target directory..."
    cd rust-lib
    cargo clean
    cd ..
    echo "✅ Rust build cleaned"
else
    echo "✅ No Rust build artifacts to clean"
fi
echo ""

echo "🎉 Clean complete!"
echo ""
echo "Next steps:"
echo "  - Rebuild: ./build.sh"
echo "  - Or build manually with: cmake and cargo"

