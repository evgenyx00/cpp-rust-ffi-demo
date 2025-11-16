# Project Summary: C++ to Rust FFI Demo

## What Was Created

This project demonstrates best practices for calling Rust code from C++ using Foreign Function Interface (FFI). The project has been successfully built and tested on macOS.

## Project Structure

```
cpp-rust-ffi-demo/
├── rust-lib/                      # Rust library with FFI
│   ├── src/lib.rs                # Rust implementation with FFI functions
│   ├── include/rust_lib.h        # C header for FFI interface
│   └── Cargo.toml                # Rust configuration (staticlib + cdylib)
├── cpp-app/
│   └── main.cpp                  # C++ application demonstrating usage
├── CMakeLists.txt                # CMake build configuration (RECOMMENDED)
├── Makefile                      # Alternative build (Linux only)
├── README.md                     # Comprehensive documentation
└── .gitignore                    # Excludes build artifacts
```

## Key Features Implemented

### 1. Rust Library (`rust-lib/src/lib.rs`)
- **C-compatible structs** using `#[repr(C)]`
- **FFI functions** with `#[no_mangle]` and `extern "C"`
- **Safe pointer handling** with null checks
- **String interop** using `CStr` for safe C string handling
- **Comprehensive safety documentation**
- **Unit tests** to verify functionality

### 2. C Header (`rust-lib/include/rust_lib.h`)
- **Cross-platform compatibility** (C and C++)
- **Proper type definitions** without causing header conflicts
- **extern "C"** guards for C++ compatibility
- **Well-documented functions**

### 3. C++ Application (`cpp-app/main.cpp`)
- **RAII pattern** for automatic resource cleanup
- **Multiple examples** demonstrating different use cases:
  - Simple greet function
  - Adult/minor person processing
  - BMI category calculation
  - Using `std::string` with C FFI
- **Modern C++17** features

### 4. Build System
- **CMake** (recommended): Handles cross-platform builds correctly
- **Makefile**: Simple alternative for Linux
- **Automatic Rust library building** before C++ compilation
- **Proper linking** of platform-specific libraries

## API Overview

### `process_person(const Person* person) -> PersonInfo`
Processes a Person struct and returns computed information:
- Determines if person is an adult (18+)
- Calculates BMI category (simplified)
- Returns name length

### `greet_person(const char* name) -> size_t`
Prints a greeting message and returns the name length.

### Data Structures

**Person** (input):
- `age`: unsigned int
- `height`: double (in meters)
- `name`: const char pointer

**PersonInfo** (output):
- `is_adult`: bool
- `bmi_category`: unsigned char (0=underweight, 1=normal, 2=overweight)
- `name_length`: unsigned long

## Build and Run

### Quick Start (CMake - Recommended)

```bash
cd /Users/evgeny/cpp-rust-ffi-demo
mkdir build && cd build
cmake ..
cmake --build .
./demo
```

### Expected Output

```
C++ calling Rust FFI Demo

--- Example 1: Simple Greeting ---
Hello from Rust, Alice!
Returned name length: 5

--- Example 2: Adult Person ---
=== Person Information ===
Name: Bob Johnson
Name length: 11
Is adult: Yes
BMI category: Normal
=========================

[... more examples ...]

Demo completed successfully!
```

## Best Practices Demonstrated

### Rust Side
✅ `#[repr(C)]` for C-compatible memory layout  
✅ `#[no_mangle]` to prevent name mangling  
✅ `extern "C"` for C calling convention  
✅ Null pointer checks before dereferencing  
✅ Safe string handling with `CStr`  
✅ Comprehensive safety documentation  
✅ Unit tests  

### C++ Side
✅ RAII for automatic cleanup  
✅ const correctness  
✅ Modern C++17 features  
✅ Multiple practical examples  

### Build System
✅ Cross-platform support  
✅ Automatic dependency building  
✅ Proper system library linking  
✅ Multiple build options  

## Testing

### Rust Tests
```bash
cd rust-lib
cargo test
```
✅ All tests passing

### Full Demo
```bash
cd build
./demo
```
✅ Successfully runs with expected output

## Platform Notes

- **macOS**: Use CMake (tested and working)
- **Linux**: CMake or Makefile both should work
- **Windows**: Use CMake with MSVC

## Next Steps / Extensions

Potential enhancements you could add:
1. More complex data structures (nested structs, arrays)
2. Callbacks from Rust to C++
3. Error handling with Result types
4. Async FFI with Tokio
5. More comprehensive BMI calculations with weight parameter
6. Thread-safe operations
7. Shared library (cdylib) example

## References

- [Rust FFI Omnibus](http://jakegoulding.com/rust-ffi-omnibus/)
- [The Rustonomicon - FFI](https://doc.rust-lang.org/nomicon/ffi.html)
- [Rust Book - Unsafe Rust](https://doc.rust-lang.org/book/ch19-01-unsafe-rust.html)

---

**Status**: ✅ Complete and fully functional
**Last Tested**: November 14, 2025
**Platform**: macOS 25.1.0

