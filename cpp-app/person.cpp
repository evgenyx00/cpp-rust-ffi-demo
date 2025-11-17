#include "person.h"

// Factory function implementations

std::unique_ptr<Address> create_address(const std::string& street, 
                                       const std::string& city, 
                                       const std::string& postal_code) {
    return std::make_unique<Address>(street, city, postal_code);
}

std::unique_ptr<ContactInfo> create_contact_info(const std::string& email,
                                                 const std::string& phone,
                                                 std::shared_ptr<Address> address) {
    return std::make_unique<ContactInfo>(email, phone, address);
}

std::unique_ptr<Person> create_person(uint32_t age,
                                     double height,
                                     const std::string& name,
                                     std::shared_ptr<ContactInfo> contact) {
    return std::make_unique<Person>(age, height, name, contact);
}

// Getter function implementations for FFI

uint32_t get_person_age(const Person& person) {
    return person.age();
}

double get_person_height(const Person& person) {
    return person.height();
}

const std::string& get_person_name(const Person& person) {
    return person.name();
}

const ContactInfo& get_person_contact(const Person& person) {
    return person.contact();
}

const std::string& get_contact_email(const ContactInfo& contact) {
    return contact.email();
}

const std::string& get_contact_phone(const ContactInfo& contact) {
    return contact.phone();
}

const Address& get_contact_address(const ContactInfo& contact) {
    return contact.address();
}

const std::string& get_address_street(const Address& address) {
    return address.street();
}

const std::string& get_address_city(const Address& address) {
    return address.city();
}

const std::string& get_address_postal_code(const Address& address) {
    return address.postal_code();
}

