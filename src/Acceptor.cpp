#include "Acceptor.hpp"

namespace ANNADATU {

Acceptor::Acceptor(std::string id, std::string name, std::string email, std::string phone,
                   std::string password, Location loc, AcceptorType aType, int capacity,
                   int mealsReceived, bool isVerified, std::string createdAt)
    : User(std::move(id), std::move(name), std::move(email), std::move(phone),
           std::move(password), std::move(loc), Role::ACCEPTOR, isVerified, std::move(createdAt)),
      m_acceptorType(aType), m_beneficiaryCapacity(capacity), m_totalMealsReceived(mealsReceived) {}

std::string Acceptor::getRoleSpecificType() const {
    return acceptorTypeToString(m_acceptorType);
}

void Acceptor::displayProfile() const {
    std::cout << "\n=============== ACCEPTOR PROFILE ===============\n";
    std::cout << " ID:                 " << m_userId << "\n";
    std::cout << " Name:               " << m_name << "\n";
    std::cout << " Role:               " << roleToString(m_role) << " (" << getRoleSpecificType() << ")\n";
    std::cout << " Contact:            " << m_phone << " | " << m_email << "\n";
    std::cout << " Base Location:      " << m_location.getFullString() << "\n";
    std::cout << " Verified:           " << (m_isVerified ? "YES [Trust Verified NGO/Acceptor]" : "NO [Pending]") << "\n";
    std::cout << " Beneficiaries Cap:  ~" << m_beneficiaryCapacity << " persons\n";
    std::cout << " Total Meals Rescued:" << m_totalMealsReceived << " meals\n";
    std::cout << " Member Since:       " << m_createdAt << "\n";
    std::cout << "================================================\n";
}

void Acceptor::showDashboard() const {
    std::cout << "\n+-------------------------------------------------------------+\n";
    std::cout << "|                  ACCEPTOR RESCUE DASHBOARD                  |\n";
    std::cout << "+-------------------------------------------------------------+\n";
    std::cout << " Welcome, " << m_name << " (" << getRoleSpecificType() << ")!\n";
    std::cout << " * Daily Beneficiary Capacity: " << m_beneficiaryCapacity << " individuals\n";
    std::cout << " * Lifetime Meals Distributed: " << m_totalMealsReceived << " meals\n";
    std::cout << " * Verified Status: " << (m_isVerified ? "Trusted Partner" : "Verification in Review") << "\n";
    std::cout << "+-------------------------------------------------------------+\n";
}

std::string Acceptor::serialize() const {
    std::ostringstream oss;
    oss << "ACCEPTOR|"
        << m_userId << "|"
        << m_name << "|"
        << m_email << "|"
        << m_phone << "|"
        << m_password << "|"
        << m_location.serialize() << "|"
        << static_cast<int>(m_acceptorType) << "|"
        << m_beneficiaryCapacity << "|"
        << m_totalMealsReceived << "|"
        << (m_isVerified ? 1 : 0) << "|"
        << m_createdAt << "|";
    return oss.str();
}

std::shared_ptr<Acceptor> Acceptor::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string tag, id, name, email, phone, pwd, locStr, atStr, capStr, mealsStr, verStr, dateStr;

    std::getline(ss, tag, '|');
    if (tag != "ACCEPTOR") return nullptr;

    if (std::getline(ss, id, '|') &&
        std::getline(ss, name, '|') &&
        std::getline(ss, email, '|') &&
        std::getline(ss, phone, '|') &&
        std::getline(ss, pwd, '|') &&
        std::getline(ss, locStr, '|') &&
        std::getline(ss, atStr, '|') &&
        std::getline(ss, capStr, '|') &&
        std::getline(ss, mealsStr, '|') &&
        std::getline(ss, verStr, '|') &&
        std::getline(ss, dateStr, '|')) {

        Location loc = Location::deserialize(locStr);
        AcceptorType at = static_cast<AcceptorType>(std::stoi(atStr));
        int cap = std::stoi(capStr);
        int meals = std::stoi(mealsStr);
        bool ver = (verStr == "1");

        return std::make_shared<Acceptor>(id, name, email, phone, pwd, loc, at, cap, meals, ver, dateStr);
    }
    return nullptr;
}

} // namespace ANNADATU
