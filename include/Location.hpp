#ifndef ANNADATU_LOCATION_HPP
#define ANNADATU_LOCATION_HPP

#include "Common.hpp"

namespace ANNADATU {

class Location {
private:
    std::string m_address;
    std::string m_area;
    std::string m_city;
    double m_latitude;
    double m_longitude;

public:
    // Default constructor
    Location();

    // Parameterized constructor
    Location(std::string address, std::string area, std::string city, double lat, double lon);

    // Getters & Setters (Encapsulation)
    const std::string& getAddress() const { return m_address; }
    void setAddress(const std::string& addr) { m_address = addr; }

    const std::string& getArea() const { return m_area; }
    void setArea(const std::string& area) { m_area = area; }

    const std::string& getCity() const { return m_city; }
    void setCity(const std::string& city) { m_city = city; }

    double getLatitude() const { return m_latitude; }
    void setLatitude(double lat) { m_latitude = lat; }

    double getLongitude() const { return m_longitude; }
    void setLongitude(double lon) { m_longitude = lon; }

    // Distance computation (Haversine)
    double distanceTo(const Location& other) const;

    // String representations
    std::string getFullString() const;
    std::string serialize() const;
    static Location deserialize(const std::string& line);

    // Operator Overloading for stream insertion
    friend std::ostream& operator<<(std::ostream& os, const Location& loc);
};

} // namespace ANNADATU

#endif // ANNADATU_LOCATION_HPP
