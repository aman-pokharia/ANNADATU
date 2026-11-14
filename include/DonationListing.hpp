#ifndef ANNADATU_DONATIONLISTING_HPP
#define ANNADATU_DONATIONLISTING_HPP

#include "Common.hpp"
#include "FoodItem.hpp"
#include "Location.hpp"

namespace ANNADATU {

class DonationListing {
private:
    std::string m_listingId;
    std::string m_donorId;
    std::string m_donorName;
    FoodItem m_foodItem;
    Location m_pickupLocation;
    std::time_t m_preparedTime;
    int m_consumptionWindowHours;
    ListingStatus m_status;
    std::string m_assignedAcceptorId;
    std::string m_assignedAcceptorName;
    std::string m_otp;
    std::string m_createdAt;
    std::string m_completedAt;

public:
    DonationListing();
    DonationListing(std::string listingId, std::string donorId, std::string donorName,
                    FoodItem food, Location pickupLoc, int windowHours,
                    std::time_t preparedTime = 0, ListingStatus status = ListingStatus::AVAILABLE,
                    std::string assignedAccId = "", std::string assignedAccName = "",
                    std::string otp = "", std::string createdAt = "", std::string completedAt = "");

    // Getters & Setters
    const std::string& getId() const { return m_listingId; }
    const std::string& getDonorId() const { return m_donorId; }
    const std::string& getDonorName() const { return m_donorName; }

    const FoodItem& getFoodItem() const { return m_foodItem; }
    FoodItem& getFoodItemRef() { return m_foodItem; }

    const Location& getPickupLocation() const { return m_pickupLocation; }
    void setPickupLocation(const Location& loc) { m_pickupLocation = loc; }

    std::time_t getPreparedTime() const { return m_preparedTime; }
    int getConsumptionWindowHours() const { return m_consumptionWindowHours; }

    ListingStatus getStatus() const { return m_status; }
    void setStatus(ListingStatus st) { m_status = st; }

    const std::string& getAssignedAcceptorId() const { return m_assignedAcceptorId; }
    const std::string& getAssignedAcceptorName() const { return m_assignedAcceptorName; }

    const std::string& getOtp() const { return m_otp; }
    const std::string& getCreatedAt() const { return m_createdAt; }
    const std::string& getCompletedAt() const { return m_completedAt; }

    // Freshness & Expiry dynamics
    double getRemainingHours() const;
    bool isExpired() const;
    std::string getFreshnessBadge() const;

    // Workflow actions
    void markRequested(const std::string& acceptorId, const std::string& acceptorName);
    void confirmAcceptance();
    bool verifyOtpAndComplete(const std::string& enteredOtp);
    void cancelListing();

    // Urgency calculation for matching algorithm
    double calculateUrgencyScore() const;

    // Operator Overloading for priority sorting (perishable first)
    bool operator<(const DonationListing& other) const;

    friend std::ostream& operator<<(std::ostream& os, const DonationListing& listing);

    // Serialization
    std::string serialize() const;
    static DonationListing deserialize(const std::string& line);
};

} // namespace ANNADATU

#endif // ANNADATU_DONATIONLISTING_HPP
