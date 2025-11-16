fn main() {
    cxx_build::bridge("src/lib.rs")
        .flag_if_supported("-std=c++14")
        .compile("rust_lib_cxx");

    println!("cargo:rerun-if-changed=src/lib.rs");
}
