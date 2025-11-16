// Include the cxx-generated header
#include "rust-lib/src/lib.rs.h"
#include <iostream>
#include <string>

void print_person_info(const PersonInfo& info, const rust::String& name) {
    std::cout << "\n=== Person Information ===" << std::endl;
    std::cout << "Name: " << std::string(name) << std::endl;
    std::cout << "Name length: " << info.name_length << std::endl;
    std::cout << "City: " << std::string(info.city) << std::endl;
    std::cout << "Is adult: " << (info.is_adult ? "Yes" : "No") << std::endl;
    
    std::cout << "BMI category: ";
    switch (info.bmi_category) {
        case 0:
            std::cout << "Underweight" << std::endl;
            break;
        case 1:
            std::cout << "Normal" << std::endl;
            break;
        case 2:
            std::cout << "Overweight" << std::endl;
            break;
        default:
            std::cout << "Unknown" << std::endl;
    }
    std::cout << "=========================\n" << std::endl;
}

// Helper function to create an Address (demonstrating nested struct construction)
Address create_address(const std::string& street, const std::string& city, const std::string& postal) {
    return Address{
        .street = rust::String(street),
        .city = rust::String(city),
        .postal_code = rust::String(postal)
    };
}

// Helper function to create ContactInfo (demonstrating nested struct construction)
ContactInfo create_contact(const std::string& email, const std::string& phone, const Address& addr) {
    return ContactInfo{
        .email = rust::String(email),
        .phone = rust::String(phone),
        .address = addr
    };
}

int main() {
    std::cout << "C++ ↔ Rust FFI Demo with Nested Structs using cxx::bridge\n" << std::endl;
    std::cout << "Demonstrating: C++ passing nested structures to Rust\n" << std::endl;
    
    // Example 1: Simple greeting with string
    std::cout << "--- Example 1: String Handling ---" << std::endl;
    size_t length = greet_person("Alice");
    std::cout << "Returned name length: " << length << "\n" << std::endl;
    
    // Example 2: Create nested Person struct in C++ and pass to Rust
    std::cout << "--- Example 2: Adult Person with Nested Structs (C++ → Rust) ---" << std::endl;
    Address addr1 = create_address("123 Main St", "New York", "10001");
    ContactInfo contact1 = create_contact("bob@example.com", "555-1234", addr1);
    
    Person adult{
        .age = 25,
        .height = 1.75,
        .name = rust::String("Bob Johnson"),
        .contact = contact1
    };
    
    PersonInfo adult_info = process_person(adult);
    print_person_info(adult_info, adult.name);
    
    // Example 3: Minor person with nested structs
    std::cout << "--- Example 3: Minor Person with Nested Structs (C++ → Rust) ---" << std::endl;
    Address addr2 = create_address("456 Oak Ave", "Boston", "02101");
    ContactInfo contact2 = create_contact("charlie@example.com", "555-5678", addr2);
    
    Person minor{
        .age = 16,
        .height = 1.60,
        .name = rust::String("Charlie Smith"),
        .contact = contact2
    };
    
    PersonInfo minor_info = process_person(minor);
    print_person_info(minor_info, minor.name);
    
    // Example 4: Inline nested struct construction
    std::cout << "--- Example 4: Inline Nested Struct Construction ---" << std::endl;
    Person inline_person{
        .age = 30,
        .height = 1.90,
        .name = rust::String("Diana Rodriguez"),
        .contact = ContactInfo{
            .email = rust::String("diana@example.com"),
            .phone = rust::String("555-9999"),
            .address = Address{
                .street = rust::String("789 Pine Rd"),
                .city = rust::String("San Francisco"),
                .postal_code = rust::String("94102")
            }
        }
    };
    
    PersonInfo inline_info = process_person(inline_person);
    print_person_info(inline_info, inline_person.name);
    
    // Example 5: Calculate BMI directly
    std::cout << "--- Example 5: Direct Function Call ---" << std::endl;
    double bmi = calculate_bmi(70.0, 1.75);
    std::cout << "BMI for 70kg, 1.75m: " << bmi << std::endl;
    std::cout << std::endl;
    
    // Example 6: Accessing nested fields from C++
    std::cout << "--- Example 6: Accessing Nested Fields in C++ ---" << std::endl;
    std::cout << "Bob's email: " << std::string(adult.contact.email) << std::endl;
    std::cout << "Bob's city: " << std::string(adult.contact.address.city) << std::endl;
    std::cout << "Charlie's phone: " << std::string(minor.contact.phone) << std::endl;
    std::cout << "Charlie's street: " << std::string(minor.contact.address.street) << std::endl;
    
    std::cout << "\n✅ Demo completed successfully!" << std::endl;
    std::cout << "\nKey Features Demonstrated:" << std::endl;
    std::cout << "  ✓ Nested C++ structures (Address → ContactInfo → Person)" << std::endl;
    std::cout << "  ✓ C++ structures passed to Rust functions" << std::endl;
    std::cout << "  ✓ Accessing nested fields from both C++ and Rust" << std::endl;
    std::cout << "  ✓ Automatic String ↔ std::string conversion" << std::endl;
    std::cout << "  ✓ Type-safe interface (compile-time checked)" << std::endl;
    std::cout << "  ✓ No raw pointers or unsafe code needed" << std::endl;
    std::cout << "  ✓ Modern C++ ergonomics with designated initializers" << std::endl;
    
    return 0;
}
