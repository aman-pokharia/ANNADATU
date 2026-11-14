#ifndef ANNADATU_FOODREQUEST_HPP
#define ANNADATU_FOODREQUEST_HPP

#include "Common.hpp"
#include "Location.hpp"

namespace ANNADATU {

class FoodRequest {
private:
    std::string m_requestId;
    std::string m_acceptorId;
    std::string m_acceptorName;
    int m_servingsNeeded;
    DietType m_dietPreference;
    UrgencyLevel m_urgency;
    Location m_destinationLocation;
    std::string m_beneficiaryGroup;
    bool m_isFulfilled;
    std::string m_createdAt;

public:
    FoodRequest();
    FoodRequest(std::string reqId, std::string accId, std::string accName,
                int servings, DietType diet, UrgencyLevel urgency,
                Location destLoc, std::string groupDesc, bool fulfilled = false, std::string createdAt = "");

    // Getters & Setters
    const std::string& getId() const { return m_requestId; }
    const std::string& getAcceptorId() const { return m_acceptorId; }
    const std::string& getAcceptorName() const { return m_acceptorName; }

    int getServingsNeeded() const { return m_servingsNeeded; }
    void setServingsNeeded(int s) { m_servingsNeeded = s; }

    DietType getDietPreference() const { return m_dietPreference; }
    void setDietPreference(DietType dt) { m_dietPreference = dt; }

    UrgencyLevel getUrgency() const { return m_urgency; }
    void setUrgency(UrgencyLevel u) { m_urgency = u; }

    const Location& getDestinationLocation() const { return m_destinationLocation; }
    void setDestinationLocation(const Location& loc) { m_destinationLocation = loc; }

    const std::string& getBeneficiaryGroup() const { return m_beneficiaryGroup; }
    void setBeneficiaryGroup(const std::string& bg) { m_beneficiaryGroup = bg; }

    bool isFulfilled() const { return m_isFulfilled; }
    void setFulfilled(bool val) { m_isFulfilled = val; }

    const std::string& getCreatedAt() const { return m_createdAt; }

    // Serialization
    std::string serialize() const;
    static FoodRequest deserialize(const std::string& line);

    friend std::ostream& operator<<(std::ostream& os, const FoodRequest& req);
};

} // namespace ANNADATU

#endif // ANNADATU_FOODREQUEST_HPP
