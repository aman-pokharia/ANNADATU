#include "StorageManager.hpp"

namespace ANNADATU {

StorageManager::StorageManager(std::string dataDir)
    : m_dataDir(std::move(dataDir)) {
    m_usersFile = m_dataDir + "/users.txt";
    m_listingsFile = m_dataDir + "/donations.txt";
    m_requestsFile = m_dataDir + "/requests.txt";
}

bool StorageManager::saveUsers(const std::vector<std::shared_ptr<User>>& users) {
    std::ofstream out(m_usersFile);
    if (!out.is_open()) return false;
    for (const auto& u : users) {
        if (u) {
            out << u->serialize() << "\n";
        }
    }
    return true;
}

std::vector<std::shared_ptr<User>> StorageManager::loadUsers() {
    std::vector<std::shared_ptr<User>> users;
    std::ifstream in(m_usersFile);
    if (!in.is_open()) return users;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line.rfind("DONOR|", 0) == 0) {
            auto d = Donor::deserialize(line);
            if (d) users.push_back(d);
        } else if (line.rfind("ACCEPTOR|", 0) == 0) {
            auto a = Acceptor::deserialize(line);
            if (a) users.push_back(a);
        }
    }
    return users;
}

bool StorageManager::saveListings(const std::vector<DonationListing>& listings) {
    std::ofstream out(m_listingsFile);
    if (!out.is_open()) return false;
    for (const auto& l : listings) {
        out << l.serialize() << "\n";
    }
    return true;
}

std::vector<DonationListing> StorageManager::loadListings() {
    std::vector<DonationListing> listings;
    std::ifstream in(m_listingsFile);
    if (!in.is_open()) return listings;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        listings.push_back(DonationListing::deserialize(line));
    }
    return listings;
}

bool StorageManager::saveRequests(const std::vector<FoodRequest>& requests) {
    std::ofstream out(m_requestsFile);
    if (!out.is_open()) return false;
    for (const auto& r : requests) {
        out << r.serialize() << "\n";
    }
    return true;
}

std::vector<FoodRequest> StorageManager::loadRequests() {
    std::vector<FoodRequest> requests;
    std::ifstream in(m_requestsFile);
    if (!in.is_open()) return requests;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        requests.push_back(FoodRequest::deserialize(line));
    }
    return requests;
}

