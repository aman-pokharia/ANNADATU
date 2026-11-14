#include "FoodItem.hpp"

namespace ANNADATU {

FoodItem::FoodItem()
    : m_title("Assorted Edible Food"), m_foodType(FoodType::COOKED), m_dietType(DietType::VEG),
      m_quantityKg(5.0), m_servingsCount(10), m_handlingNotes("Covered containers"), m_photoUrlOrTag("sample_pack.jpg") {}

FoodItem::FoodItem(std::string title, FoodType fType, DietType dType, double qtyKg, int servings,
                   std::string handling, std::string photoTag)
    : m_title(std::move(title)), m_foodType(fType), m_dietType(dType),
      m_quantityKg(qtyKg), m_servingsCount(servings),
      m_handlingNotes(std::move(handling)), m_photoUrlOrTag(std::move(photoTag)) {
    if (m_quantityKg <= 0.0) m_quantityKg = 1.0;
    if (m_servingsCount <= 0) m_servingsCount = 1;
}

void FoodItem::setQuantityKg(double qty) {
    if (qty < 0.0) throw ValidationException("Food quantity cannot be negative.");
    m_quantityKg = qty;
}

void FoodItem::setServingsCount(int s) {
    if (s < 0) throw ValidationException("Servings count cannot be negative.");
    m_servingsCount = s;
}

FoodItem& FoodItem::operator+=(const FoodItem& rhs) {
    this->m_quantityKg += rhs.m_quantityKg;
    this->m_servingsCount += rhs.m_servingsCount;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const FoodItem& item) {
    os << item.m_title << " (" << foodTypeToString(item.m_foodType) << ", "
       << dietTypeToString(item.m_dietType) << ") - "
       << item.m_quantityKg << " kg (~" << item.m_servingsCount << " servings)";
    return os;
}

std::string FoodItem::serialize() const {
    std::ostringstream oss;
    oss << m_title << "~"
        << static_cast<int>(m_foodType) << "~"
        << static_cast<int>(m_dietType) << "~"
        << m_quantityKg << "~"
        << m_servingsCount << "~"
        << m_handlingNotes << "~"
        << m_photoUrlOrTag << "~";
    return oss.str();
}

FoodItem FoodItem::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string title, ftStr, dtStr, qtyStr, servStr, handling, photo;
    if (std::getline(ss, title, '~') &&
        std::getline(ss, ftStr, '~') &&
        std::getline(ss, dtStr, '~') &&
        std::getline(ss, qtyStr, '~') &&
        std::getline(ss, servStr, '~') &&
        std::getline(ss, handling, '~')) {
        std::getline(ss, photo, '~');
        try {
            FoodType ft = static_cast<FoodType>(std::stoi(ftStr));
            DietType dt = static_cast<DietType>(std::stoi(dtStr));
            double qty = std::stod(qtyStr);
            int serv = std::stoi(servStr);
            return FoodItem(title, ft, dt, qty, serv, handling, photo);
        } catch (...) {}
    }
    return FoodItem();
}

} // namespace ANNADATU
