#include "MatchingEngine.hpp"

namespace ANNADATU {

// UrgencyAwareMatchingStrategy implementation
std::vector<MatchCandidate> UrgencyAwareMatchingStrategy::match(
    const Location& targetLocation,
    DietType dietPref,
    int minServings,
    const std::vector<DonationListing>& allListings,
    double maxRadiusKm
) const {
    std::vector<MatchCandidate> candidates;

    for (const auto& listing : allListings) {
        // Must be Available and Not Expired
        if (listing.getStatus() != ListingStatus::AVAILABLE) continue;
        if (listing.isExpired()) continue;

        // Diet preference filter
        if (dietPref != DietType::ANY) {
            if (dietPref == DietType::VEG && listing.getFoodItem().getDietType() != DietType::VEG) {
                continue;
            }
        }

        // Distance check
        double dist = targetLocation.distanceTo(listing.getPickupLocation());
        if (dist > maxRadiusKm) continue;

        // Scoring algorithm:
        // Urgency factor: 0-100 pts inversely proportional to hours left
        double remHours = listing.getRemainingHours();
        double urgencyFactor = 0.0;
        if (remHours <= 2.0) urgencyFactor = 60.0;
        else if (remHours <= 4.0) urgencyFactor = 40.0;
        else urgencyFactor = 20.0;

        // Proximity factor: closer = higher score (max 40 pts)
        double proximityFactor = std::max(0.0, (maxRadiusKm - dist) / maxRadiusKm * 40.0);

        // Quantity fit bonus
        double qtyBonus = 0.0;
        if (listing.getFoodItem().getServingsCount() >= minServings) {
            qtyBonus = 10.0;
        }

        double totalScore = urgencyFactor + proximityFactor + qtyBonus;

        std::ostringstream note;
        note << std::fixed << std::setprecision(1) << dist << " km away | "
             << (remHours > 0 ? remHours : 0.0) << "h freshness left";

        candidates.push_back({listing, dist, totalScore, note.str()});
    }

    std::sort(candidates.begin(), candidates.end());
    return candidates;
}

// ProximityFirstStrategy implementation
std::vector<MatchCandidate> ProximityFirstStrategy::match(
    const Location& targetLocation,
    DietType dietPref,
    int minServings,
    const std::vector<DonationListing>& allListings,
    double maxRadiusKm
) const {
    std::vector<MatchCandidate> candidates;

    for (const auto& listing : allListings) {
        if (listing.getStatus() != ListingStatus::AVAILABLE) continue;
        if (listing.isExpired()) continue;

        if (dietPref != DietType::ANY) {
            if (dietPref == DietType::VEG && listing.getFoodItem().getDietType() != DietType::VEG) {
                continue;
            }
        }

        double dist = targetLocation.distanceTo(listing.getPickupLocation());
        if (dist > maxRadiusKm) continue;

        double score = 1000.0 - dist * 10.0; // Pure distance rank

        std::ostringstream note;
        note << std::fixed << std::setprecision(1) << dist << " km direct distance";

        candidates.push_back({listing, dist, score, note.str()});
    }

    std::sort(candidates.begin(), candidates.end());
    return candidates;
}

// MatchingEngine Context methods
MatchingEngine::MatchingEngine()
    : m_strategy(std::make_unique<UrgencyAwareMatchingStrategy>()) {}

MatchingEngine::MatchingEngine(std::unique_ptr<IMatchingStrategy> strategy)
    : m_strategy(std::move(strategy)) {}

void MatchingEngine::setStrategy(std::unique_ptr<IMatchingStrategy> strategy) {
    m_strategy = std::move(strategy);
}

std::vector<MatchCandidate> MatchingEngine::findNearbySurplus(
    const Location& acceptorLoc,
    DietType dietPref,
    int minServings,
    const std::vector<DonationListing>& listings,
    double maxRadiusKm
) const {
    if (!m_strategy) return {};
    return m_strategy->match(acceptorLoc, dietPref, minServings, listings, maxRadiusKm);
}

} // namespace ANNADATU
