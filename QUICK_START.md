# 🚀 Quick Start Guide

## Project Location
✅ **Successfully moved to**: `/Users/evgeny/GIT/cpp-rust-ffi-demo`

## 🎯 One-Command Setup

The easiest way to initialize git and verify everything works:

```bash
cd /Users/evgeny/GIT/cpp-rust-ffi-demo
chmod +x init-git.sh build.sh
./init-git.sh    # Initialize git
./build.sh       # Build and test everything
```

That's it! This will:
1. ✅ Initialize the git repository
2. ✅ Create the initial commit
3. ✅ Build the Rust library
4. ✅ Build the C++ application
5. ✅ Run tests
6. ✅ Run the demo

## 📁 What You Have

### New Scripts (Created During Move)
- **`init-git.sh`** - Initialize git repository with initial commit
- **`build.sh`** - Build everything and run tests/demo
- **`VERIFICATION.md`** - Detailed verification checklist
- **`QUICK_START.md`** - This file!

### Original Project Files
- **`README.md`** - Full project documentation
- **`PROJECT_SUMMARY.md`** - Project overview
- **`rust-lib/`** - Rust library with FFI
- **`cpp-app/`** - C++ application
- **`CMakeLists.txt`** - CMake build config
- **`Makefile`** - Alternative build (Linux)
- **`.gitignore`** - Git ignore rules

## 🔧 Manual Commands (If You Prefer)

### Initialize Git
```bash
cd /Users/evgeny/GIT/cpp-rust-ffi-demo
git init
git add .
git commit -m "Initial commit: C++ to Rust FFI demo"
```

### Build Project
```bash
# Build Rust library
cd rust-lib
cargo build --release
cd ..

# Build C++ application
mkdir -p build
cd build
cmake ..
cmake --build .

# Run demo
./demo
```

### Run Tests
```bash
cd rust-lib
cargo test
```

## 📊 Expected Output

When you run `./build.sh`, you should see:

```
=== C++ Rust FFI Demo Build Script ===
Initializing git repository...
✅ Git repository initialized

Building Rust library...
✅ Rust library built successfully

Building C++ application with CMake...
✅ C++ application built successfully

Running Rust tests...
✅ Tests passed

Running demo...
================================
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
================================

🎉 Build and verification complete!
```

## 🎮 Common Git Commands

```bash
# Check status
git status

# View history
git log --oneline

# Create a new branch
git checkout -b feature/my-feature

# View all branches
git branch -a

# Add remote repository
git remote add origin https://github.com/yourusername/cpp-rust-ffi-demo.git

# Push to remote
git push -u origin main
```

## 📚 Documentation

- **Full docs**: See `README.md`
- **Project overview**: See `PROJECT_SUMMARY.md`
- **Verification details**: See `VERIFICATION.md`

## ✅ Verification Checklist

Run these to verify everything is working:

```bash
# 1. Check files are present
ls -la

# 2. Initialize git
./init-git.sh

# 3. Build and test
./build.sh

# 4. Check git status
git status
git log

# 5. Run demo manually
./build/demo
```

## 🐛 Troubleshooting

### Permission Denied for Scripts
```bash
chmod +x init-git.sh build.sh
```

### CMake Not Found
```bash
brew install cmake  # macOS
```

### Rust Not Found
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

## 🎯 Next Steps

After verifying the project works:

1. **Explore the code**: Check out `rust-lib/src/lib.rs` and `cpp-app/main.cpp`
2. **Modify examples**: Try changing the Person struct or adding new functions
3. **Add features**: Extend with callbacks, async, or more complex types
4. **Create remote**: Push to GitHub/GitLab for backup
5. **Share**: Show others your FFI implementation!

## 📍 Project Path

```
/Users/evgeny/GIT/cpp-rust-ffi-demo
```

**Everything is ready to go!** Just run the scripts above to initialize git and build.

