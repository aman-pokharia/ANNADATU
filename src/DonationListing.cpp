#include "DonationListing.hpp"

namespace ANNADATU {

DonationListing::DonationListing()
    : m_listingId("LST_000"), m_donorId("D_000"), m_donorName("Anonymous"),
      m_foodItem(), m_pickupLocation(), m_preparedTime(getCurrentTime()),
      m_consumptionWindowHours(6), m_status(ListingStatus::AVAILABLE),
      m_assignedAcceptorId(""), m_assignedAcceptorName(""), m_otp(generateOTP()),
      m_createdAt(formatTimestamp(getCurrentTime())), m_completedAt("") {}

DonationListing::DonationListing(std::string listingId, std::string donorId, std::string donorName,
                                 FoodItem food, Location pickupLoc, int windowHours,
                                 std::time_t preparedTime, ListingStatus status,
                                 std::string assignedAccId, std::string assignedAccName,
                                 std::string otp, std::string createdAt, std::string completedAt)
    : m_listingId(std::move(listingId)), m_donorId(std::move(donorId)), m_donorName(std::move(donorName)),
      m_foodItem(std::move(food)), m_pickupLocation(std::move(pickupLoc)),
      m_consumptionWindowHours(windowHours), m_status(status),
      m_assignedAcceptorId(std::move(assignedAccId)), m_assignedAcceptorName(std::move(assignedAccName)),
      m_otp(std::move(otp)), m_createdAt(std::move(createdAt)), m_completedAt(std::move(completedAt)) {
    m_preparedTime = (preparedTime == 0) ? getCurrentTime() : preparedTime;
    if (m_otp.empty()) {
        m_otp = generateOTP();
    }
    if (m_createdAt.empty()) {
        m_createdAt = formatTimestamp(getCurrentTime());
    }
}

double DonationListing::getRemainingHours() const {
    std::time_t now = getCurrentTime();
    double secondsElapsed = std::difftime(now, m_preparedTime);
    double hoursElapsed = secondsElapsed / 3600.0;
    double hoursLeft = static_cast<double>(m_consumptionWindowHours) - hoursElapsed;
    return hoursLeft;
}

bool DonationListing::isExpired() const {
    return getRemainingHours() <= 0.0;
}

std::string DonationListing::getFreshnessBadge() const {
    double rem = getRemainingHours();
    if (rem <= 0.0) {
        return "[EXPIRED - DO NOT CONSUME]";
    } else if (rem <= 1.5) {
        return "[CRITICAL FRESHNESS (<1.5h)]";
    } else if (rem <= 3.5) {
        return "[SERVE SOON (<3.5h)]";
    } else {
        return "[FRESH & SAFE (>3.5h)]";
    }
}

void DonationListing::markRequested(const std::string& acceptorId, const std::string& acceptorName) {
    if (m_status != ListingStatus::AVAILABLE) {
        throw ListingException("Listing is not available for booking.");
    }
    m_assignedAcceptorId = acceptorId;
    m_assignedAcceptorName = acceptorName;
    m_status = ListingStatus::REQUESTED;
}

void DonationListing::confirmAcceptance() {
    if (m_status != ListingStatus::REQUESTED) {
        throw ListingException("No pending request to confirm for this listing.");
    }
    m_status = ListingStatus::MATCHED;
}

bool DonationListing::verifyOtpAndComplete(const std::string& enteredOtp) {
    if (m_status != ListingStatus::MATCHED && m_status != ListingStatus::REQUESTED) {
        throw ListingException("Listing is not in matched state for handover.");
    }
    if (m_otp == enteredOtp) {
        m_status = ListingStatus::COMPLETED;
        m_completedAt = formatTimestamp(getCurrentTime());
        return true;
    }
    return false;
}

void DonationListing::cancelListing() {
    m_status = ListingStatus::CANCELLED;
}

double DonationListing::calculateUrgencyScore() const {
    // Higher score means higher urgency to rescue
    double rem = getRemainingHours();
    if (rem <= 0.0) return -100.0; // Expired
    // Cooked food is more urgent than raw food
    double typeMultiplier = (m_foodItem.getFoodType() == FoodType::COOKED) ? 1.5 : 1.0;
    // Less hours remaining => much higher score
    double urgencyScore = (100.0 / (rem + 0.5)) * typeMultiplier;
    return urgencyScore;
}

// Less operator: Sorts so that highest urgency comes first
bool DonationListing::operator<(const DonationListing& other) const {
    return this->calculateUrgencyScore() > other.calculateUrgencyScore();
}

std::ostream& operator<<(std::ostream& os, const DonationListing& l) {
    os << "[" << l.m_listingId << "] " << l.m_foodItem.getTitle()
       << " | Qty: " << l.m_foodItem.getQuantityKg() << "kg (~" << l.m_foodItem.getServingsCount() << " meals)\n"
       << "      Donor: " << l.m_donorName << " | Location: " << l.m_pickupLocation.getFullString() << "\n"
       << "      Status: " << listingStatusToString(l.m_status) << " " << l.getFreshnessBadge()
       << " (Remaining: " << std::fixed << std::setprecision(1) << (l.getRemainingHours() > 0 ? l.getRemainingHours() : 0.0) << "h)";
    return os;
}

std::string DonationListing::serialize() const {
    std::ostringstream oss;
    oss << m_listingId << "|"
        << m_donorId << "|"
        << m_donorName << "|"
        << m_foodItem.serialize() << "|"
        << m_pickupLocation.serialize() << "|"
        << m_preparedTime << "|"
        << m_consumptionWindowHours << "|"
        << static_cast<int>(m_status) << "|"
        << m_assignedAcceptorId << "|"
        << m_assignedAcceptorName << "|"
        << m_otp << "|"
        << m_createdAt << "|"
        << m_completedAt << "|";
    return oss.str();
}

DonationListing DonationListing::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string id, dId, dName, foodStr, locStr, prepStr, winStr, stStr, accId, accName, otp, cDate, compDate;

    if (std::getline(ss, id, '|') &&
        std::getline(ss, dId, '|') &&
        std::getline(ss, dName, '|') &&
        std::getline(ss, foodStr, '|') &&
        std::getline(ss, locStr, '|') &&
        std::getline(ss, prepStr, '|') &&
        std::getline(ss, winStr, '|') &&
        std::getline(ss, stStr, '|') &&
        std::getline(ss, accId, '|') &&
        std::getline(ss, accName, '|') &&
        std::getline(ss, otp, '|') &&
        std::getline(ss, cDate, '|')) {

        std::getline(ss, compDate, '|');

        FoodItem food = FoodItem::deserialize(foodStr);
        Location loc = Location::deserialize(locStr);
        std::time_t prep = static_cast<std::time_t>(std::stoll(prepStr));
        int win = std::stoi(winStr);
        ListingStatus st = static_cast<ListingStatus>(std::stoi(stStr));

        return DonationListing(id, dId, dName, food, loc, win, prep, st, accId, accName, otp, cDate, compDate);
    }
    return DonationListing();
}

} // namespace ANNADATU
