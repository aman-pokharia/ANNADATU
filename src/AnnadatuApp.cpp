#include "ANNADATUApp.hpp"

namespace ANNADATU {

ANNADATUApp::ANNADATUApp()
    : m_currentUser(nullptr), m_storage("data"), m_matchingEngine(), m_isRunning(true) {}

void ANNADATUApp::initialize() {
    m_users = m_storage.loadUsers();
    m_listings = m_storage.loadListings();
    m_requests = m_storage.loadRequests();

    if (m_users.empty()) {
        m_storage.seedInitialDemoData(m_users, m_listings, m_requests);
    }
}

std::shared_ptr<User> ANNADATUApp::findUserById(const std::string& id) const {
    for (const auto& u : m_users) {
        if (u && u->getId() == id) return u;
    }
    return nullptr;
}

std::shared_ptr<User> ANNADATUApp::findUserByEmail(const std::string& email) const {
    for (const auto& u : m_users) {
        if (u && u->getEmail() == email) return u;
    }
    return nullptr;
}

DonationListing* ANNADATUApp::findListingById(const std::string& id) {
    for (auto& l : m_listings) {
        if (l.getId() == id) return &l;
    }
    return nullptr;
}

std::string ANNADATUApp::generateListingId() {
    return "LST_" + std::to_string(100 + m_listings.size() + 1);
}

std::string ANNADATUApp::generateRequestId() {
    return "REQ_" + std::to_string(200 + m_requests.size() + 1);
}

std::string ANNADATUApp::generateUserId(Role role) {
    std::string prefix = (role == Role::DONOR) ? "USR_D" : "USR_A";
    return prefix + std::to_string(300 + m_users.size() + 1);
}

void ANNADATUApp::syncStorage() {
    m_storage.saveUsers(m_users);
    m_storage.saveListings(m_listings);
    m_storage.saveRequests(m_requests);
}

void ANNADATUApp::run() {
    initialize();

    while (m_isRunning) {
        UIHelper::clearScreen();
        UIHelper::printMainBanner();
        UIHelper::printTeamCredits();

        if (m_currentUser) {
            if (m_currentUser->getRole() == Role::DONOR) {
                showDonorPortal();
            } else if (m_currentUser->getRole() == Role::ACCEPTOR) {
                showAcceptorPortal();
            }
        } else {
            std::cout << "\n+-------------------------------------------------------------+\n";
            std::cout << "|                      WELCOME TO ANNADATU                    |\n";
            std::cout << "+-------------------------------------------------------------+\n";
            std::cout << " 1. Login as Donor    (Restaurants, Banquets, Hostels, Homes)\n";
            std::cout << " 2. Login as Acceptor (NGOs, Shelters, Community Kitchens)\n";
            std::cout << " 3. Register New Donor\n";
            std::cout << " 4. Register New Acceptor\n";
            std::cout << " 5. View Food Waste Problem & Impact Stats\n";
            std::cout << " 6. View Donor Leaderboard\n";
            std::cout << " 7. Exit Program\n";
            std::cout << "+-------------------------------------------------------------+\n";

            int choice = UIHelper::getMenuChoice(1, 7);
            switch (choice) {
                case 1: handleDonorLogin(); break;
                case 2: handleAcceptorLogin(); break;
                case 3: handleRegister(Role::DONOR); break;
                case 4: handleRegister(Role::ACCEPTOR); break;
                case 5: showGlobalImpactStats(); break;
                case 6: {
                    std::vector<std::shared_ptr<Donor>> donors;
                    for (const auto& u : m_users) {
                        if (u && u->getRole() == Role::DONOR) {
                            donors.push_back(std::dynamic_pointer_cast<Donor>(u));
                        }
                    }
                    auto lb = GamificationEngine::generateLeaderboard(donors);
                    GamificationEngine::printLeaderboard(lb);
                    UIHelper::pause();
                    break;
                }
                case 7: {
                    UIHelper::printInfo("Thank you for supporting ANNADATU - Zero Waste, Zero Hunger!");
                    m_isRunning = false;
                    break;
                }
            }
        }
    }
}

void ANNADATUApp::handleDonorLogin() {
    UIHelper::printSectionHeader("DONOR LOGIN");
    std::vector<std::shared_ptr<Donor>> donorList;
    for (const auto& u : m_users) {
        if (u && u->getRole() == Role::DONOR) {
            donorList.push_back(std::dynamic_pointer_cast<Donor>(u));
        }
    }

    std::cout << "Choose an existing Donor profile or login with credentials:\n";
    for (size_t i = 0; i < donorList.size(); ++i) {
        std::cout << " [" << (i + 1) << "] " << donorList[i]->getName()
                  << " (" << donorList[i]->getRoleSpecificType() << ")\n";
    }
    std::cout << " [" << (donorList.size() + 1) << "] Enter Email & Password\n";
    std::cout << " [0] Return to Main Menu\n";

    int opt = UIHelper::getMenuChoice(0, static_cast<int>(donorList.size() + 1));
    if (opt == 0) return;
    if (opt >= 1 && opt <= static_cast<int>(donorList.size())) {
        m_currentUser = donorList[opt - 1];
        UIHelper::printSuccess("Logged in as " + m_currentUser->getName());
        UIHelper::pause();
    } else {
        handleEmailLogin(Role::DONOR);
    }
}

void ANNADATUApp::handleAcceptorLogin() {
    UIHelper::printSectionHeader("ACCEPTOR / NGO LOGIN");
    std::vector<std::shared_ptr<Acceptor>> accList;
    for (const auto& u : m_users) {
        if (u && u->getRole() == Role::ACCEPTOR) {
            accList.push_back(std::dynamic_pointer_cast<Acceptor>(u));
        }
    }

    std::cout << "Choose an existing Acceptor profile or login with credentials:\n";
    for (size_t i = 0; i < accList.size(); ++i) {
        std::cout << " [" << (i + 1) << "] " << accList[i]->getName()
                  << " (" << accList[i]->getRoleSpecificType() << ")\n";
    }
    std::cout << " [" << (accList.size() + 1) << "] Enter Email & Password\n";
    std::cout << " [0] Return to Main Menu\n";

    int opt = UIHelper::getMenuChoice(0, static_cast<int>(accList.size() + 1));
    if (opt == 0) return;
    if (opt >= 1 && opt <= static_cast<int>(accList.size())) {
        m_currentUser = accList[opt - 1];
        UIHelper::printSuccess("Logged in as " + m_currentUser->getName());
        UIHelper::pause();
    } else {
        handleEmailLogin(Role::ACCEPTOR);
    }
}

void ANNADATUApp::handleEmailLogin(Role role) {
    UIHelper::printSectionHeader(role == Role::DONOR ? "DONOR CREDENTIAL LOGIN" : "ACCEPTOR CREDENTIAL LOGIN");
    std::string email = UIHelper::readLine("Enter registered email: ");
    std::string password = UIHelper::readLine("Enter password: ");

    auto user = findUserByEmail(email);
    if (user && user->verifyPassword(password) && user->getRole() == role) {
        m_currentUser = user;
        UIHelper::printSuccess("Welcome back, " + user->getName() + "!");
    } else {
        UIHelper::printError("Invalid email, password, or account type.");
    }
    UIHelper::pause();
}

void ANNADATUApp::handleRegister(Role role) {
    UIHelper::printSectionHeader(role == Role::DONOR ? "REGISTER AS SURPLUS FOOD DONOR" : "REGISTER AS FOOD ACCEPTOR / NGO");

    std::string name = UIHelper::readLine("Enter Entity / Organization Name: ");
    std::string email = UIHelper::readLine("Enter Contact Email: ");
    if (findUserByEmail(email)) {
        UIHelper::printError("An account with this email already exists!");
        UIHelper::pause();
        return;
    }
    std::string phone = UIHelper::readLine("Enter Contact Phone Number: ");
    std::string password = UIHelper::readLine("Choose a secure password: ");
    std::string addr = UIHelper::readLine("Street Address / Landmark: ");
    std::string area = UIHelper::readLine("Locality / Area: ");
    std::string city = UIHelper::readLine("City: ");
    double lat = UIHelper::readDouble("Approx Latitude (e.g. 28.6139 for Delhi): ", -90.0);
    double lon = UIHelper::readDouble("Approx Longitude (e.g. 77.2090 for Delhi): ", -180.0);

    Location loc(addr, area, city, lat, lon);
    std::string uid = generateUserId(role);

    if (role == Role::DONOR) {
        std::cout << "\nSelect Donor Category:\n";
        std::cout << " 1. Home / Individual\n 2. Restaurant / Cafe\n 3. Event / Wedding Caterer\n 4. Hostel / University Mess\n 5. Bakery / Supermarket\n";
        int t = UIHelper::getMenuChoice(1, 5);
        DonorType dt = DonorType::HOME;
        if (t == 2) dt = DonorType::RESTAURANT;
        else if (t == 3) dt = DonorType::EVENT_CATERER;
        else if (t == 4) dt = DonorType::HOSTEL_MESS;
        else if (t == 5) dt = DonorType::BAKERY;

        auto donor = std::make_shared<Donor>(uid, name, email, phone, password, loc, dt, 50, 0, 0.0, true);
        m_users.push_back(donor);
        m_currentUser = donor;
        UIHelper::printSuccess("Registration Complete! +50 Welcome Karma Points awarded!");
    } else {
        std::cout << "\nSelect Acceptor Category:\n";
        std::cout << " 1. Registered NGO\n 2. Shelter Home\n 3. Community Kitchen / Langar\n 4. Orphanage\n 5. Individual in Need\n";
        int t = UIHelper::getMenuChoice(1, 5);
        AcceptorType at = AcceptorType::NGO;
        if (t == 2) at = AcceptorType::SHELTER_HOME;
        else if (t == 3) at = AcceptorType::COMMUNITY_KITCHEN;
        else if (t == 4) at = AcceptorType::ORPHANAGE;
        else if (t == 5) at = AcceptorType::INDIVIDUAL_NEED;

        int capacity = UIHelper::readInt("Estimated people / beneficiaries served daily: ", 1);
        auto acceptor = std::make_shared<Acceptor>(uid, name, email, phone, password, loc, at, capacity, 0, true);
        m_users.push_back(acceptor);
        m_currentUser = acceptor;
        UIHelper::printSuccess("Registration Complete! Verified partner account activated.");
    }

    syncStorage();
    UIHelper::pause();
}

void ANNADATUApp::showGlobalImpactStats() {
    UIHelper::clearScreen();
    std::cout << "========================================================================================\n";
    std::cout << "                       ANNADATU MISSION: THE PARADOX OF PLENTY                          \n";
    std::cout << "========================================================================================\n";
    std::cout << " INDIA'S FOOD-WASTE REALITY (UNEP Food Waste Index & GHI Reports):\n";
    std::cout << "  * 78 - 80 Million tonnes of food is wasted every single year in India.\n";
    std::cout << "  * Rs 1.55 Lakh Crore estimated annual financial value wasted (~1% of National GDP).\n";
    std::cout << "  * ~19 Crore individuals remain undernourished daily.\n";
    std::cout << "  * Food waste contributes to 8 - 10% of global greenhouse emissions.\n\n";
    std::cout << " OUR MOTIVE: 'TO BUILD THIS BRIDGE'\n";
    std::cout << "  The surplus exists, but hunger persists. ANNADATU turns this into a solvable\n";
    std::cout << "  hyperlocal matching problem through Trust by Design, OTP verification & Speed!\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    // Calculate dynamic stats from all current users & listings
    int totalMealsSaved = 0;
    double totalKgSaved = 0.0;
    int activeListings = 0;
    int completedListings = 0;

    for (const auto& u : m_users) {
        if (u && u->getRole() == Role::DONOR) {
            auto d = std::dynamic_pointer_cast<Donor>(u);
            if (d) {
                totalMealsSaved += d->getTotalMealsDonated();
                totalKgSaved += d->getTotalKgSaved();
            }
        }
    }

    for (const auto& l : m_listings) {
        if (l.getStatus() == ListingStatus::AVAILABLE) activeListings++;
        if (l.getStatus() == ListingStatus::COMPLETED) completedListings++;
    }

    std::cout << " LIVE ANNADATU PLATFORM METRICS:\n";
    std::cout << "  - Total Meals Rescued & Served:     " << totalMealsSaved << " meals\n";
    std::cout << "  - Total Food Weight Diverted:       " << std::fixed << std::setprecision(1) << totalKgSaved << " kg\n";
    std::cout << "  - Estimated CO2e Emissions Averted: " << GamificationEngine::calculateCarbonOffsetKg(totalKgSaved) << " kg CO2e\n";
    std::cout << "  - Virtual Water Footprint Saved:    " << GamificationEngine::calculateWaterSavedLiters(totalKgSaved) << " Liters\n";
    std::cout << "  - Active Surplus Food Listings Now: " << activeListings << " live offerings\n";
    std::cout << "  - Successfully Completed Handouts:  " << completedListings << " handovers\n";
    std::cout << "========================================================================================\n";
    UIHelper::pause();
}

// -------------------------------------------------------------
// DONOR PORTAL
// -------------------------------------------------------------
void ANNADATUApp::showDonorPortal() {
    auto donor = std::dynamic_pointer_cast<Donor>(m_currentUser);
    if (!donor) return;

    donor->showDashboard();

    std::cout << "\nDONOR PORTAL ACTIONS:\n";
    std::cout << " 1. Post a New Surplus Food Listing\n";
    std::cout << " 2. View My Active Food Listings\n";
    std::cout << " 3. Review & Approve Incoming Requests\n";
    std::cout << " 4. View My Donor Profile & Badges\n";
    std::cout << " 5. View Community Leaderboard\n";
    std::cout << " 6. Logout to Main Menu\n";

    int choice = UIHelper::getMenuChoice(1, 6);
    switch (choice) {
        case 1: donorPostFood(); break;
        case 2: donorViewMyListings(); break;
        case 3: donorReviewRequests(); break;
        case 4: donor->displayProfile(); UIHelper::pause(); break;
        case 5: {
            std::vector<std::shared_ptr<Donor>> donors;
            for (const auto& u : m_users) {
                if (u && u->getRole() == Role::DONOR) {
                    donors.push_back(std::dynamic_pointer_cast<Donor>(u));
                }
            }
            auto lb = GamificationEngine::generateLeaderboard(donors);
            GamificationEngine::printLeaderboard(lb);
            UIHelper::pause();
            break;
        }
        case 6: {
            m_currentUser = nullptr;
            UIHelper::printInfo("Logged out successfully.");
            UIHelper::pause();
            break;
        }
    }
}

void ANNADATUApp::donorPostFood() {
    UIHelper::printSectionHeader("POST SURPLUS FOOD DONATION");

    std::string title = UIHelper::readLine("Food Item Name / Description: ");

    std::cout << "Food Preparation Type:\n 1. Cooked / Hot Meals\n 2. Raw / Grains / Packaged Pantry\n";
    int ftChoice = UIHelper::getMenuChoice(1, 2);
    FoodType ft = (ftChoice == 1) ? FoodType::COOKED : FoodType::RAW;

    std::cout << "Dietary Classification:\n 1. Pure Veg (Green)\n 2. Non-Veg (Red)\n 3. Eggetarian\n";
    int dtChoice = UIHelper::getMenuChoice(1, 3);
    DietType dt = DietType::VEG;
    if (dtChoice == 2) dt = DietType::NON_VEG;
    else if (dtChoice == 3) dt = DietType::EGG;

    double qtyKg = UIHelper::readDouble("Estimated Weight in Kg: ", 0.5);
    int servings = UIHelper::readInt("Approximate Number of Servings / Meals: ", 1);
    int expiryHours = UIHelper::readInt("Safe Consumption Window (Hours until expiry, e.g. 4): ", 1);
    std::string handling = UIHelper::readLine("Storage & Handling Instructions (e.g. 'Insulated containers', or press Enter): ");
    if (handling.empty()) handling = "Hygienic covered packaging";

    FoodItem item(title, ft, dt, qtyKg, servings, handling);
    std::string listId = generateListingId();

    DonationListing listing(listId, m_currentUser->getId(), m_currentUser->getName(),
                            item, m_currentUser->getLocation(), expiryHours,
                            getCurrentTime(), ListingStatus::AVAILABLE, "", "", "");

    m_listings.push_back(listing);
    syncStorage();

    UIHelper::printSuccess("Food donation listing [" + listId + "] published successfully!");
    UIHelper::pause();
}

void ANNADATUApp::donorViewMyListings() {
    UIHelper::printSectionHeader("MY SURPLUS FOOD LISTINGS");
    bool found = false;

    for (const auto& l : m_listings) {
        if (l.getDonorId() == m_currentUser->getId()) {
            found = true;
            std::cout << "----------------------------------------------------------------------------------------\n";
            std::cout << l << "\n";
            if (!l.getAssignedAcceptorName().empty()) {
                std::cout << "      Assigned to: " << l.getAssignedAcceptorName() << "\n";
            }
        }
    }
    if (!found) {
        std::cout << "You have no active or past listings.\n";
    }
    std::cout << "----------------------------------------------------------------------------------------\n";
    UIHelper::pause();
}

void ANNADATUApp::donorReviewRequests() {
    UIHelper::printSectionHeader("REVIEW CONNECTION REQUESTS");
    std::vector<DonationListing*> pendingListings;

    for (auto& l : m_listings) {
        if (l.getDonorId() == m_currentUser->getId() && l.getStatus() == ListingStatus::REQUESTED) {
            pendingListings.push_back(&l);
        }
    }

    if (pendingListings.empty()) {
        std::cout << "No pending pickup requests waiting for your approval.\n";
        UIHelper::pause();
        return;
    }

    std::cout << "The following Acceptors have requested your food donation:\n\n";
    for (size_t i = 0; i < pendingListings.size(); ++i) {
        std::cout << "[" << (i + 1) << "] Listing: " << pendingListings[i]->getId()
                  << " - " << pendingListings[i]->getFoodItem().getTitle() << "\n"
                  << "    Requested by: " << pendingListings[i]->getAssignedAcceptorName() << "\n\n";
    }

    std::cout << "Select a request to approve (or 0 to cancel): ";
    int sel = UIHelper::getMenuChoice(0, static_cast<int>(pendingListings.size()));
    if (sel > 0) {
        pendingListings[sel - 1]->confirmAcceptance();
        syncStorage();
        UIHelper::printSuccess("Request approved! Status is now MATCHED (Ready for pickup).");
    }
    UIHelper::pause();
}

// -------------------------------------------------------------
// ACCEPTOR PORTAL
// -------------------------------------------------------------
void ANNADATUApp::showAcceptorPortal() {
    auto acceptor = std::dynamic_pointer_cast<Acceptor>(m_currentUser);
    if (!acceptor) return;

    acceptor->showDashboard();

    std::cout << "\nACCEPTOR / NGO PORTAL ACTIONS:\n";
    std::cout << " 1. Browse Available Surplus Food Nearby\n";
    std::cout << " 2. Claim / Request Food Donation\n";
    std::cout << " 3. Confirm Food Collection / Delivery Received\n";
    std::cout << " 4. Post Food Requirement for Beneficiaries\n";
    std::cout << " 5. View My Food Requests\n";
    std::cout << " 6. View My Acceptor Profile\n";
    std::cout << " 7. Logout to Main Menu\n";

    int choice = UIHelper::getMenuChoice(1, 7);
    switch (choice) {
        case 1: acceptorBrowseSurplus(); break;
        case 2: acceptorClaimDonation(); break;
        case 3: acceptorConfirmPickup(); break;
        case 4: acceptorRaiseFoodNeed(); break;
        case 5: acceptorViewMyNeeds(); break;
        case 6: acceptor->displayProfile(); UIHelper::pause(); break;
        case 7: {
            m_currentUser = nullptr;
            UIHelper::printInfo("Logged out successfully.");
            UIHelper::pause();
            break;
        }
    }
}

void ANNADATUApp::acceptorBrowseSurplus() {
    UIHelper::printSectionHeader("SURPLUS FOOD LISTINGS NEARBY");

    std::cout << "Diet Preference:\n 1. Any Food\n 2. Pure Veg Only\n";
    int dChoice = UIHelper::getMenuChoice(1, 2);
    DietType dt = (dChoice == 1) ? DietType::ANY : DietType::VEG;

    double maxRadius = UIHelper::readDouble("Search Radius (km, e.g. 20): ", 1.0);

    auto matches = m_matchingEngine.findNearbySurplus(
        m_currentUser->getLocation(), dt, 1, m_listings, maxRadius
    );

    std::cout << "\nFound " << matches.size() << " available surplus donations within " << maxRadius << " km:\n";
    std::cout << "========================================================================================\n";

    if (matches.empty()) {
        std::cout << "No available food listings found in this radius.\n";
    } else {
        int idx = 1;
        for (const auto& m : matches) {
            std::cout << "[" << idx++ << "] Listing ID: " << m.listing.getId() << "\n"
                      << "    Food: " << m.listing.getFoodItem().getTitle() << "\n"
                      << "    Donor: " << m.listing.getDonorName() << "\n"
                      << "    Pickup Location: " << m.listing.getPickupLocation().getFullString() << "\n"
                      << "    Quantity: " << m.listing.getFoodItem().getQuantityKg() << " kg (~" 
                      << m.listing.getFoodItem().getServingsCount() << " meals)\n"
                      << "    Distance: " << m.highlightNote << "\n"
                      << "    Freshness: " << m.listing.getFreshnessBadge() << "\n";
            std::cout << "----------------------------------------------------------------------------------------\n";
        }
    }
    UIHelper::pause();
}

void ANNADATUApp::acceptorClaimDonation() {
    UIHelper::printSectionHeader("CLAIM / REQUEST FOOD DONATION");

    std::string listingId = UIHelper::readLine("Enter Listing ID to request (e.g. LST_001): ");
    auto listing = findListingById(listingId);

    if (!listing) {
        UIHelper::printError("Listing with ID '" + listingId + "' not found.");
        UIHelper::pause();
        return;
    }

    if (listing->getStatus() != ListingStatus::AVAILABLE) {
        UIHelper::printWarning("Listing is already " + listingStatusToString(listing->getStatus()));
        UIHelper::pause();
        return;
    }

    if (listing->isExpired()) {
        UIHelper::printError("This food item has expired and cannot be safely consumed.");
        UIHelper::pause();
        return;
    }

    listing->markRequested(m_currentUser->getId(), m_currentUser->getName());
    syncStorage();

    UIHelper::printSuccess("Food request submitted!");
    std::cout << "Donor: " << listing->getDonorName() << "\n";
    std::cout << "Pickup Address: " << listing->getPickupLocation().getFullString() << "\n";
    std::cout << "Status updated to: REQUEST PENDING (Awaiting Donor approval)\n";
    UIHelper::pause();
}

void ANNADATUApp::acceptorConfirmPickup() {
    UIHelper::printSectionHeader("CONFIRM FOOD COLLECTION / DELIVERY");

    std::vector<DonationListing*> myClaimedListings;
    for (auto& l : m_listings) {
        if (l.getAssignedAcceptorId() == m_currentUser->getId() &&
            (l.getStatus() == ListingStatus::MATCHED || l.getStatus() == ListingStatus::REQUESTED)) {
            myClaimedListings.push_back(&l);
        }
    }

    if (myClaimedListings.empty()) {
        std::cout << "No active food collections waiting for handover confirmation.\n";
        UIHelper::pause();
        return;
    }

    std::cout << "Your active claimed donations:\n\n";
    for (size_t i = 0; i < myClaimedListings.size(); ++i) {
        std::cout << " [" << (i + 1) << "] " << myClaimedListings[i]->getId() << " - "
                  << myClaimedListings[i]->getFoodItem().getTitle()
                  << " (" << myClaimedListings[i]->getFoodItem().getQuantityKg() << " kg, ~"
                  << myClaimedListings[i]->getFoodItem().getServingsCount() << " meals)\n"
                  << "     Donor: " << myClaimedListings[i]->getDonorName() << "\n"
                  << "     Pickup Point: " << myClaimedListings[i]->getPickupLocation().getFullString() << "\n"
                  << "     Current Status: " << listingStatusToString(myClaimedListings[i]->getStatus()) << "\n\n";
    }

    std::cout << "Select donation number to confirm collection (or 0 to cancel): ";
    int choice = UIHelper::getMenuChoice(0, static_cast<int>(myClaimedListings.size()));
    if (choice == 0) return;

    auto listing = myClaimedListings[choice - 1];

    listing->markCompleted();

    int mealsRescued = listing->getFoodItem().getServingsCount();
    double kgRescued = listing->getFoodItem().getQuantityKg();

    // 1. Update Acceptor stats
    auto acceptor = std::dynamic_pointer_cast<Acceptor>(m_currentUser);
    if (acceptor) {
        acceptor->recordMealsReceived(mealsRescued);
    }

    // 2. Award Points and Record Impact for Donor
    auto donorUser = findUserById(listing->getDonorId());
    auto donor = std::dynamic_pointer_cast<Donor>(donorUser);
    int pointsEarned = GamificationEngine::calculatePoints(
        mealsRescued, kgRescued, listing->getFoodItem().getFoodType(), true
    );

    if (donor) {
        donor->recordSuccessfulDonation(mealsRescued, kgRescued, pointsEarned);
    }

    syncStorage();

    UIHelper::clearScreen();
    std::cout << "========================================================================================\n";
    std::cout << "                       FOOD DELIVERED SUCCESSFULLY - ZERO WASTE!                        \n";
    std::cout << "========================================================================================\n";
    std::cout << " [SUCCESS] Handover confirmed! Food received and recorded.\n";
    std::cout << " IMPACT DELIVERED:\n";
    std::cout << "  - " << mealsRescued << " nutritious meals delivered to people in need\n";
    std::cout << "  - " << kgRescued << " kg edible food diverted from landfills\n";
    std::cout << "  - " << GamificationEngine::calculateCarbonOffsetKg(kgRescued) << " kg CO2e greenhouse emissions prevented\n";
    std::cout << "  - Donor (" << listing->getDonorName() << ") earned +" << pointsEarned << " Karma Points!\n";
    std::cout << "========================================================================================\n";
    UIHelper::pause();
}

void ANNADATUApp::acceptorRaiseFoodNeed() {
    UIHelper::printSectionHeader("RAISE URGENT FOOD NEED");

    int servings = UIHelper::readInt("Number of Meals / Servings needed: ", 1);

    std::cout << "Dietary Requirement:\n 1. Pure Veg Only\n 2. Any Food (Veg / Non-Veg)\n";
    int dtChoice = UIHelper::getMenuChoice(1, 2);
    DietType dt = (dtChoice == 1) ? DietType::VEG : DietType::ANY;

    std::cout << "Urgency Level:\n 1. Low\n 2. Medium\n 3. High\n 4. CRITICAL (Urgent immediate need)\n";
    int uChoice = UIHelper::getMenuChoice(1, 4);
    UrgencyLevel ul = static_cast<UrgencyLevel>(uChoice);

    std::string notes = UIHelper::readLine("Target Beneficiaries description (e.g. 'Night shelter elders'): ");

    std::string reqId = generateRequestId();
    FoodRequest req(reqId, m_currentUser->getId(), m_currentUser->getName(),
                    servings, dt, ul, m_currentUser->getLocation(), notes, false);

    m_requests.push_back(req);
    syncStorage();

    UIHelper::printSuccess("Need request [" + reqId + "] registered!");
    std::cout << "Nearby donors in your locality will be prioritized for this demand.\n";
    UIHelper::pause();
}

void ANNADATUApp::acceptorViewMyNeeds() {
    UIHelper::printSectionHeader("MY FOOD DEMAND REQUESTS");
    bool found = false;

    for (const auto& r : m_requests) {
        if (r.getAcceptorId() == m_currentUser->getId()) {
            found = true;
            std::cout << "----------------------------------------------------------------------------------------\n";
            std::cout << r << "\n";
        }
    }
    if (!found) {
        std::cout << "You have no active food requests.\n";
    }
    std::cout << "----------------------------------------------------------------------------------------\n";
    UIHelper::pause();
}

} // namespace ANNADATU
