#include "GamificationEngine.hpp"

namespace ANNADATU {

int GamificationEngine::calculatePoints(int mealsCount, double kgAmount, FoodType foodType, bool fastHandover) {
    int points = mealsCount * 10;
    if (foodType == FoodType::COOKED) {
        points += 20; // Extra bonus for saving perishable hot meals
    }
    if (kgAmount >= 20.0) {
        points += 50; // Bulk rescue bonus
    }
    if (fastHandover) {
        points += 30; // Quick handover prompt bonus
    }
    return points;
}

double GamificationEngine::calculateCarbonOffsetKg(double kgSaved) {
    return kgSaved * 2.5; // 2.5 kg CO2e averted per kg of food waste saved
}

double GamificationEngine::calculateWaterSavedLiters(double kgSaved) {
    return kgSaved * 800.0; // ~800 liters virtual water footprint saved
}

std::vector<LeaderboardEntry> GamificationEngine::generateLeaderboard(const std::vector<std::shared_ptr<Donor>>& donors) {
    std::vector<LeaderboardEntry> entries;
    for (const auto& d : donors) {
        if (!d) continue;
        LeaderboardEntry entry;
        entry.donorId = d->getId();
        entry.name = d->getName();
        entry.donorTypeStr = d->getRoleSpecificType();
        entry.points = d->getRewardPoints();
        entry.mealsDonated = d->getTotalMealsDonated();
        entry.kgSaved = d->getTotalKgSaved();
        entry.badgesCount = static_cast<int>(d->getBadges().size());
        entries.push_back(entry);
    }
    std::sort(entries.begin(), entries.end());
    return entries;
}

void GamificationEngine::printLeaderboard(const std::vector<LeaderboardEntry>& board) {
    std::cout << "\n========================================================================================\n";
    std::cout << "                        ANNADATU HEROES - COMMUNITY LEADERBOARD                         \n";
    std::cout << "                            'Every meal saved is a meal served!'                        \n";
    std::cout << "========================================================================================\n";
    std::cout << std::left 
              << std::setw(6)  << "RANK"
              << std::setw(26) << "DONOR / ENTITY"
              << std::setw(22) << "TYPE"
              << std::setw(12) << "MEALS SAVED"
              << std::setw(12) << "KG RESCUED"
              << std::setw(10) << "KARMA PTS"
              << "BADGES\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    if (board.empty()) {
        std::cout << "   No donors registered yet. Be the first hero on the leaderboard!\n";
    } else {
        int rank = 1;
        for (const auto& entry : board) {
            std::string rankDisplay;
            if (rank == 1) rankDisplay = "[#1]";
            else if (rank == 2) rankDisplay = "[#2]";
            else if (rank == 3) rankDisplay = "[#3]";
            else rankDisplay = " #" + std::to_string(rank);

            std::cout << std::left
                      << std::setw(6)  << rankDisplay
                      << std::setw(26) << entry.name.substr(0, 24)
                      << std::setw(22) << entry.donorTypeStr.substr(0, 20)
                      << std::setw(12) << entry.mealsDonated
                      << std::setw(12) << (std::to_string((int)entry.kgSaved) + " kg")
                      << std::setw(10) << entry.points
                      << entry.badgesCount << " badges\n";
            rank++;
        }
    }
    std::cout << "========================================================================================\n";
}

} // namespace ANNADATU
