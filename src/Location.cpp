#include "Location.hpp"

namespace ANNADATU {

Location::Location()
    : m_address("Unknown"), m_area("City Center"), m_city("Delhi"), m_latitude(28.6139), m_longitude(77.2090) {}

Location::Location(std::string address, std::string area, std::string city, double lat, double lon)
    : m_address(std::move(address)), m_area(std::move(area)), m_city(std::move(city)), m_latitude(lat), m_longitude(lon) {}

double Location::distanceTo(const Location& other) const {
    return calculateDistanceKm(m_latitude, m_longitude, other.m_latitude, other.m_longitude);
}

std::string Location::getFullString() const {
    std::ostringstream oss;
    oss << m_address << ", " << m_area << ", " << m_city;
    return oss.str();
}

std::string Location::serialize() const {
    // Delimited with '~'
    std::ostringstream oss;
    oss << m_address << "~" << m_area << "~" << m_city << "~" << m_latitude << "~" << m_longitude << "~";
    return oss.str();
}

Location Location::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string addr, area, city, latStr, lonStr;
    if (std::getline(ss, addr, '~') &&
        std::getline(ss, area, '~') &&
        std::getline(ss, city, '~') &&
        std::getline(ss, latStr, '~') &&
        std::getline(ss, lonStr, '~')) {
        try {
            double lat = std::stod(latStr);
            double lon = std::stod(lonStr);
            return Location(addr, area, city, lat, lon);
        } catch (...) {}
    }
    return Location();
}

std::ostream& operator<<(std::ostream& os, const Location& loc) {
    os << loc.getFullString() << " (" << std::fixed << std::setprecision(4) 
       << loc.m_latitude << ", " << loc.m_longitude << ")";
    return os;
}

} // namespace ANNADATU
