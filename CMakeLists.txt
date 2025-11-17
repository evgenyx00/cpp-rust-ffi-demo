cmake_minimum_required(VERSION 3.15)
project(cpp_rust_ffi_demo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Platform detection
if(APPLE)
    set(RUST_LIB_NAME "librust_lib.a")
    set(RUST_TARGET_DIR "${CMAKE_SOURCE_DIR}/rust-lib/target/release")
elseif(WIN32)
    set(RUST_LIB_NAME "rust_lib.lib")
    set(RUST_TARGET_DIR "${CMAKE_SOURCE_DIR}/rust-lib/target/release")
else()
    set(RUST_LIB_NAME "librust_lib.a")
    set(RUST_TARGET_DIR "${CMAKE_SOURCE_DIR}/rust-lib/target/release")
endif()

# Custom target to build Rust library
add_custom_target(build_rust
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/rust-lib
    COMMENT "Building Rust library with cxx bridge..."
)

# Find the cxx-generated headers dynamically and copy to rust-lib/src/
file(GLOB CXX_BRIDGE_DIRS "${RUST_TARGET_DIR}/build/rust-lib-*/out/cxxbridge")
if(CXX_BRIDGE_DIRS)
    list(GET CXX_BRIDGE_DIRS 0 CXX_BRIDGE_DIR)
    
    # Copy generated headers directly to rust-lib/src/ for IDE
    add_custom_command(TARGET build_rust POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying bridge headers to rust-lib/src/..."
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CXX_BRIDGE_DIR}/include/rust-lib/src/lib.rs.h"
            "${CMAKE_SOURCE_DIR}/rust-lib/src/lib.rs.h"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CXX_BRIDGE_DIR}/include/rust-lib/src/lib.rs"
            "${CMAKE_SOURCE_DIR}/rust-lib/src/lib.rs.cc"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CXX_BRIDGE_DIR}/include/rust/cxx.h"
            "${CMAKE_SOURCE_DIR}/rust-lib/src/cxx.h"
        COMMENT "Syncing bridge headers for IDE"
    )
    message(STATUS "cxx bridge directory: ${CXX_BRIDGE_DIR}")
    
    # Add include paths for both the project root and rust bridge
    include_directories(${CMAKE_SOURCE_DIR})
    include_directories(${CXX_BRIDGE_DIR}/include)
else()
    message(WARNING "cxx bridge directory not found. Run: cd rust-lib && cargo build --release")
endif()

# Create imported library target for Rust library
add_library(rust_lib STATIC IMPORTED)
set_target_properties(rust_lib PROPERTIES
    IMPORTED_LOCATION "${RUST_TARGET_DIR}/${RUST_LIB_NAME}"
)

# C++ executable with sources
add_executable(demo 
    cpp-app/main.cpp
    cpp-app/person.cpp
)

# Make sure Rust library is built before the C++ executable
add_dependencies(demo build_rust)

# Link Rust library
target_link_libraries(demo PRIVATE rust_lib)

# Platform-specific system libraries that Rust might need
if(APPLE)
    target_link_libraries(demo PRIVATE
        "-framework Security"
        "-framework CoreFoundation"
        pthread
        dl
        c++
    )
elseif(UNIX)
    target_link_libraries(demo PRIVATE
        pthread
        dl
        m
        stdc++
    )
elseif(WIN32)
    target_link_libraries(demo PRIVATE
        ws2_32
        userenv
        bcrypt
    )
endif()

# Print configuration info
message(STATUS "Rust library path: ${RUST_TARGET_DIR}/${RUST_LIB_NAME}")
message(STATUS "CXX bridge headers: ${RUST_TARGET_DIR}/cxxbridge/")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
