#ifndef ANNADATU_USER_HPP
#define ANNADATU_USER_HPP

#include "Common.hpp"
#include "Location.hpp"

namespace ANNADATU {

// Abstract Base Class illustrating OOP Abstraction & Polymorphism
class User {
protected:
    std::string m_userId;
    std::string m_name;
    std::string m_email;
    std::string m_phone;
    std::string m_password;
    Location m_location;
    Role m_role;
    bool m_isVerified;
    std::string m_createdAt;

public:
    User(std::string id, std::string name, std::string email, std::string phone, 
         std::string password, Location loc, Role role, bool isVerified = true, std::string createdAt = "");

    virtual ~User() = default;

    // Pure virtual methods to enforce polymorphic behavior in derived classes
    virtual void displayProfile() const = 0;
    virtual void showDashboard() const = 0;
    virtual std::string getRoleSpecificType() const = 0;
    virtual std::string serialize() const = 0;

    // Common Getters & Setters
    const std::string& getId() const { return m_userId; }
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    const std::string& getEmail() const { return m_email; }
    void setEmail(const std::string& email) { m_email = email; }

    const std::string& getPhone() const { return m_phone; }
    void setPhone(const std::string& phone) { m_phone = phone; }

    bool verifyPassword(const std::string& pwd) const { return m_password == pwd; }
    void setPassword(const std::string& newPwd) { m_password = newPwd; }

    const Location& getLocation() const { return m_location; }
    void setLocation(const Location& loc) { m_location = loc; }

    Role getRole() const { return m_role; }
    bool isVerified() const { return m_isVerified; }
    void setVerified(bool val) { m_isVerified = val; }

    const std::string& getCreatedAt() const { return m_createdAt; }
};

} // namespace ANNADATU

#endif // ANNADATU_USER_HPP
