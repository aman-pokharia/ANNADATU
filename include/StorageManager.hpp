#ifndef ANNADATU_STORAGEMANAGER_HPP
#define ANNADATU_STORAGEMANAGER_HPP

#include "Common.hpp"
#include "User.hpp"
#include "Donor.hpp"
#include "Acceptor.hpp"
#include "DonationListing.hpp"
#include "FoodRequest.hpp"
#include <fstream>

namespace ANNADATU {

class StorageManager {
private:
    std::string m_dataDir;
    std::string m_usersFile;
    std::string m_listingsFile;
    std::string m_requestsFile;

public:
    explicit StorageManager(std::string dataDir = "data");

    // File Persistence
    bool saveUsers(const std::vector<std::shared_ptr<User>>& users);
    std::vector<std::shared_ptr<User>> loadUsers();

    bool saveListings(const std::vector<DonationListing>& listings);
    std::vector<DonationListing> loadListings();

    bool saveRequests(const std::vector<FoodRequest>& requests);
    std::vector<FoodRequest> loadRequests();

    // Seed Demo Data if repository is brand new
    void seedInitialDemoData(
        std::vector<std::shared_ptr<User>>& users,
        std::vector<DonationListing>& listings,
        std::vector<FoodRequest>& requests
    );
};

} // namespace ANNADATU

#endif // ANNADATU_STORAGEMANAGER_HPP
