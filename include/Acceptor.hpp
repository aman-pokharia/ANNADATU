#ifndef ANNADATU_ACCEPTOR_HPP
#define ANNADATU_ACCEPTOR_HPP

#include "User.hpp"

namespace ANNADATU {

class Acceptor : public User {
private:
    AcceptorType m_acceptorType;
    int m_beneficiaryCapacity;
    int m_totalMealsReceived;
    std::vector<std::string> m_requestIds;

public:
    Acceptor(std::string id, std::string name, std::string email, std::string phone,
             std::string password, Location loc, AcceptorType aType, int capacity = 50,
             int mealsReceived = 0, bool isVerified = true, std::string createdAt = "");

    // Overridden Polymorphic Methods
    void displayProfile() const override;
    void showDashboard() const override;
    std::string getRoleSpecificType() const override;
    std::string serialize() const override;

    // Acceptor-specific functionality
    AcceptorType getAcceptorType() const { return m_acceptorType; }
    void setAcceptorType(AcceptorType at) { m_acceptorType = at; }

    int getBeneficiaryCapacity() const { return m_beneficiaryCapacity; }
    void setBeneficiaryCapacity(int cap) { m_beneficiaryCapacity = cap; }

    int getTotalMealsReceived() const { return m_totalMealsReceived; }
    void recordMealsReceived(int meals) { m_totalMealsReceived += meals; }

    const std::vector<std::string>& getRequestHistory() const { return m_requestIds; }
    void addRequestId(const std::string& id) { m_requestIds.push_back(id); }

    static std::shared_ptr<Acceptor> deserialize(const std::string& line);
};

} // namespace ANNADATU

#endif // ANNADATU_ACCEPTOR_HPP
