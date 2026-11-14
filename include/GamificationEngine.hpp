#ifndef ANNADATU_GAMIFICATIONENGINE_HPP
#define ANNADATU_GAMIFICATIONENGINE_HPP

#include "Common.hpp"
#include "Donor.hpp"

namespace ANNADATU {

struct LeaderboardEntry {
    std::string donorId;
    std::string name;
    std::string donorTypeStr;
    int points;
    int mealsDonated;
    double kgSaved;
    int badgesCount;

    bool operator<(const LeaderboardEntry& other) const {
        if (this->points != other.points) {
            return this->points > other.points; // Descending
        }
        return this->mealsDonated > other.mealsDonated;
    }
};

class GamificationEngine {
public:
    // Calculates karma points to reward for a completed rescue
    static int calculatePoints(int mealsCount, double kgAmount, FoodType foodType, bool fastHandover = false);

    // Calculates environmental savings
    static double calculateCarbonOffsetKg(double kgSaved);
    static double calculateWaterSavedLiters(double kgSaved);

    // Generates a sorted leaderboard from a list of donors
    static std::vector<LeaderboardEntry> generateLeaderboard(const std::vector<std::shared_ptr<Donor>>& donors);

    // Displays the formatted leaderboard banner
    static void printLeaderboard(const std::vector<LeaderboardEntry>& board);
};

} // namespace ANNADATU

#endif // ANNADATU_GAMIFICATIONENGINE_HPP
