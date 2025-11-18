#[cxx::bridge]
mod ffi {
    // ============================================================================
    // OPAQUE C++ TYPES - Defined in C++ code (person.h)
    // Rust can hold references but cannot see inside these types
    // ============================================================================
    unsafe extern "C++" {
        include!("cpp-app/person.h");
        
        // Opaque C++ types - these are existing C++ classes
        // Rust cannot see inside these types, only hold references
        type Person;
        type ContactInfo;
        type Address;
        
        // Getter functions to access C++ object data from Rust
        // These are the bridge between opaque C++ types and Rust
        fn get_person_age(person: &Person) -> u32;
        fn get_person_height(person: &Person) -> f64;
        fn get_person_name(person: &Person) -> &CxxString;
        fn get_person_contact(person: &Person) -> &ContactInfo;
        
        fn get_contact_email(contact: &ContactInfo) -> &CxxString;
        fn get_contact_phone(contact: &ContactInfo) -> &CxxString;
        fn get_contact_address(contact: &ContactInfo) -> &Address;
        
        fn get_address_street(address: &Address) -> &CxxString;
        fn get_address_city(address: &Address) -> &CxxString;
        fn get_address_postal_code(address: &Address) -> &CxxString;
    }

    // ============================================================================
    // BRIDGE STRUCTS - For data exchange between Rust and C++
    // These are new structs created specifically for passing results
    // ============================================================================
    
    /// Result struct with computed information from Rust
    /// This is NOT a C++ type - it's a bridge type for communication
    struct PersonInfo {
        is_adult: bool,
        bmi_category: u8,    // 0=underweight, 1=normal, 2=overweight
        name_length: usize,
        city: String,        // Extracted from nested C++ structs
    }
    
    /// Health analysis result - new Rust functionality
    struct HealthAnalysis {
        bmi: f64,
        risk_score: f64,
        recommendation: String,
        city_risk_factor: f64,
    }

    // ============================================================================
    // RUST FUNCTIONS - New functionality exposed to C++
    // These work with opaque C++ types and return bridge structs
    // ============================================================================
    extern "Rust" {
        /// Process a C++ Person object and return computed information
        /// Demonstrates: Rust receiving opaque C++ type and extracting data via getters
        fn process_person(person: &Person) -> PersonInfo;
        
        /// Perform health analysis on a C++ Person object
        /// Demonstrates: New Rust functionality working with existing C++ types
        fn analyze_health(person: &Person, weight_kg: f64) -> HealthAnalysis;
        
        /// Simple greeting function
        fn greet_person(name: &str) -> usize;
        
        /// Calculate BMI - pure Rust calculation
        fn calculate_bmi(weight_kg: f64, height_m: f64) -> f64;
        
        /// Validate contact info - demonstrates deep access into nested C++ objects
        fn validate_contact(contact: &ContactInfo) -> bool;
    }
}

// ============================================================================
// RUST IMPLEMENTATIONS
// These functions work with opaque C++ types using getter functions
// ============================================================================

/// Process a C++ Person object and return computed information
/// 
/// This demonstrates the typical pattern:
/// 1. Receive opaque C++ type as reference
/// 2. Use getter functions to extract needed data
/// 3. Perform Rust logic
/// 4. Return bridge struct with results
fn process_person(person: &ffi::Person) -> ffi::PersonInfo {
    // Extract data from C++ object using getter functions
    let age = ffi::get_person_age(person);
    let height = ffi::get_person_height(person);
    let name = ffi::get_person_name(person);
    let contact = ffi::get_person_contact(person);
    let address = ffi::get_contact_address(contact);
    let city = ffi::get_address_city(address);
    
    // Rust logic - determine if person is an adult
    let is_adult = age >= 18;
    
    // Calculate BMI category (simplified with assumed weight)
    let assumed_weight_kg = 70.0;
    let bmi = assumed_weight_kg / (height * height);
    
    let bmi_category = if bmi < 18.5 {
        0 // underweight
    } else if bmi < 25.0 {
        1 // normal
    } else {
        2 // overweight
    };
    
    // Get name length - CxxString can be used like &str in Rust
    let name_length = name.to_str().map(|s| s.len()).unwrap_or(0);
    
    // Extract city from nested C++ objects
    let city_string = city.to_str().unwrap_or("Unknown").to_string();
    
    // Return bridge struct
    ffi::PersonInfo {
        is_adult,
        bmi_category,
        name_length,
        city: city_string,
    }
}

