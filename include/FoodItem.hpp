#ifndef ANNADATU_FOODITEM_HPP
#define ANNADATU_FOODITEM_HPP

#include "Common.hpp"

namespace ANNADATU {

class FoodItem {
private:
    std::string m_title;
    FoodType m_foodType;
    DietType m_dietType;
    double m_quantityKg;
    int m_servingsCount;
    std::string m_handlingNotes;
    std::string m_photoUrlOrTag;

public:
    FoodItem();
    FoodItem(std::string title, FoodType fType, DietType dType, double qtyKg, int servings, 
             std::string handling = "Standard hygienic container", std::string photoTag = "food_pack.jpg");

    // Getters & Setters
    const std::string& getTitle() const { return m_title; }
    void setTitle(const std::string& title) { m_title = title; }

    FoodType getFoodType() const { return m_foodType; }
    void setFoodType(FoodType ft) { m_foodType = ft; }

    DietType getDietType() const { return m_dietType; }
    void setDietType(DietType dt) { m_dietType = dt; }

    double getQuantityKg() const { return m_quantityKg; }
    void setQuantityKg(double qty);

    int getServingsCount() const { return m_servingsCount; }
    void setServingsCount(int s);

    const std::string& getHandlingNotes() const { return m_handlingNotes; }
    void setHandlingNotes(const std::string& notes) { m_handlingNotes = notes; }

    const std::string& getPhotoTag() const { return m_photoUrlOrTag; }
    void setPhotoTag(const std::string& photo) { m_photoUrlOrTag = photo; }

    // Operators
    FoodItem& operator+=(const FoodItem& rhs);
    friend std::ostream& operator<<(std::ostream& os, const FoodItem& item);

    // Serialization
    std::string serialize() const;
    static FoodItem deserialize(const std::string& line);
};

} // namespace ANNADATU

#endif // ANNADATU_FOODITEM_HPP
