#include "User.hpp"

namespace ANNADATU {

User::User(std::string id, std::string name, std::string email, std::string phone, 
           std::string password, Location loc, Role role, bool isVerified, std::string createdAt)
    : m_userId(std::move(id)), m_name(std::move(name)), m_email(std::move(email)), 
      m_phone(std::move(phone)), m_password(std::move(password)), 
      m_location(std::move(loc)), m_role(role), m_isVerified(isVerified), 
      m_createdAt(std::move(createdAt)) {
    if (m_createdAt.empty()) {
        m_createdAt = formatTimestamp(getCurrentTime());
    }
}

} // namespace ANNADATU
