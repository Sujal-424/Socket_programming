#include "user.hpp"

User::User(int id, std::string username, std::string password, Role role,
           int balance, std::string phone, std::string address)
    : id_(id),
      username_(std::move(username)),
      password_(std::move(password)),
      role_(role),
      balance_(balance),
      phone_(std::move(phone)),
      address_(std::move(address)) {}

void User::editInfo(const std::string& newPassword, const std::string& newPhone, const std::string& newAddress) {
    password_ = newPassword;
    phone_ = newPhone;
    address_ = newAddress;
}

void User::increaseBalance(int amount) {
    balance_ += amount;
}

void User::decreaseBalance(int amount) {
    balance_ -= amount;
}

bool User::isPasswordCorrect(const std::string& hashedPassword) const {
    return password_ == hashedPassword;
}

User::Role User::getRole() const { return role_; }
int User::getId() const { return id_; }
std::string User::getUsername() const { return username_; }
int User::getBalance() const { return balance_; }

nlohmann::json User::toJson(bool includePassword) const {
    nlohmann::json j;
    j["id"] = id_;
    j["username"] = username_;
    if (includePassword) {
        j["password"] = password_;
    }
    if (role_ == Role::Admin) {
        j["admin"] = true;
    }
    else {
        j["admin"] = false;
        j["balance"] = balance_;
        j["phone"] = phone_;
        j["address"] = address_;
    }
    return j;
}
