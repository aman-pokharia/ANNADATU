#ifndef ANNADATU_APP_HPP
#define ANNADATU_APP_HPP

#include "Common.hpp"
#include "User.hpp"
#include "Donor.hpp"
#include "Acceptor.hpp"
#include "DonationListing.hpp"
#include "FoodRequest.hpp"
#include "MatchingEngine.hpp"
#include "GamificationEngine.hpp"
#include "StorageManager.hpp"
#include "UIHelper.hpp"

namespace ANNADATU {

class ANNADATUApp {
private:
    std::vector<std::shared_ptr<User>> m_users;
    std::vector<DonationListing> m_listings;
    std::vector<FoodRequest> m_requests;

    std::shared_ptr<User> m_currentUser;
    StorageManager m_storage;
    MatchingEngine m_matchingEngine;

    bool m_isRunning;

    // Helper finders
    std::shared_ptr<User> findUserById(const std::string& id) const;
    std::shared_ptr<User> findUserByEmail(const std::string& email) const;
    DonationListing* findListingById(const std::string& id);

    // ID generators
    std::string generateListingId();
    std::string generateRequestId();
    std::string generateUserId(Role role);

public:
    ANNADATUApp();

    void initialize();
    void run();

    // Authentication workflows
    void handleDonorLogin();
    void handleAcceptorLogin();
    void handleRegister(Role role);
    void handleEmailLogin(Role role);

    // Portal menus
    void showDonorPortal();
    void showAcceptorPortal();
    void showGlobalImpactStats();

    // Donor actions
    void donorPostFood();
    void donorViewMyListings();
    void donorReviewRequests();

    // Acceptor actions
    void acceptorBrowseSurplus();
    void acceptorClaimDonation();
    void acceptorConfirmPickup();
    void acceptorRaiseFoodNeed();
    void acceptorViewMyNeeds();

    // Persistence
    void syncStorage();
};

} // namespace ANNADATU

#endif // ANNADATU_APP_HPP
