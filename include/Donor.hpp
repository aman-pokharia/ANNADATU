#ifndef ANNADATU_DONOR_HPP
#define ANNADATU_DONOR_HPP

#include "User.hpp"

namespace ANNADATU {

class Donor : public User {
private:
    DonorType m_donorType;
    int m_rewardPoints;
    int m_totalMealsDonated;
    double m_totalKgSaved;
    std::vector<std::string> m_badges;
    std::vector<std::string> m_donationListingIds;

public:
    Donor(std::string id, std::string name, std::string email, std::string phone,
          std::string password, Location loc, DonorType dType, int points = 0,
          int meals = 0, double kgSaved = 0.0, bool isVerified = true, std::string createdAt = "");

    // Overridden Polymorphic Methods
    void displayProfile() const override;
    void showDashboard() const override;
    std::string getRoleSpecificType() const override;
    std::string serialize() const override;

    // Donor-specific functionality
    DonorType getDonorType() const { return m_donorType; }
    void setDonorType(DonorType dt) { m_donorType = dt; }

    int getRewardPoints() const { return m_rewardPoints; }
    int getTotalMealsDonated() const { return m_totalMealsDonated; }
    double getTotalKgSaved() const { return m_totalKgSaved; }

    const std::vector<std::string>& getBadges() const { return m_badges; }
    const std::vector<std::string>& getDonationHistory() const { return m_donationListingIds; }

    void addDonationId(const std::string& id) { m_donationListingIds.push_back(id); }
    void awardPoints(int points);
    void recordSuccessfulDonation(int meals, double kg, int pointsEarned);

    // Badges & Milestones evaluation
    void evaluateBadges();
    double getEmissionsAvertedKg() const { return m_totalKgSaved * 2.5; } // UNEP metric
    double getWaterSavedLiters() const { return m_totalKgSaved * 800.0; }

    static std::shared_ptr<Donor> deserialize(const std::string& line);
};

} // namespace ANNADATU

#endif // ANNADATU_DONOR_HPP
