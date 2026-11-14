#ifndef ANNADATU_COMMON_HPP
#define ANNADATU_COMMON_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <random>

namespace ANNADATU {

// User Roles in the Two-Sided Marketplace
enum class Role {
    DONOR,
    ACCEPTOR,
    ADMIN
};

// Food Categories
enum class FoodType {
    RAW,        // Grains, vegetables, pantry surplus, packaged
    COOKED      // Prepared meals from restaurants, events, hostels
};

// Dietary Preference
enum class DietType {
    VEG,
    NON_VEG,
    EGG,
    ANY
};

// Urgency Classification for Matching
enum class UrgencyLevel {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4
};

// Listing Lifecycle Status
enum class ListingStatus {
    AVAILABLE,
    REQUESTED,
    MATCHED,
    COMPLETED,
    EXPIRED,
    CANCELLED
};

// Types of Donors
enum class DonorType {
    HOME,
    RESTAURANT,
    EVENT_CATERER,
    HOSTEL_MESS,
    BAKERY,
    SUPERMARKET
};

// Types of Acceptors
enum class AcceptorType {
    NGO,
    SHELTER_HOME,
    COMMUNITY_KITCHEN,
    ORPHANAGE,
    INDIVIDUAL_NEED
};

// Custom Domain Exceptions
class ANNADATUException : public std::runtime_error {
public:
    explicit ANNADATUException(const std::string& message) : std::runtime_error(message) {}
};

class ValidationException : public ANNADATUException {
public:
    explicit ValidationException(const std::string& message) : ANNADATUException("[Validation Error] " + message) {}
};

class AuthException : public ANNADATUException {
public:
    explicit AuthException(const std::string& message) : ANNADATUException("[Authentication Error] " + message) {}
};

class ListingException : public ANNADATUException {
public:
    explicit ListingException(const std::string& message) : ANNADATUException("[Listing Error] " + message) {}
};

// String conversion helpers
inline std::string roleToString(Role r) {
    switch (r) {
        case Role::DONOR: return "Donor";
        case Role::ACCEPTOR: return "Acceptor";
        case Role::ADMIN: return "Admin";
        default: return "Unknown";
    }
}

inline std::string foodTypeToString(FoodType t) {
    switch (t) {
        case FoodType::RAW: return "Raw / Groceries";
        case FoodType::COOKED: return "Cooked / Prepared";
        default: return "Unknown";
    }
}

inline std::string dietTypeToString(DietType d) {
    switch (d) {
        case DietType::VEG: return "Pure Veg (Green)";
        case DietType::NON_VEG: return "Non-Veg (Red)";
        case DietType::EGG: return "Eggetarian";
        case DietType::ANY: return "Any Diet";
        default: return "Unknown";
    }
}

inline std::string urgencyToString(UrgencyLevel u) {
    switch (u) {
        case UrgencyLevel::LOW: return "Low";
        case UrgencyLevel::MEDIUM: return "Medium";
        case UrgencyLevel::HIGH: return "High";
        case UrgencyLevel::CRITICAL: return "CRITICAL (Immediate)";
        default: return "Normal";
    }
}

inline std::string listingStatusToString(ListingStatus s) {
    switch (s) {
        case ListingStatus::AVAILABLE: return "Available";
        case ListingStatus::REQUESTED: return "Request Pending";
        case ListingStatus::MATCHED: return "Matched (Pickup in progress)";
        case ListingStatus::COMPLETED: return "Completed (Delivered)";
        case ListingStatus::EXPIRED: return "Expired";
        case ListingStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

inline std::string donorTypeToString(DonorType dt) {
    switch (dt) {
        case DonorType::HOME: return "Individual / Home";
        case DonorType::RESTAURANT: return "Restaurant / Cafe";
        case DonorType::EVENT_CATERER: return "Wedding / Event Caterer";
        case DonorType::HOSTEL_MESS: return "Hostel / College Mess";
        case DonorType::BAKERY: return "Bakery / Confectionery";
        case DonorType::SUPERMARKET: return "Grocery / Supermarket";
        default: return "Food Provider";
    }
}

inline std::string acceptorTypeToString(AcceptorType at) {
    switch (at) {
        case AcceptorType::NGO: return "Registered NGO";
        case AcceptorType::SHELTER_HOME: return "Shelter Home";
        case AcceptorType::COMMUNITY_KITCHEN: return "Community Kitchen / Langar";
        case AcceptorType::ORPHANAGE: return "Orphanage / Elderly Care";
        case AcceptorType::INDIVIDUAL_NEED: return "Individual in Need";
        default: return "Beneficiary";
    }
}

// Current Unix timestamp
inline std::time_t getCurrentTime() {
    return std::time(nullptr);
}

// Formats timestamp to readable string
inline std::string formatTimestamp(std::time_t t) {
    std::tm tm_buf;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_buf, &t);
#else
    localtime_r(&t, &tm_buf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M");
    return oss.str();
}

// Generates secure 6-digit OTP for trust-by-design handover
inline std::string generateOTP() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100000, 999999);
    return std::to_string(dis(gen));
}

// Helper to calculate great-circle distance (Haversine formula) in kilometers
inline double calculateDistanceKm(double lat1, double lon1, double lat2, double lon2) {
    constexpr double R = 6371.0; // Earth's mean radius in km
    constexpr double PI = 3.14159265358979323846;

    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;

    double rLat1 = lat1 * PI / 180.0;
    double rLat2 = lat2 * PI / 180.0;

    double a = std::sin(dLat / 2.0) * std::sin(dLat / 2.0) +
               std::cos(rLat1) * std::cos(rLat2) *
               std::sin(dLon / 2.0) * std::sin(dLon / 2.0);
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return R * c;
}

} // namespace ANNADATU

#endif // ANNADATU_COMMON_HPP
