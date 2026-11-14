#include "Donor.hpp"

namespace ANNADATU {

Donor::Donor(std::string id, std::string name, std::string email, std::string phone,
             std::string password, Location loc, DonorType dType, int points,
             int meals, double kgSaved, bool isVerified, std::string createdAt)
    : User(std::move(id), std::move(name), std::move(email), std::move(phone), 
           std::move(password), std::move(loc), Role::DONOR, isVerified, std::move(createdAt)),
      m_donorType(dType), m_rewardPoints(points), m_totalMealsDonated(meals), m_totalKgSaved(kgSaved) {
    evaluateBadges();
}

void Donor::awardPoints(int points) {
    if (points > 0) {
        m_rewardPoints += points;
        evaluateBadges();
    }
}

void Donor::recordSuccessfulDonation(int meals, double kg, int pointsEarned) {
    m_totalMealsDonated += meals;
    m_totalKgSaved += kg;
    m_rewardPoints += pointsEarned;
    evaluateBadges();
}

void Donor::evaluateBadges() {
    auto hasBadge = [this](const std::string& b) {
        return std::find(m_badges.begin(), m_badges.end(), b) != m_badges.end();
    };

    if (m_totalMealsDonated >= 1 && !hasBadge("First Spark")) {
        m_badges.push_back("First Spark");
    }
    if (m_totalMealsDonated >= 50 && !hasBadge("Bronze Caregiver")) {
        m_badges.push_back("Bronze Caregiver");
    }
    if (m_totalMealsDonated >= 200 && !hasBadge("Silver Guardian")) {
        m_badges.push_back("Silver Guardian");
    }
    if (m_totalMealsDonated >= 500 && !hasBadge("Gold Champion")) {
        m_badges.push_back("Gold Champion");
    }
    if (m_totalMealsDonated >= 1000 && !hasBadge("Zero-Waste Titan")) {
        m_badges.push_back("Zero-Waste Titan");
    }
}

std::string Donor::getRoleSpecificType() const {
    return donorTypeToString(m_donorType);
}

void Donor::displayProfile() const {
    std::cout << "\n================ DONOR PROFILE ================\n";
    std::cout << " ID:               " << m_userId << "\n";
    std::cout << " Name:             " << m_name << "\n";
    std::cout << " Role:             " << roleToString(m_role) << " (" << getRoleSpecificType() << ")\n";
    std::cout << " Contact:          " << m_phone << " | " << m_email << "\n";
    std::cout << " Location:         " << m_location.getFullString() << "\n";
    std::cout << " Verified:         " << (m_isVerified ? "YES [Trust Verified]" : "NO [Pending]") << "\n";
    std::cout << " Member Since:     " << m_createdAt << "\n";
    std::cout << " Karma Points:     " << m_rewardPoints << " pts\n";
    std::cout << " Total Meals Saved:" << m_totalMealsDonated << " meals\n";
    std::cout << " Surplus Rescued:  " << std::fixed << std::setprecision(1) << m_totalKgSaved << " kg\n";
    std::cout << " Badges Earned:    ";
    if (m_badges.empty()) {
        std::cout << "No badges yet (start donating to unlock!)";
    } else {
        for (size_t i = 0; i < m_badges.size(); ++i) {
            std::cout << "[" << m_badges[i] << "] ";
        }
    }
    std::cout << "\n===============================================\n";
}

void Donor::showDashboard() const {
    std::cout << "\n+-------------------------------------------------------------+\n";
    std::cout << "|                   DONOR IMPACT DASHBOARD                    |\n";
    std::cout << "+-------------------------------------------------------------+\n";
    std::cout << " Welcome back, " << m_name << " (" << getRoleSpecificType() << ")!\n";
    std::cout << " * Impact: " << m_totalMealsDonated << " meals served to people in need\n";
    std::cout << " * Environmental Impact: " << std::fixed << std::setprecision(1)
              << getEmissionsAvertedKg() << " kg CO2e averted | "
              << getWaterSavedLiters() << " L water saved\n";
    std::cout << " * Rewards Balance: " << m_rewardPoints << " ANNADATU Karma Points\n";
    std::cout << " * Badges: " << m_badges.size() << " unlocked\n";
    std::cout << "+-------------------------------------------------------------+\n";
}

std::string Donor::serialize() const {
    std::ostringstream oss;
    oss << "DONOR|"
        << m_userId << "|"
        << m_name << "|"
        << m_email << "|"
        << m_phone << "|"
        << m_password << "|"
        << m_location.serialize() << "|"
        << static_cast<int>(m_donorType) << "|"
        << m_rewardPoints << "|"
        << m_totalMealsDonated << "|"
        << m_totalKgSaved << "|"
        << (m_isVerified ? 1 : 0) << "|"
        << m_createdAt << "|";

    // Badges separated by comma
    for (size_t i = 0; i < m_badges.size(); ++i) {
        oss << m_badges[i];
        if (i + 1 < m_badges.size()) oss << ",";
    }
    return oss.str();
}

std::shared_ptr<Donor> Donor::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string tag, id, name, email, phone, pwd, locStr, dtStr, ptsStr, mealsStr, kgStr, verStr, dateStr, badgeStr;
    
    std::getline(ss, tag, '|');
    if (tag != "DONOR") return nullptr;

    if (std::getline(ss, id, '|') &&
        std::getline(ss, name, '|') &&
        std::getline(ss, email, '|') &&
        std::getline(ss, phone, '|') &&
        std::getline(ss, pwd, '|') &&
        std::getline(ss, locStr, '|') &&
        std::getline(ss, dtStr, '|') &&
        std::getline(ss, ptsStr, '|') &&
        std::getline(ss, mealsStr, '|') &&
        std::getline(ss, kgStr, '|') &&
        std::getline(ss, verStr, '|') &&
        std::getline(ss, dateStr, '|')) {
        
        Location loc = Location::deserialize(locStr);
        DonorType dt = static_cast<DonorType>(std::stoi(dtStr));
        int pts = std::stoi(ptsStr);
        int meals = std::stoi(mealsStr);
        double kg = std::stod(kgStr);
        bool ver = (verStr == "1");

        auto donor = std::make_shared<Donor>(id, name, email, phone, pwd, loc, dt, pts, meals, kg, ver, dateStr);

        if (std::getline(ss, badgeStr, '|')) {
            std::stringstream bss(badgeStr);
            std::string b;
            while (std::getline(bss, b, ',')) {
                if (!b.empty()) donor->m_badges.push_back(b);
            }
        }
        return donor;
    }
    return nullptr;
}

} // namespace ANNADATU