void StorageManager::seedInitialDemoData(
    std::vector<std::shared_ptr<User>>& users,
    std::vector<DonationListing>& listings,
    std::vector<FoodRequest>& requests
) {
    // 1. Seed Donors
    Location locD1("Bikanervala Banquet, Pusa Road", "Karol Bagh", "New Delhi", 28.6448, 77.1906);
    auto donor1 = std::make_shared<Donor>(
        "USR_D101", "Bikanervala Grand Banquets", "events@bikanervala.in", "9811001122",
        "pass123", locD1, DonorType::EVENT_CATERER, 420, 240, 120.0, true, "2026-10-01 11:00"
    );

    Location locD2("Haldiram's Sweets & Restaurant", "Connaught Place", "New Delhi", 28.6328, 77.2197);
    auto donor2 = std::make_shared<Donor>(
        "USR_D102", "Haldirams CP Flagship", "cp@haldirams.com", "9811003344",
        "pass123", locD2, DonorType::RESTAURANT, 650, 480, 240.0, true, "2026-09-15 14:30"
    );

    Location locD3("Hostel 4 Mess Dining Hall", "IIT Delhi, Hauz Khas", "New Delhi", 28.5450, 77.1926);
    auto donor3 = std::make_shared<Donor>(
        "USR_D103", "IIT Delhi Mess Committee", "hostel4mess@iitd.ac.in", "9876543210",
        "pass123", locD3, DonorType::HOSTEL_MESS, 310, 190, 95.0, true, "2026-10-10 18:00"
    );

    Location locD4("Pocket B-4, Sector 7", "Rohini", "New Delhi", 28.7159, 77.1147);
    auto donor4 = std::make_shared<Donor>(
        "USR_D104", "Sharma Family Residence", "sharma.home@gmail.com", "9910223344",
        "pass123", locD4, DonorType::HOME, 80, 40, 20.0, true, "2026-11-01 09:15"
    );

    users.push_back(donor1);
    users.push_back(donor2);
    users.push_back(donor3);
    users.push_back(donor4);

    // 2. Seed Acceptors
    Location locA1("Robin Hood Army Base, Hanuman Mandir", "Connaught Place", "New Delhi", 28.6289, 77.2144);
    auto acc1 = std::make_shared<Acceptor>(
        "USR_A201", "Robin Hood Army Delhi", "delhi@robinhoodarmy.com", "9899112233",
        "pass123", locA1, AcceptorType::NGO, 250, 680, true, "2026-08-20 10:00"
    );

    Location locA2("Aasra Community Night Shelter", "Old Delhi Rly Station", "New Delhi", 28.6600, 77.2280);
    auto acc2 = std::make_shared<Acceptor>(
        "USR_A202", "Aasra Night Shelter for Homeless", "aasra.delhi@shelter.org", "9711889900",
        "pass123", locA2, AcceptorType::SHELTER_HOME, 120, 310, true, "2026-09-01 12:00"
    );

    Location locA3("Nanhe Parinde Child Care Shelter", "Lajpat Nagar IV", "New Delhi", 28.5678, 77.2433);
    auto acc3 = std::make_shared<Acceptor>(
        "USR_A203", "Nanhe Parinde Children Welfare", "support@nanheparinde.ngo", "9818776655",
        "pass123", locA3, AcceptorType::ORPHANAGE, 60, 180, true, "2026-10-05 16:45"
    );

    users.push_back(acc1);
    users.push_back(acc2);
    users.push_back(acc3);

    // 3. Seed Active Donation Listings
    // Listing 1: Cooked Shahi Paneer, Pulao & Roti from Bikanervala (Urgent, 3.5h window)
    FoodItem food1("Shahi Paneer, Jeera Pulao & Butter Roti", FoodType::COOKED, DietType::VEG, 25.0, 50,
                   "Kept in thermo-insulated food pans. Ready for hot distribution.", "bikanervala_pulao.jpg");
    DonationListing list1(
        "LST_001", donor1->getId(), donor1->getName(), food1, locD1, 4,
        getCurrentTime() - 1800, // prepared 30 mins ago
        ListingStatus::AVAILABLE, "", "", "842193", "2026-11-14 18:30"
    );

    // Listing 2: Cooked Rajma Masala & Steamed Rice from IIT Mess (5h window)
    FoodItem food2("Fresh Rajma Chawal & Seasonal Salad", FoodType::COOKED, DietType::VEG, 30.0, 60,
                   "Clean stainless steel catering containers.", "iit_rajma.jpg");
    DonationListing list2(
        "LST_002", donor3->getId(), donor3->getName(), food2, locD3, 6,
        getCurrentTime() - 3600, // prepared 1 hr ago
        ListingStatus::AVAILABLE, "", "", "519284", "2026-11-14 19:00"
    );

    // Listing 3: Assorted Sandwiches, Dhokla & Idli from Haldiram CP (2.5h window - critical urgency)
    FoodItem food3("Assorted Snack Combo: Dhokla, Idli & Veg Sandwiches", FoodType::COOKED, DietType::VEG, 15.0, 35,
                   "Boxed in individual eco-friendly kraft paper packets.", "haldiram_snacks.jpg");
    DonationListing list3(
        "LST_003", donor2->getId(), donor2->getName(), food3, locD2, 3,
        getCurrentTime() - 3600, // prepared 1 hr ago, 2 hrs left
        ListingStatus::AVAILABLE, "", "", "631902", "2026-11-14 19:30"
    );

    // Listing 4: Raw pantry grains & flour from Sharma Residence
    FoodItem food4("Pantry Surplus: Whole Wheat Atta, Toor Dal & Rice", FoodType::RAW, DietType::VEG, 18.0, 40,
                   "Sealed branded sacks, dry storage.", "pantry_flour.jpg");
    DonationListing list4(
        "LST_004", donor4->getId(), donor4->getName(), food4, locD4, 72,
        getCurrentTime() - 7200,
        ListingStatus::AVAILABLE, "", "", "774120", "2026-11-14 17:00"
    );

    listings.push_back(list1);
    listings.push_back(list2);
    listings.push_back(list3);
    listings.push_back(list4);

    // 4. Seed Food Requests
    FoodRequest req1(
        "REQ_001", acc1->getId(), acc1->getName(), 45, DietType::VEG, UrgencyLevel::HIGH,
        locA1, "Evening food drive for migrant workers under Minto Bridge", false, "2026-11-14 18:00"
    );

    FoodRequest req2(
        "REQ_002", acc2->getId(), acc2->getName(), 70, DietType::ANY, UrgencyLevel::CRITICAL,
        locA2, "Dinner serving for 70 elderly & shelter residents", false, "2026-11-14 18:45"
    );

    requests.push_back(req1);
    requests.push_back(req2);

    // Save to disk
    saveUsers(users);
    saveListings(listings);
    saveRequests(requests);
}

} // namespace ANNADATU
