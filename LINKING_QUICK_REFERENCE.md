# Linking Quick Reference

## Current Status: STATIC LINKING ✓

Your project currently uses **static linking** - the Rust library is compiled directly into the executable.

---

## Quick Commands

### Check Current Mode
```bash
./switch_linking.sh show
```

### Switch to Dynamic Linking
```bash
./switch_linking.sh dynamic
```

### Switch Back to Static Linking
```bash
./switch_linking.sh static
```

---

## What's the Difference?

### Static Linking (Current)
```
┌─────────────────┐
│     demo        │  Single file
│  (includes      │  ~2-3 MB
│   Rust code)    │
└─────────────────┘
```

**When to use:**
- ✅ Distributing a single executable
- ✅ Embedded systems
- ✅ Simpler deployment

### Dynamic Linking
```
┌─────────────┐      ┌──────────────────┐
│    demo     │ ───> │  librust_lib     │
│  (~500 KB)  │      │  (.dylib/.so)    │
└─────────────┘      └──────────────────┘
```

**When to use:**
- ✅ Development (faster rebuilds)
- ✅ Multiple programs using same library
- ✅ Updating library without recompiling main app
- ✅ Smaller executable size

---

## Manual Switch Instructions

### To Dynamic Linking

**1. Update Cargo.toml:**
```bash
# In rust-lib/Cargo.toml, change:
crate-type = ["staticlib"]
# To:
crate-type = ["cdylib"]
```

**2. Update CMakeLists.txt:**
```bash
# Change library name detection to:
# macOS:   set(RUST_LIB_NAME "librust_lib.dylib")
# Linux:   set(RUST_LIB_NAME "librust_lib.so")
# Windows: set(RUST_LIB_NAME "rust_lib.dll")

# Change import type from:
add_library(rust_lib STATIC IMPORTED)
# To:
add_library(rust_lib SHARED IMPORTED)
```

**3. Rebuild:**
```bash
./clean.sh && ./build.sh
```

### To Static Linking

**1. Update Cargo.toml:**
```bash
# In rust-lib/Cargo.toml, change:
crate-type = ["cdylib"]
# To:
crate-type = ["staticlib"]
```

**2. Update CMakeLists.txt:**
```bash
# Change library name to .a/.lib
# Change import type to STATIC IMPORTED
```

**3. Rebuild:**
```bash
./clean.sh && ./build.sh
```

---

## Verification

### Check Library Type

**macOS:**
```bash
file rust-lib/target/release/librust_lib.*

# Static:  librust_lib.a: current ar archive
# Dynamic: librust_lib.dylib: Mach-O 64-bit dynamically linked shared library
```

**Linux:**
```bash
file rust-lib/target/release/librust_lib.*

# Static:  librust_lib.a: current ar archive
# Dynamic: librust_lib.so: ELF 64-bit LSB shared object
```

### Check Executable Dependencies

**macOS:**
```bash
otool -L build/demo

# Dynamic linking will show:
#   @rpath/librust_lib.dylib
```

**Linux:**
```bash
ldd build/demo

# Dynamic linking will show:
#   librust_lib.so => /path/to/librust_lib.so
```

**Windows:**
```powershell
dumpbin /dependents build\demo.exe

# Dynamic linking will show:
#   rust_lib.dll
```

---

## Template Files Provided

The following template files are included for easy switching:

- **`Cargo.toml.dynamic`** - Rust config for dynamic library
- **`CMakeLists.txt.dynamic`** - CMake config for dynamic linking
- **`switch_linking.sh`** - Automated switching script

---

## FAQ

**Q: Which should I use?**
A: Use **dynamic linking** during development for faster iteration. Choose based on deployment needs:
- Single file? → Static
- Multiple apps sharing library? → Dynamic
- Frequent library updates? → Dynamic

**Q: Is there a performance difference?**
A: Minimal (~1-2 nanoseconds per function call with dynamic linking). Negligible in practice.

**Q: Can I use both?**
A: Yes! Rust can build both:
```toml
crate-type = ["staticlib", "cdylib"]
```
Then choose which to link against in CMakeLists.txt.

**Q: What about runtime loading (dlopen)?**
A: Possible but complex with `cxx` bridge due to C++ name mangling. See `DYNAMIC_LINKING.md` for details. Generally not recommended unless you need plugin architecture.

---

## Common Issues

### "Library not found" at runtime (Dynamic)

**macOS:**
```bash
# Add rpath to executable
install_name_tool -add_rpath @executable_path/../lib build/demo
```

**Linux:**
```bash
# Set library path
export LD_LIBRARY_PATH=$PWD/rust-lib/target/release:$LD_LIBRARY_PATH
./build/demo
```

**Windows:**
```powershell
# Copy DLL to executable directory
copy rust-lib\target\release\rust_lib.dll build\
```

### Build errors after switching

```bash
# Clean everything and rebuild
./clean.sh
rm -rf rust-lib/target
cd rust-lib && cargo build --release && cd ..
./build.sh
```

---

## Related Files

- **`DYNAMIC_LINKING.md`** - Complete guide with all details
- **`cpp-app/dynamic_loader.cpp`** - Example of runtime loading with dlopen
- **`switch_linking.sh`** - Automated switching script

---

## Summary

| Aspect | Static | Dynamic |
|--------|--------|---------|
| **File count** | 1 | 2 |
| **Executable size** | Larger | Smaller |
| **Distribution** | Easier | Need both files |
| **Updates** | Recompile all | Just library |
| **Startup time** | Faster (slightly) | Normal |
| **Current default** | ✓ Yes | No |

**Try it yourself:**
```bash
# See current setup
./switch_linking.sh show

# Try dynamic linking
./switch_linking.sh dynamic
./build/demo

# Switch back
./switch_linking.sh static
```

