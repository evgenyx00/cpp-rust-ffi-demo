// Include our C++ definitions (existing C++ code)
#include "person.h"

// Include the cxx-generated header (for Rust FFI)
#include "rust-lib/src/lib.rs.h"

#include <iostream>
#include <string>
#include <memory>

// Helper function to print PersonInfo results from Rust
void print_person_info(const PersonInfo& info, const std::string& name) {
    std::cout << "\n=== Person Information (from Rust analysis) ===" << std::endl;
    std::cout << "Name: " << name << std::endl;
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
    std::cout << "============================================\n" << std::endl;
}

// Helper function to print HealthAnalysis results from Rust
void print_health_analysis(const HealthAnalysis& analysis, const std::string& name) {
    std::cout << "\n=== Health Analysis for " << name << " (Rust) ===" << std::endl;
    std::cout << "BMI: " << analysis.bmi << std::endl;
    std::cout << "Risk Score: " << analysis.risk_score << std::endl;
    std::cout << "City Risk Factor: " << analysis.city_risk_factor << std::endl;
    std::cout << "Recommendation: " << std::string(analysis.recommendation) << std::endl;
    std::cout << "============================================\n" << std::endl;
}

int main() {
    std::cout << "C++ ↔ Rust FFI Demo with Opaque C++ Types\n" << std::endl;
    std::cout << "Demonstrating: Real-world integration pattern" << std::endl;
    std::cout << "- C++ classes defined in C++ code (person.h)" << std::endl;
    std::cout << "- Rust treats them as opaque types" << std::endl;
    std::cout << "- New Rust functionality works with existing C++ objects\n" << std::endl;
    
    // Example 1: Simple greeting
    std::cout << "--- Example 1: String Handling ---" << std::endl;
    size_t length = greet_person("Alice");
    std::cout << "Returned name length: " << length << "\n" << std::endl;
    
    // Example 2: Create C++ objects using our existing C++ classes
    std::cout << "--- Example 2: C++ Objects → Rust Processing ---" << std::endl;
    
    // Create Address (our C++ class)
    auto addr1 = std::make_shared<Address>("123 Main St", "New York", "10001");
    
    // Create ContactInfo (our C++ class with nested Address)
    auto contact1 = std::make_shared<ContactInfo>("bob@example.com", "555-1234", addr1);
    
    // Create Person (our C++ class)
    auto person1 = std::make_unique<Person>(25, 1.75, "Bob Johnson", contact1);
    
    std::cout << "Created C++ Person: " << person1->name() << std::endl;
    std::cout << "Age: " << person1->age() << ", Height: " << person1->height() << "m" << std::endl;
    std::cout << "City: " << person1->contact().address().city() << std::endl;
    
    // Pass C++ object to Rust for processing
    std::cout << "\nSending to Rust for analysis..." << std::endl;
    PersonInfo info1 = process_person(*person1);
    print_person_info(info1, person1->name());
    
    // Example 3: New Rust functionality - Health Analysis
    std::cout << "--- Example 3: Advanced Health Analysis (New Rust Feature) ---" << std::endl;
    double weight1 = 75.0; // kg
    HealthAnalysis health1 = analyze_health(*person1, weight1);
    print_health_analysis(health1, person1->name());
    
    // Example 4: Minor person
    std::cout << "--- Example 4: Minor Person Analysis ---" << std::endl;
    auto addr2 = std::make_shared<Address>("456 Oak Ave", "Boston", "02101");
    auto contact2 = std::make_shared<ContactInfo>("charlie@example.com", "555-5678", addr2);
    auto person2 = std::make_unique<Person>(16, 1.60, "Charlie Smith", contact2);
    
    std::cout << "Created C++ Person: " << person2->name() << std::endl;
    PersonInfo info2 = process_person(*person2);
    print_person_info(info2, person2->name());
    
    double weight2 = 55.0; // kg
    HealthAnalysis health2 = analyze_health(*person2, weight2);
    print_health_analysis(health2, person2->name());
    
    // Example 5: Contact validation (new Rust functionality)
    std::cout << "--- Example 5: Contact Validation (Rust) ---" << std::endl;
    bool valid1 = validate_contact(person1->contact());
    bool valid2 = validate_contact(person2->contact());
    
    std::cout << person1->name() << "'s contact is " 
              << (valid1 ? "VALID ✓" : "INVALID ✗") << std::endl;
    std::cout << person2->name() << "'s contact is " 
              << (valid2 ? "VALID ✓" : "INVALID ✗") << std::endl;
    std::cout << std::endl;
    
    // Example 6: Invalid contact
    std::cout << "--- Example 6: Testing Invalid Contact ---" << std::endl;
    auto bad_addr = std::make_shared<Address>("", "", "123");
    auto bad_contact = std::make_shared<ContactInfo>("bademail", "123", bad_addr);
    auto person3 = std::make_unique<Person>(30, 1.80, "Invalid User", bad_contact);
    
    bool valid3 = validate_contact(person3->contact());
    std::cout << person3->name() << "'s contact is " 
              << (valid3 ? "VALID ✓" : "INVALID ✗") << std::endl;
    std::cout << std::endl;
    
    // Example 7: Direct BMI calculation
    std::cout << "--- Example 7: Direct BMI Calculation (Pure Rust) ---" << std::endl;
    double bmi = calculate_bmi(70.0, 1.75);
    std::cout << "BMI for 70kg, 1.75m: " << bmi << std::endl;
    std::cout << std::endl;
    
    // Example 8: Using C++ methods alongside Rust functions
    std::cout << "--- Example 8: C++ Methods + Rust Functions ---" << std::endl;
    std::cout << "Bob's age (C++ method): " << person1->age() << std::endl;
    std::cout << "Bob is adult (C++ method): " << (person1->is_adult() ? "Yes" : "No") << std::endl;
    std::cout << "Bob's BMI from C++ method: " << person1->calculate_bmi(75.0) << std::endl;
    std::cout << "Bob's BMI from Rust function: " << calculate_bmi(75.0, person1->height()) << std::endl;
    
    // Using factory functions to create objects
    std::cout << "\n--- Example 9: Using Factory Functions ---" << std::endl;
    auto addr3 = create_address("789 Pine Rd", "San Francisco", "94102");
    std::cout << "Created address: " << addr3->city() << std::endl;
    
    std::cout << "\n✅ Demo completed successfully!" << std::endl;
    std::cout << "\n╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║         Key Integration Patterns Demonstrated            ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "\n✓ C++ Classes Defined in C++ Code" << std::endl;
    std::cout << "  - Person, ContactInfo, Address classes in person.h" << std::endl;
    std::cout << "  - Full C++ encapsulation with private fields" << std::endl;
    std::cout << "  - C++ methods and business logic preserved" << std::endl;
    
    std::cout << "\n✓ Opaque Types in Rust" << std::endl;
    std::cout << "  - Rust declares C++ types but doesn't see inside" << std::endl;
    std::cout << "  - Uses getter functions to access data" << std::endl;
    std::cout << "  - Type-safe at compile time" << std::endl;
    
    std::cout << "\n✓ New Rust Functionality" << std::endl;
    std::cout << "  - process_person() analyzes C++ objects" << std::endl;
    std::cout << "  - analyze_health() adds new features" << std::endl;
    std::cout << "  - validate_contact() provides safe validation" << std::endl;
    
    std::cout << "\n✓ Bridge Structs for Results" << std::endl;
    std::cout << "  - PersonInfo and HealthAnalysis" << std::endl;
    std::cout << "  - Used only for data exchange (Rust → C++)" << std::endl;
    std::cout << "  - Shared between both languages" << std::endl;
    
    std::cout << "\n✓ Best Practices" << std::endl;
    std::cout << "  - Minimal changes to existing C++ code" << std::endl;
    std::cout << "  - Gradual Rust adoption" << std::endl;
    std::cout << "  - No unsafe code in Rust" << std::endl;
    std::cout << "  - Clear separation of concerns" << std::endl;
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  This is the RECOMMENDED approach for integrating Rust   ║" << std::endl;
    std::cout << "║  into existing C++ codebases!                            ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝\n" << std::endl;
    
    return 0;
}
