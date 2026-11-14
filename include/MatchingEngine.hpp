#ifndef ANNADATU_MATCHINGENGINE_HPP
#define ANNADATU_MATCHINGENGINE_HPP

#include "Common.hpp"
#include "DonationListing.hpp"
#include "FoodRequest.hpp"

namespace ANNADATU {

struct MatchCandidate {
    DonationListing listing;
    double distanceKm;
    double matchScore;
    std::string highlightNote;

    bool operator<(const MatchCandidate& other) const {
        return this->matchScore > other.matchScore; // descending
    }
};

// Strategy Pattern Interface (OOP Design Pattern)
class IMatchingStrategy {
public:
    virtual ~IMatchingStrategy() = default;
    virtual std::vector<MatchCandidate> match(
        const Location& targetLocation,
        DietType dietPref,
        int minServings,
        const std::vector<DonationListing>& allListings,
        double maxRadiusKm = 25.0
    ) const = 0;

    virtual std::string getStrategyName() const = 0;
};

// Concrete Strategy 1: Urgency-Aware Matching (Balances proximity with freshness countdown)
class UrgencyAwareMatchingStrategy : public IMatchingStrategy {
public:
    std::vector<MatchCandidate> match(
        const Location& targetLocation,
        DietType dietPref,
        int minServings,
        const std::vector<DonationListing>& allListings,
        double maxRadiusKm = 25.0
    ) const override;

    std::string getStrategyName() const override {
        return "Urgency & Freshness-First Hyperlocal Strategy";
    }
};

// Concrete Strategy 2: Proximity-First Strategy (Pure physical distance ranking)
class ProximityFirstStrategy : public IMatchingStrategy {
public:
    std::vector<MatchCandidate> match(
        const Location& targetLocation,
        DietType dietPref,
        int minServings,
        const std::vector<DonationListing>& allListings,
        double maxRadiusKm = 25.0
    ) const override;

    std::string getStrategyName() const override {
        return "Shortest Proximity Distance Strategy";
    }
};

// Matching Engine Context
class MatchingEngine {
private:
    std::unique_ptr<IMatchingStrategy> m_strategy;

public:
    MatchingEngine();
    explicit MatchingEngine(std::unique_ptr<IMatchingStrategy> strategy);

    void setStrategy(std::unique_ptr<IMatchingStrategy> strategy);
    const IMatchingStrategy* getStrategy() const { return m_strategy.get(); }

    std::vector<MatchCandidate> findNearbySurplus(
        const Location& acceptorLoc,
        DietType dietPref,
        int minServings,
        const std::vector<DonationListing>& listings,
        double maxRadiusKm = 25.0
    ) const;
};

} // namespace ANNADATU

#endif // ANNADATU_MATCHINGENGINE_HPP
