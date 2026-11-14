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
            std::cout << " 1. Quick Demo Login (Preloaded verified test accounts)\n";
            std::cout << " 2. Login with Email & Password\n";
            std::cout << " 3. Register as a Donor   (Restaurant / Caterer / Hostel / Home)\n";
            std::cout << " 4. Register as Acceptor (NGO / Shelter / Community Kitchen)\n";
            std::cout << " 5. View Global Mission & Paradox of Plenty Statistics\n";
            std::cout << " 6. View ANNADATU Community Heroes Leaderboard\n";
            std::cout << " 7. Exit Program\n";
            std::cout << "+-------------------------------------------------------------+\n";

            int choice = UIHelper::getMenuChoice(1, 7);
            switch (choice) {
                case 1: handleQuickDemoLogin(); break;
                case 2: handleLogin(); break;
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

void ANNADATUApp::handleQuickDemoLogin() {
    UIHelper::printSectionHeader("QUICK DEMO LOGIN SELECTOR");
    std::cout << "Choose a preconfigured persona to explore the app:\n";
    std::cout << " [1] Donor:    Bikanervala Grand Banquets (Caterer, Karol Bagh)\n";
    std::cout << " [2] Donor:    Haldirams CP Flagship (Restaurant, CP)\n";
    std::cout << " [3] Donor:    IIT Delhi Mess Committee (Hostel Mess, Hauz Khas)\n";
    std::cout << " [4] Acceptor: Robin Hood Army Delhi (NGO, CP)\n";
    std::cout << " [5] Acceptor: Aasra Community Night Shelter (Shelter Home, Old Delhi)\n";
    std::cout << " [6] Acceptor: Nanhe Parinde Children Welfare (Orphanage, Lajpat Nagar)\n";
    std::cout << " [7] Return to Main Menu\n";

    int opt = UIHelper::getMenuChoice(1, 7);
    if (opt == 1) m_currentUser = findUserById("USR_D101");
    else if (opt == 2) m_currentUser = findUserById("USR_D102");
    else if (opt == 3) m_currentUser = findUserById("USR_D103");
    else if (opt == 4) m_currentUser = findUserById("USR_A201");
    else if (opt == 5) m_currentUser = findUserById("USR_A202");
    else if (opt == 6) m_currentUser = findUserById("USR_A203");
    else return;

    if (m_currentUser) {
        UIHelper::printSuccess("Logged in successfully as " + m_currentUser->getName());
        UIHelper::pause();
    }
}

void ANNADATUApp::handleLogin() {
    UIHelper::printSectionHeader("ACCOUNT LOGIN");
    std::string email = UIHelper::readLine("Enter registered email: ");
    std::string password = UIHelper::readLine("Enter password: ");

    auto user = findUserByEmail(email);
    if (user && user->verifyPassword(password)) {
        m_currentUser = user;
        UIHelper::printSuccess("Welcome back, " + user->getName() + "!");
    } else {
        UIHelper::printError("Invalid email or password. Please try again.");
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
    std::cout << " 1. Post a New Surplus Food Listing (Raw/Cooked, Expiry Window, Pickup Point)\n";
    std::cout << " 2. View My Active Food Listings & Freshness Meter\n";
    std::cout << " 3. Review Incoming Acceptor Requests & Confirm Booking\n";
    std::cout << " 4. View Handover OTP (Show to Acceptor at pickup)\n";
    std::cout << " 5. View Full Donor Profile & Badges\n";
    std::cout << " 6. View ANNADATU Community Leaderboard\n";
    std::cout << " 7. Logout to Main Menu\n";

    int choice = UIHelper::getMenuChoice(1, 7);
    switch (choice) {
        case 1: donorPostFood(); break;
        case 2: donorViewMyListings(); break;
        case 3: donorReviewRequests(); break;
        case 4: donorShowHandoverOTP(); break;
        case 5: donor->displayProfile(); UIHelper::pause(); break;
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
    std::string handling = UIHelper::readLine("Storage & Handling Instructions (e.g., 'Insulated hot containers'): ");
    if (handling.empty()) handling = "Hygienic covered packaging";

    std::string photoTag = UIHelper::readLine("Photo Reference / Verification Tag: ");
    if (photoTag.empty()) photoTag = "food_listing_" + std::to_string(m_listings.size() + 1) + ".jpg";

    FoodItem item(title, ft, dt, qtyKg, servings, handling, photoTag);
    std::string listId = generateListingId();
    std::string otp = generateOTP();

    DonationListing listing(listId, m_currentUser->getId(), m_currentUser->getName(),
                            item, m_currentUser->getLocation(), expiryHours,
                            getCurrentTime(), ListingStatus::AVAILABLE, "", "", otp);

    m_listings.push_back(listing);
    syncStorage();

    UIHelper::printSuccess("Food donation listing [" + listId + "] published successfully!");
    std::cout << " [!] Assigned Security OTP for Handover: " << otp << "\n";
    std::cout << "     (Share this code with the verified Acceptor upon physical food pickup)\n";
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
            std::cout << "      Assigned OTP: " << l.getOtp() << "\n";
            if (!l.getAssignedAcceptorName().empty()) {
                std::cout << "      Requested/Matched with: " << l.getAssignedAcceptorName() << "\n";
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

    std::cout << "Select a request to confirm and schedule pickup (or 0 to cancel): ";
    int sel = UIHelper::getMenuChoice(0, static_cast<int>(pendingListings.size()));
    if (sel > 0) {
        pendingListings[sel - 1]->confirmAcceptance();
        syncStorage();
        UIHelper::printSuccess("Pickup request confirmed! Status updated to MATCHED.");
        std::cout << "Please ensure food is prepared. Handover OTP: " << pendingListings[sel - 1]->getOtp() << "\n";
    }
    UIHelper::pause();
}

void ANNADATUApp::donorShowHandoverOTP() {
    UIHelper::printSectionHeader("HANDOVER OTP VERIFICATION CODES");
    std::cout << "Active matched donations awaiting pickup:\n";
    bool anyMatched = false;

    for (const auto& l : m_listings) {
        if (l.getDonorId() == m_currentUser->getId() &&
            (l.getStatus() == ListingStatus::MATCHED || l.getStatus() == ListingStatus::REQUESTED)) {
            anyMatched = true;
            std::cout << " + Listing ID:     " << l.getId() << "\n";
            std::cout << "   Food:           " << l.getFoodItem().getTitle() << "\n";
            std::cout << "   Acceptor:       " << l.getAssignedAcceptorName() << "\n";
            std::cout << "   SECURE OTP:     >>>  " << l.getOtp() << "  <<<\n";
            std::cout << "   (Show this code to the pickup person to verify the handover)\n";
            std::cout << "--------------------------------------------------------\n";
        }
    }

    if (!anyMatched) {
        std::cout << "No donations are currently in MATCHED or REQUESTED status.\n";
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
    std::cout << " 1. Browse Nearby Surplus Food (Urgency & Hyperlocal Distance Matching)\n";
    std::cout << " 2. Connect / Request Surplus Food Donation\n";
    std::cout << " 3. Complete Pickup & Verify Handover using Donor's OTP\n";
    std::cout << " 4. Raise / Post Urgent Food Need for Beneficiaries\n";
    std::cout << " 5. View My Food Requests & Distributed Total\n";
    std::cout << " 6. View My Acceptor Profile\n";
    std::cout << " 7. Logout to Main Menu\n";

    int choice = UIHelper::getMenuChoice(1, 7);
    switch (choice) {
        case 1: acceptorBrowseSurplus(); break;
        case 2: acceptorClaimDonation(); break;
        case 3: acceptorVerifyHandoverWithOTP(); break;
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
    UIHelper::printSectionHeader("HYPERLOCAL SURPLUS FOOD MATCHING");

    std::cout << "Filter Diet Preference:\n 1. Any Diet\n 2. Pure Veg Only\n";
    int dChoice = UIHelper::getMenuChoice(1, 2);
    DietType dt = (dChoice == 1) ? DietType::ANY : DietType::VEG;

    double maxRadius = UIHelper::readDouble("Search Radius (km, e.g. 20): ", 1.0);

    // Run Strategy Pattern matching
    auto matches = m_matchingEngine.findNearbySurplus(
        m_currentUser->getLocation(), dt, 1, m_listings, maxRadius
    );

    std::cout << "\nStrategy: " << m_matchingEngine.getStrategy()->getStrategyName() << "\n";
    std::cout << "Found " << matches.size() << " available surplus donations within " << maxRadius << " km:\n";
    std::cout << "========================================================================================\n";

    if (matches.empty()) {
        std::cout << "No available listings matched your criteria in this radius.\n";
    } else {
        int idx = 1;
        for (const auto& m : matches) {
            std::cout << "[" << idx++ << "] Listing ID: " << m.listing.getId()
                      << " | Score: " << std::fixed << std::setprecision(1) << m.matchScore << " pts\n"
                      << "    Food: " << m.listing.getFoodItem().getTitle() << "\n"
                      << "    Donor: " << m.listing.getDonorName() << "\n"
                      << "    Location: " << m.listing.getPickupLocation().getFullString() << "\n"
                      << "    Quantity: " << m.listing.getFoodItem().getQuantityKg() << " kg (~" 
                      << m.listing.getFoodItem().getServingsCount() << " servings)\n"
                      << "    Proximity & Urgency: " << m.highlightNote << "\n"
                      << "    Freshness Meter: " << m.listing.getFreshnessBadge() << "\n";
            std::cout << "----------------------------------------------------------------------------------------\n";
        }
    }
    UIHelper::pause();
}

void ANNADATUApp::acceptorClaimDonation() {
    UIHelper::printSectionHeader("REQUEST / CLAIM FOOD DONATION");

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
        UIHelper::printError("This food item has expired and cannot be safely distributed.");
        UIHelper::pause();
        return;
    }

    listing->markRequested(m_currentUser->getId(), m_currentUser->getName());
    syncStorage();

    UIHelper::printSuccess("Request submitted successfully!");
    std::cout << "The Donor (" << listing->getDonorName() << ") has been notified of your request.\n";
    std::cout << "Pickup Point: " << listing->getPickupLocation().getFullString() << "\n";
    std::cout << "Once at pickup, ask the donor for their 6-digit OTP to complete verification!\n";
    UIHelper::pause();
}

void ANNADATUApp::acceptorVerifyHandoverWithOTP() {
    UIHelper::printSectionHeader("OTP-VERIFIED HANDOVER (TRUST BY DESIGN)");

    std::string listingId = UIHelper::readLine("Enter Listing ID you are picking up (e.g. LST_001): ");
    auto listing = findListingById(listingId);

    if (!listing) {
        UIHelper::printError("Listing not found.");
        UIHelper::pause();
        return;
    }

    if (listing->getAssignedAcceptorId() != m_currentUser->getId()) {
        UIHelper::printError("You are not the designated recipient for this listing.");
        UIHelper::pause();
        return;
    }

    if (listing->getStatus() == ListingStatus::COMPLETED) {
        UIHelper::printInfo("This donation has already been verified and closed.");
        UIHelper::pause();
        return;
    }

    std::cout << "\nVerifying Handover for:\n";
    std::cout << " * Listing: " << listing->getId() << " - " << listing->getFoodItem().getTitle() << "\n";
    std::cout << " * Quantity: " << listing->getFoodItem().getQuantityKg() << " kg (~"
              << listing->getFoodItem().getServingsCount() << " meals)\n";
    std::cout << " * Donor: " << listing->getDonorName() << "\n\n";

    std::string enteredOtp = UIHelper::readLine("Enter the 6-digit OTP provided by the Donor: ");

    if (listing->verifyOtpAndComplete(enteredOtp)) {
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

        // Thank You & Impact Screen (Slide 4, 5, 6, 7)
        UIHelper::clearScreen();
        std::cout << "========================================================================================\n";
        std::cout << "                  HANDOVER VERIFIED! - EVERY MEAL SAVED IS A MEAL SERVED!               \n";
        std::cout << "========================================================================================\n";
        std::cout << " [SUCCESS] OTP Matched! Transaction securely closed in ANNADATU ledger.\n";
        std::cout << " IMPACT DELIVERED:\n";
        std::cout << "  - " << mealsRescued << " nutritious meals delivered to people in need\n";
        std::cout << "  - " << kgRescued << " kg edible surplus diverted from landfills\n";
        std::cout << "  - " << GamificationEngine::calculateCarbonOffsetKg(kgRescued) << " kg CO2e greenhouse emissions prevented\n";
        std::cout << "  - Donor (" << listing->getDonorName() << ") earned +" << pointsEarned << " Karma Points on Leaderboard!\n";
        std::cout << "========================================================================================\n";
    } else {
        UIHelper::printError("INCORRECT OTP! Handover verification failed. Please check with donor.");
    }
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
