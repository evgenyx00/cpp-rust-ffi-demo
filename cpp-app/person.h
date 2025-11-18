#pragma once

#include <string>
#include <memory>

// C++ defined structs - these are "existing" C++ classes
// that we want to integrate with Rust

/// Address information - a typical C++ class
class Address {
private:
    std::string street_;
    std::string city_;
    std::string postal_code_;

public:
    Address(const std::string& street, const std::string& city, const std::string& postal_code)
        : street_(street), city_(city), postal_code_(postal_code) {}
    
    // Getters (needed for Rust FFI)
    const std::string& street() const { return street_; }
    const std::string& city() const { return city_; }
    const std::string& postal_code() const { return postal_code_; }
    
    // Setters
    void set_street(const std::string& street) { street_ = street; }
    void set_city(const std::string& city) { city_ = city; }
    void set_postal_code(const std::string& postal_code) { postal_code_ = postal_code; }
};

/// Contact information with nested Address
class ContactInfo {
private:
    std::string email_;
    std::string phone_;
    std::shared_ptr<Address> address_;

public:
    ContactInfo(const std::string& email, const std::string& phone, std::shared_ptr<Address> address)
        : email_(email), phone_(phone), address_(address) {}
    
    // Getters
    const std::string& email() const { return email_; }
    const std::string& phone() const { return phone_; }
    const Address& address() const { return *address_; }
    std::shared_ptr<Address> address_ptr() const { return address_; }
    
    // Setters
    void set_email(const std::string& email) { email_ = email; }
    void set_phone(const std::string& phone) { phone_ = phone; }
};

/// Person class - a typical C++ class with data and behavior
class Person {
private:
    uint32_t age_;
    double height_;
    std::string name_;
    std::shared_ptr<ContactInfo> contact_;

public:
    Person(uint32_t age, double height, const std::string& name, std::shared_ptr<ContactInfo> contact)
        : age_(age), height_(height), name_(name), contact_(contact) {}
    
    // Getters (these will be exposed to Rust)
    uint32_t age() const { return age_; }
    double height() const { return height_; }
    const std::string& name() const { return name_; }
    const ContactInfo& contact() const { return *contact_; }
    
    // Setters
    void set_age(uint32_t age) { age_ = age; }
    void set_height(double height) { height_ = height; }
    void set_name(const std::string& name) { name_ = name; }
    
    // C++ methods (business logic)
    bool is_adult() const { return age_ >= 18; }
    double calculate_bmi(double weight_kg) const {
        if (height_ <= 0.0) return 0.0;
        return weight_kg / (height_ * height_);
    }
};

// Factory functions for convenient construction (exposed to Rust)
std::unique_ptr<Address> create_address(const std::string& street, 
                                       const std::string& city, 
                                       const std::string& postal_code);

std::unique_ptr<ContactInfo> create_contact_info(const std::string& email,
                                                 const std::string& phone,
                                                 std::shared_ptr<Address> address);

std::unique_ptr<Person> create_person(uint32_t age,
                                     double height,
                                     const std::string& name,
                                     std::shared_ptr<ContactInfo> contact);

// Getter functions for Rust FFI (these bridge the gap)
uint32_t get_person_age(const Person& person);
double get_person_height(const Person& person);
const std::string& get_person_name(const Person& person);
const ContactInfo& get_person_contact(const Person& person);

const std::string& get_contact_email(const ContactInfo& contact);
const std::string& get_contact_phone(const ContactInfo& contact);
const Address& get_contact_address(const ContactInfo& contact);

const std::string& get_address_street(const Address& address);
const std::string& get_address_city(const Address& address);
const std::string& get_address_postal_code(const Address& address);