/// Perform comprehensive health analysis
/// 
/// This demonstrates NEW Rust functionality that works with existing C++ types
/// In a real scenario, this might use Rust's advanced features:
/// - Machine learning crates
/// - Concurrent processing
/// - Safe data validation
fn analyze_health(person: &ffi::Person, weight_kg: f64) -> ffi::HealthAnalysis {
    // Extract data from C++ Person object
    let age = ffi::get_person_age(person);
    let height = ffi::get_person_height(person);
    let contact = ffi::get_person_contact(person);
    let address = ffi::get_contact_address(contact);
    let city = ffi::get_address_city(address);
    
    // Calculate BMI
    let bmi = if height > 0.0 {
        weight_kg / (height * height)
    } else {
        0.0
    };
    
    // Complex risk calculation (this is where Rust shines)
    let age_risk = if age < 18 || age > 65 { 1.5 } else { 1.0 };
    let bmi_risk = if bmi < 18.5 || bmi > 25.0 { 1.3 } else { 1.0 };
    
    // City-based risk factor (demonstrating string processing)
    let city_str = city.to_str().unwrap_or("");
    let city_risk = match city_str {
        "New York" => 1.2,
        "Los Angeles" => 1.1,
        _ => 1.0,
    };
    
    let risk_score = age_risk * bmi_risk * city_risk;
    
    // Generate recommendation based on analysis
    let recommendation = if risk_score < 1.2 {
        "Excellent health profile. Maintain current lifestyle.".to_string()
    } else if risk_score < 1.5 {
        "Good health. Consider minor lifestyle adjustments.".to_string()
    } else {
        "Elevated risk factors. Recommend consultation with healthcare provider.".to_string()
    };
    
    ffi::HealthAnalysis {
        bmi,
        risk_score,
        recommendation,
        city_risk_factor: city_risk,
    }
}

/// Greet a person by name
/// 
/// Simple function demonstrating string handling
fn greet_person(name: &str) -> usize {
    if name.is_empty() {
        println!("Hello, stranger!");
        return 0;
    }
    
    println!("Hello from Rust, {}!", name);
    name.len()
}

/// Calculate BMI from weight and height
/// 
/// Pure Rust calculation - no C++ interaction
fn calculate_bmi(weight_kg: f64, height_m: f64) -> f64 {
    if height_m <= 0.0 {
        return 0.0;
    }
    weight_kg / (height_m * height_m)
}

/// Validate contact information
/// 
/// Demonstrates deep access into nested C++ objects:
/// ContactInfo -> Address -> fields
fn validate_contact(contact: &ffi::ContactInfo) -> bool {
    // Extract data from nested C++ objects
    let email = ffi::get_contact_email(contact);
    let phone = ffi::get_contact_phone(contact);
    let address = ffi::get_contact_address(contact);
    let city = ffi::get_address_city(address);
    let postal_code = ffi::get_address_postal_code(address);
    
    // Rust validation logic
    let email_str = email.to_str().unwrap_or("");
    let phone_str = phone.to_str().unwrap_or("");
    let city_str = city.to_str().unwrap_or("");
    let postal_str = postal_code.to_str().unwrap_or("");
    
    // Simple validation rules
    let email_valid = email_str.contains('@') && email_str.len() > 3;
    let phone_valid = phone_str.len() >= 7;
    let city_valid = !city_str.is_empty();
    let postal_valid = postal_str.len() >= 5;
    
    email_valid && phone_valid && city_valid && postal_valid
}

// ============================================================================
// TESTS
// ============================================================================

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_greet_person() {
        let length = greet_person("Alice");
        assert_eq!(length, 5);
    }

    #[test]
    fn test_greet_empty() {
        let length = greet_person("");
        assert_eq!(length, 0);
    }

    #[test]
    fn test_calculate_bmi() {
        let bmi = calculate_bmi(70.0, 1.75);
        assert!((bmi - 22.86).abs() < 0.01);
    }
    
    #[test]
    fn test_calculate_bmi_zero_height() {
        let bmi = calculate_bmi(70.0, 0.0);
        assert_eq!(bmi, 0.0);
    }

    // Note: Tests involving C++ types would need C++ test framework
    // or integration tests. Pure Rust functions can be unit tested here.
}
