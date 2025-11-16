#[cxx::bridge]
mod ffi {
    // Nested struct example: Address information
    struct Address {
        street: String,
        city: String,
        postal_code: String,
    }

    // Nested struct example: Contact information (contains Address)
    struct ContactInfo {
        email: String,
        phone: String,
        address: Address,
    }

    // A shared struct representing a person with nested ContactInfo
    // This struct is shared between C++ and Rust
    struct Person {
        age: u32,
        height: f64,
        name: String,
        contact: ContactInfo,
    }

    // Result struct with computed information
    struct PersonInfo {
        is_adult: bool,
        bmi_category: u8,    // 0=underweight, 1=normal, 2=overweight
        name_length: usize,
        city: String,        // From nested struct
    }

    // Rust functions callable from C++
    extern "Rust" {
        fn process_person(person: &Person) -> PersonInfo;
        fn greet_person(name: &str) -> usize;
        fn calculate_bmi(weight_kg: f64, height_m: f64) -> f64;
    }
}

/// Process a Person struct and return computed information
/// 
/// This function demonstrates:
/// - Receiving a C++ struct in Rust
/// - Accessing nested struct fields
/// - Accessing String fields (automatic conversion!)
/// - Returning a Rust struct to C++
fn process_person(person: &ffi::Person) -> ffi::PersonInfo {
    // Determine if person is an adult (18+)
    let is_adult = person.age >= 18;
    
    // Calculate BMI category (simplified with assumed weight)
    let assumed_weight_kg = 70.0;
    let bmi = assumed_weight_kg / (person.height * person.height);
    
    let bmi_category = if bmi < 18.5 {
        0 // underweight
    } else if bmi < 25.0 {
        1 // normal
    } else {
        2 // overweight
    };
    
    // Get name length - direct access to String, no unsafe code!
    let name_length = person.name.len();
    
    // Access nested struct fields: person.contact.address.city
    let city = person.contact.address.city.clone();
    
    ffi::PersonInfo {
        is_adult,
        bmi_category,
        name_length,
        city,
    }
}

/// Greet a person by name
/// 
/// Demonstrates string handling - no manual conversion needed!
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
/// Simple function demonstrating passing primitive types
fn calculate_bmi(weight_kg: f64, height_m: f64) -> f64 {
    if height_m <= 0.0 {
        return 0.0;
    }
    weight_kg / (height_m * height_m)
}

#[cfg(test)]
mod tests {
    use super::ffi::{Person, ContactInfo, Address};
    use super::*;

    fn create_test_address(city: &str) -> Address {
        Address {
            street: "123 Test St".to_string(),
            city: city.to_string(),
            postal_code: "12345".to_string(),
        }
    }

    fn create_test_contact(city: &str) -> ContactInfo {
        ContactInfo {
            email: "test@example.com".to_string(),
            phone: "555-1234".to_string(),
            address: create_test_address(city),
        }
    }

    #[test]
    fn test_process_person_adult() {
        let person = Person {
            age: 25,
            height: 1.70,
            name: "Alice".to_string(),
            contact: create_test_contact("New York"),
        };
        
        let info = process_person(&person);
        assert!(info.is_adult);
        assert_eq!(info.name_length, 5);
        assert_eq!(info.city, "New York");
    }

    #[test]
    fn test_process_person_minor() {
        let person = Person {
            age: 16,
            height: 1.60,
            name: "Bob".to_string(),
            contact: create_test_contact("Boston"),
        };
        
        let info = process_person(&person);
        assert!(!info.is_adult);
        assert_eq!(info.name_length, 3);
        assert_eq!(info.city, "Boston");
    }

    #[test]
    fn test_greet_person() {
        let length = greet_person("Charlie");
        assert_eq!(length, 7);
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
    fn test_nested_struct_access() {
        let person = Person {
            age: 30,
            height: 1.80,
            name: "Charlie".to_string(),
            contact: create_test_contact("San Francisco"),
        };
        
        // Test nested field access
        assert_eq!(person.contact.address.city, "San Francisco");
        assert_eq!(person.contact.email, "test@example.com");
    }
}
