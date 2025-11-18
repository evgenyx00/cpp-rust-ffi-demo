fn main() {
    // Get the project root directory (parent of rust-lib)
    let project_root = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("Failed to get project root");
    
    cxx_build::bridge("src/lib.rs")
        .flag_if_supported("-std=c++14")
        .include(project_root)  // Add project root to include path for cpp-app/person.h
        .file("../cpp-app/person.cpp")  // Include the C++ implementation file
        .compile("rust_lib_cxx");

    println!("cargo:rerun-if-changed=src/lib.rs");
    println!("cargo:rerun-if-changed=../cpp-app/person.h");
    println!("cargo:rerun-if-changed=../cpp-app/person.cpp");
}
