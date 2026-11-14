#include "FoodRequest.hpp"

namespace ANNADATU {

FoodRequest::FoodRequest()
    : m_requestId("REQ_000"), m_acceptorId("A_000"), m_acceptorName("Anonymous NGO"),
      m_servingsNeeded(50), m_dietPreference(DietType::ANY), m_urgency(UrgencyLevel::MEDIUM),
      m_destinationLocation(), m_beneficiaryGroup("Local community shelter"),
      m_isFulfilled(false), m_createdAt(formatTimestamp(getCurrentTime())) {}

FoodRequest::FoodRequest(std::string reqId, std::string accId, std::string accName,
                         int servings, DietType diet, UrgencyLevel urgency,
                         Location destLoc, std::string groupDesc, bool fulfilled, std::string createdAt)
    : m_requestId(std::move(reqId)), m_acceptorId(std::move(accId)), m_acceptorName(std::move(accName)),
      m_servingsNeeded(servings), m_dietPreference(diet), m_urgency(urgency),
      m_destinationLocation(std::move(destLoc)), m_beneficiaryGroup(std::move(groupDesc)),
      m_isFulfilled(fulfilled), m_createdAt(std::move(createdAt)) {
    if (m_createdAt.empty()) {
        m_createdAt = formatTimestamp(getCurrentTime());
    }
}

std::string FoodRequest::serialize() const {
    std::ostringstream oss;
    oss << m_requestId << "|"
        << m_acceptorId << "|"
        << m_acceptorName << "|"
        << m_servingsNeeded << "|"
        << static_cast<int>(m_dietPreference) << "|"
        << static_cast<int>(m_urgency) << "|"
        << m_destinationLocation.serialize() << "|"
        << m_beneficiaryGroup << "|"
        << (m_isFulfilled ? 1 : 0) << "|"
        << m_createdAt << "|";
    return oss.str();
}

FoodRequest FoodRequest::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string id, aId, aName, servStr, dietStr, urgStr, locStr, group, fulStr, cDate;

    if (std::getline(ss, id, '|') &&
        std::getline(ss, aId, '|') &&
        std::getline(ss, aName, '|') &&
        std::getline(ss, servStr, '|') &&
        std::getline(ss, dietStr, '|') &&
        std::getline(ss, urgStr, '|') &&
        std::getline(ss, locStr, '|') &&
        std::getline(ss, group, '|') &&
        std::getline(ss, fulStr, '|') &&
        std::getline(ss, cDate, '|')) {

        int servings = std::stoi(servStr);
        DietType diet = static_cast<DietType>(std::stoi(dietStr));
        UrgencyLevel urg = static_cast<UrgencyLevel>(std::stoi(urgStr));
        Location loc = Location::deserialize(locStr);
        bool ful = (fulStr == "1");

        return FoodRequest(id, aId, aName, servings, diet, urg, loc, group, ful, cDate);
    }
    return FoodRequest();
}

std::ostream& operator<<(std::ostream& os, const FoodRequest& req) {
    os << "[" << req.m_requestId << "] " << req.m_acceptorName
       << " requires " << req.m_servingsNeeded << " meals (" << dietTypeToString(req.m_dietPreference)
       << ")\n      Urgency: " << urgencyToString(req.m_urgency)
       << " | Location: " << req.m_destinationLocation.getFullString()
       << " | For: " << req.m_beneficiaryGroup
       << " | Status: " << (req.m_isFulfilled ? "[FULFILLED]" : "[ACTIVE NEED]");
    return os;
}

} // namespace ANNADATU
