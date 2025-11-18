# Dynamic vs Static Linking Guide

This guide explains how to use dynamic linking instead of static linking for the Rust library in this C++ project.

## Table of Contents
1. [Current Setup (Static Linking)](#current-setup-static-linking)
2. [Option 1: Dynamic Linking (Recommended)](#option-1-dynamic-linking-recommended)
3. [Option 2: Runtime Loading with dlopen](#option-2-runtime-loading-with-dlopen)
4. [Comparison](#comparison)
5. [Troubleshooting](#troubleshooting)

---

## Current Setup (Static Linking)

**Current Configuration:**
- `rust-lib/Cargo.toml`: `crate-type = ["staticlib"]`
- `CMakeLists.txt`: Uses `STATIC IMPORTED` library
- Output: `librust_lib.a` (macOS/Linux) or `rust_lib.lib` (Windows)
- Result: Rust code is embedded into the final executable

**Pros:**
- ✅ No runtime library dependencies
- ✅ Slightly faster (no dynamic linking overhead)
- ✅ Single executable file to distribute

**Cons:**
- ❌ Larger executable size
- ❌ Must recompile C++ code when Rust library changes
- ❌ Cannot share library between multiple executables

---

## Option 1: Dynamic Linking (Recommended)

Dynamic linking means the Rust library is loaded at program startup, not compiled into the executable.

### Step-by-Step Instructions

#### 1. Update Cargo.toml

Change the crate type from `staticlib` to `cdylib`:

```bash
# Backup current version
cp rust-lib/Cargo.toml rust-lib/Cargo.toml.static

# Option A: Manually edit rust-lib/Cargo.toml
# Change line: crate-type = ["staticlib"]
# To:          crate-type = ["cdylib"]

# Option B: Use the provided template
cp Cargo.toml.dynamic rust-lib/Cargo.toml
```

#### 2. Update CMakeLists.txt

```bash
# Backup current version
cp CMakeLists.txt CMakeLists.txt.static

# Use the provided dynamic linking configuration
cp CMakeLists.txt.dynamic CMakeLists.txt
```

**Key Changes in CMakeLists.txt:**
- Platform detection now looks for `.dylib`/`.so`/`.dll` instead of `.a`/`.lib`
- Uses `SHARED IMPORTED` instead of `STATIC IMPORTED`
- Adds `rpath` configuration so executable can find the library

#### 3. Rebuild Everything

```bash
# Clean previous build
./clean.sh

# Build with dynamic linking
./build.sh

# Run the demo
./build/demo
```

#### 4. Verify Dynamic Linking

**On macOS:**
```bash
# Check what libraries the executable depends on
otool -L build/demo

# You should see something like:
#   @rpath/librust_lib.dylib
```

**On Linux:**
```bash
ldd build/demo
# Should show librust_lib.so
```

**On Windows:**
```powershell
dumpbin /dependents build/demo.exe
# Should show rust_lib.dll
```

### What Changes?

**Before (Static):**
```
executable size: ~2-3 MB (includes Rust code)
files: demo (executable only)
```

**After (Dynamic):**
```
executable size: ~500 KB (smaller!)
files: demo (executable) + librust_lib.dylib (library)
```

### Distribution

When distributing your application, you must include both:
1. The executable (`demo`)
2. The dynamic library (`librust_lib.dylib`/`.so`/`.dll`)

**Recommended structure:**
```
MyApp/
├── bin/
│   └── demo
└── lib/
    └── librust_lib.dylib
```

The `rpath` settings in `CMakeLists.txt.dynamic` handle finding the library automatically.

---

## Option 2: Runtime Loading with dlopen

This is more complex but allows loading libraries conditionally at runtime (plugin-style).

### Challenges with cxx Bridge

⚠️ **Important**: The `cxx` bridge uses C++ name mangling, making runtime loading difficult.

**Problem:**
```cpp
// cxx generates C++ mangled names like:
// _ZN8rust_lib13greet_person17h9f2e3c4a5b6c7d8eE
```

**Solutions:**

#### Solution A: Add extern "C" Wrappers

Modify `rust-lib/src/lib.rs` to add C-compatible wrappers:

```rust
// After the #[cxx::bridge] block, add:

// C-compatible wrappers for dlopen
#[no_mangle]
pub extern "C" fn calculate_bmi_c(weight_kg: f64, height_m: f64) -> f64 {
    calculate_bmi(weight_kg, height_m)
}

#[no_mangle]
pub extern "C" fn greet_person_c(name_ptr: *const u8, name_len: usize) -> usize {
    let name = unsafe {
        std::str::from_utf8_unchecked(std::slice::from_raw_parts(name_ptr, name_len))
    };
    greet_person(name).len()
}
```

Then in C++:

```cpp
#include <dlfcn.h>

// Load library
void* handle = dlopen("librust_lib.dylib", RTLD_LAZY);

// Load symbols (unmangled C names)
auto calc_bmi = (double(*)(double, double))dlsym(handle, "calculate_bmi_c");

// Use it
double bmi = calc_bmi(70.0, 1.75);

// Clean up
dlclose(handle);
```

#### Solution B: Use Raw FFI Instead of cxx

For plugin systems, raw FFI with `extern "C"` throughout might be better than `cxx`.

### Example: Dynamic Loader

See `cpp-app/dynamic_loader.cpp` for a complete example of runtime loading.

**Compile the example:**
```bash
# First, build the dynamic library
cd rust-lib
cargo build --release
cd ..

# Compile the loader
g++ -std=c++17 cpp-app/dynamic_loader.cpp -o build/dynamic_loader -ldl

# Run it
./build/dynamic_loader
```

---

## Comparison

### When to Use Each Approach

| Scenario | Recommendation |
|----------|---------------|
| **Production application** | Dynamic linking (Option 1) |
| **Quick development iteration** | Dynamic linking (Option 1) |
| **Single-file distribution** | Static linking (Current) |
| **Embedded systems** | Static linking (Current) |
| **Plugin architecture** | Runtime loading (Option 2 + extern "C") |
| **Shipping to users** | Dynamic linking if you can bundle libs |
| **Maximum performance** | Static linking (minimal difference) |

### Performance Impact

**Dynamic linking overhead:** ~1-2 nanoseconds per function call (negligible)

### Feature Comparison

| Feature | Static | Dynamic | Runtime (dlopen) |
|---------|--------|---------|------------------|
| Compile-time type checking | ✅ | ✅ | ❌ |
| Single file distribution | ✅ | ❌ | ❌ |
| Update library without recompiling | ❌ | ✅ | ✅ |
| Smaller executable | ❌ | ✅ | ✅ |
| Multiple programs share library | ❌ | ✅ | ✅ |
| Conditional loading | ❌ | ❌ | ✅ |
| Works with cxx bridge | ✅ | ✅ | ⚠️ (needs wrappers) |
| Complexity | Easy | Easy | Medium/Hard |

---

## Troubleshooting

### Library Not Found at Runtime

**macOS:**
```bash
# Check library location
otool -L build/demo

# If needed, update rpath
install_name_tool -add_rpath @executable_path/../lib build/demo
```

**Linux:**
```bash
# Check library dependencies
ldd build/demo

# If needed, set LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PWD/rust-lib/target/release:$LD_LIBRARY_PATH
```

**Windows:**
- Ensure `.dll` is in the same directory as `.exe`
- Or add to PATH

### Symbol Not Found

```bash
# List exported symbols (macOS/Linux)
nm -gU rust-lib/target/release/librust_lib.dylib

# With C++ name demangling
nm -gU rust-lib/target/release/librust_lib.dylib | c++filt

# On Linux
nm -D rust-lib/target/release/librust_lib.so
```

### Build Fails After Switching

```bash
# Clean everything
./clean.sh
rm -rf rust-lib/target

# Rebuild from scratch
cd rust-lib && cargo build --release && cd ..
./build.sh
```

---

## Quick Switch Commands

### Switch to Dynamic Linking

```bash
cp Cargo.toml.dynamic rust-lib/Cargo.toml
cp CMakeLists.txt.dynamic CMakeLists.txt
./clean.sh && ./build.sh
```

### Switch Back to Static Linking

```bash
cp rust-lib/Cargo.toml.static rust-lib/Cargo.toml 2>/dev/null || echo "[lib]\nname = \"rust_lib\"\ncrate-type = [\"staticlib\"]" > rust-lib/Cargo.toml
cp CMakeLists.txt.static CMakeLists.txt 2>/dev/null || git checkout CMakeLists.txt
./clean.sh && ./build.sh
```

---

## Recommendations

### For Your Project

Given that you're using the `cxx` bridge for type-safe FFI:

✅ **Recommended: Dynamic Linking (Option 1)**
- Keeps all the benefits of `cxx` type safety
- Smaller executables
- Faster development iteration
- Easy to implement (just change config files)

❌ **Not Recommended: Runtime Loading (Option 2)**
- Loses `cxx` type safety benefits
- Requires additional `extern "C"` wrappers
- More complex code
- Only needed for plugin systems

### Best Practice

Use **dynamic linking** during development, then decide:
- **Ship with dynamic library**: If you can bundle the `.dylib`/`.so`/`.dll`
- **Ship with static linking**: If you need single-file distribution

You can easily switch between them by changing two config files!

---

## Additional Resources

- [Rust FFI Guide](https://doc.rust-lang.org/nomicon/ffi.html)
- [cxx Documentation](https://cxx.rs/)
- [CMake RPATH Handling](https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling)

