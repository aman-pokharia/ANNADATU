# 🍲 ANNADATU: Connecting Surplus Food to Hungry Hearts Worldwide
> **"Ann + Setu – The Bridge Against Hunger."**  
> *"Every meal saved is a meal served!"*

---

## 👥 Authors & Contributors
- **Aman Pokharia**
- **Roshan Choudhary**
- **Khushi Gupta**

---

## 📋 Table of Contents
1. [The Problem: The Paradox of Plenty](#-the-problem-the-paradox-of-plenty)
2. [Our Solution: Annadatu](#-our-solution-annadatu)
3. [Key Highlights & Innovation](#-key-highlights--innovation)
4. [Object-Oriented Programming (OOPS) Architecture](#-object-oriented-programming-oops-architecture)
   - [Encapsulation & Data Invariants](#1-encapsulation--data-invariants)
   - [Inheritance Hierarchy](#2-inheritance-hierarchy)
   - [Polymorphism & Abstract Interfaces](#3-polymorphism--abstract-interfaces)
   - [Operator Overloading](#4-operator-overloading)
   - [Design Patterns](#5-design-patterns)
5. [End-to-End System Workflow](#-end-to-end-system-workflow)
   - [The Donor Journey](#the-donor-journey)
   - [The Acceptor (NGO/Shelter) Journey](#the-acceptor-ngoshelter-journey)
   - [Trust by Design: OTP Handover](#trust-by-design-otp-handover)
   - [Hyperlocal & Urgency-Aware Matching](#hyperlocal--urgency-aware-matching)
   - [Gamification & Environmental Impact Math](#gamification--environmental-impact-math)
6. [Project Directory Structure](#-project-directory-structure)
7. [How to Run the Application](#-how-to-run-the-application)
   - [Running in Windows PowerShell](#option-a-windows-powershell-recommended)
   - [Running in Windows Command Prompt (CMD)](#option-b-windows-command-prompt-cmd)
   - [Recompiling the Code](#recompiling-the-code)
8. [Pre-Seeded Demo Personas (Instant Testing)](#-pre-seeded-demo-personas-instant-testing)
9. [Future Roadmap](#-future-roadmap)

---

## 🌍 The Problem: The Paradox of Plenty

India faces a staggering paradox:
- **78 – 80 Million tonnes** of edible food is wasted every year in India (*UNEP Food Waste Index*).
- **₹1.55 Lakh Crore** estimated economic value lost annually (~1% of National GDP).
- **~19 Crore people** remain undernourished daily (*Global Hunger Index*).
- Food decomposing in landfills drives **8 – 10% of global greenhouse emissions**.

> **"The surplus exists, but the hunger persists. What is missing is the bridge."**  
> **Our motive is to build this bridge — ANNADATU (Ann + Setu).**

---

## 💡 Our Solution: Annadatu

**Annadatu** is a high-performance **two-sided P2P marketplace for surplus food**:
- **Donors** (Restaurants, banquet caterers, university hostel messes, supermarkets, and homes) list surplus food before it spoils.
- **Acceptors** (Registered NGOs, night shelters, orphanages, and community kitchens) discover nearby food in real-time.
- **The Platform Engine** matches them by **distance**, **food perishability**, and **dietary preference**, ensuring safe, verified handover with **zero food waste**.

```text
+-----------------------+           +-----------------------+           +-----------------------+
|        DONOR          |  Post     |       ANNADATU        |  Matches  |       ACCEPTOR        |
| Restaurants, Caterers,| --------> | Hyperlocal Engine     | --------> | NGOs, Shelters,       |
| Hostels, Homes        |           | Verifies & Notifies   |           | Community Kitchens    |
+-----------------------+           +-----------------------+           +-----------------------+
            ^                                   |                                   |
            |                         Secure 6-Digit OTP                            |
            +========================= Handover Verification =======================+
```

---

## ⚡ Key Highlights & Innovation

1. **Hyperlocal Proximity Matching**: Utilizes the mathematical **Haversine Formula** to compute great-circle physical distance in kilometers between donor pickup points and recipient shelters.
2. **Urgency-Aware Matching Engine**: Rather than simple FIFO (First In, First Out), our engine dynamically scores listings by **time remaining until expiry** and preparation type (cooked hot meals rank higher than dry raw grains) so perishable food is rescued first.
3. **Trust by Design (OTP Verification)**: Every donation generates a cryptographically random **6-digit One-Time Password (OTP)**. The transaction only closes and points are awarded once the recipient enters this OTP upon physical pickup. No fake claims, 100% accountability.
4. **Gamified Giving**: Donors earn **Annadatu Karma Points**, unlock milestone badges, and climb the **Community Leaderboard**, transforming one-time donors into habitual lifesavers.
5. **Freshness Transparency**: Clear status meters (`[FRESH & SAFE]`, `[SERVE SOON]`, `[CRITICAL FRESHNESS]`, `[EXPIRED]`) ensure total food safety.
6. **Data Persistence**: Uses structured file serialization to disk (`data/`) preserving users, donations, requests, and metrics across restarts.

---

## 🏛️ Object-Oriented Programming (OOPS) Architecture

The application is built in **Modern C++ (C++17)** adhering strictly to production OOP design principles.

### 1. Encapsulation & Data Invariants
- All class fields (`m_quantityKg`, `m_servingsCount`, `m_latitude`, `m_otp`, `m_rewardPoints`, etc.) are declared `private` or `protected`.
- Mutators strictly validate domain boundaries. For example, setting negative food quantity or negative serving counts throws custom exceptions:
  - `ANNADATUException` (Base exception)
  - `ValidationException`
  - `AuthException`
  - `ListingException`

### 2. Inheritance Hierarchy
A clean class hierarchy models participants in the two-sided marketplace:

```text
                             +------------------------+
                             |       User (Base)      |
                             +------------------------+
                             | - m_userId: string     |
                             | - m_name: string       |
                             | - m_email: string      |
                             | - m_phone: string      |
                             | - m_location: Location |
                             | - m_role: Role         |
                             +------------------------+
                                         ^
                                         |
                     +-------------------+-------------------+
                     |                                       |
          +----------------------+               +------------------------+
          |     Donor (Child)    |               |    Acceptor (Child)    |
          +----------------------+               +------------------------+
          | - m_donorType        |               | - m_acceptorType       |
          | - m_rewardPoints     |               | - m_beneficiaryCapacity|
          | - m_totalMealsDonated|               | - m_totalMealsReceived |
          | - m_totalKgSaved     |               | - m_requestIds         |
          | - m_badges           |               +------------------------+
          +----------------------+
```

### 3. Polymorphism & Abstract Interfaces
- Base class `User` is an **Abstract Class** with pure virtual functions:
  ```cpp
  virtual void displayProfile() const = 0;
  virtual void showDashboard() const = 0;
  virtual std::string getRoleSpecificType() const = 0;
  virtual std::string serialize() const = 0;
  ```
- Runtime polymorphic dispatch allows `ANNADATUApp` and `StorageManager` to handle mixed collections of users seamlessly via `std::shared_ptr<User>`.

### 4. Operator Overloading
The project showcases rich operator overloading:
- **Stream Insertion (`operator<<`)**:
  - `operator<<(std::ostream&, const Location&)`: Formats coordinates and address strings.
  - `operator<<(std::ostream&, const FoodItem&)`: Pretty-prints diet badges, weight, and meal portions.
  - `operator<<(std::ostream&, const DonationListing&)`: Prints formatted food cards.
  - `operator<<(std::ostream&, const FoodRequest&)`: Formats acceptor demand listings.
- **Urgency Comparison (`operator<`)**:
  - `DonationListing::operator<`: Ranks listings by urgency score so the highest urgency always appears first.
  - `MatchCandidate::operator<`: Sorts matching candidates by composite match score.
  - `LeaderboardEntry::operator<`: Sorts donors descending by Karma Points and meals donated.
- **Accumulation (`operator+=`)**:
  - `FoodItem::operator+=`: Aggregates food quantities and servings safely.

### 5. Design Patterns
- **Strategy Pattern (`IMatchingStrategy`)**:
  - Interface declaring `virtual std::vector<MatchCandidate> match(...) = 0;`.
  - Concrete strategies:
    - `UrgencyAwareMatchingStrategy`: Combines distance penalty with expiry countdown.
    - `ProximityFirstStrategy`: Pure distance-based ranking.
  - The strategy can be swapped at runtime inside `MatchingEngine`.
- **Repository / Storage Pattern (`StorageManager`)**:
  - Decouples persistence from domain entities. Serializes and deserializes records to `data/users.txt`, `data/donations.txt`, and `data/requests.txt`.
- **Controller Pattern (`ANNADATUApp`)**:
  - Central engine coordinating authentication, donor portal, acceptor portal, and statistics.

---

## 🔄 End-to-End System Workflow

### The Donor Journey
1. **Sign-In / Register**: Select Donor category (`Restaurant`, `Event Caterer`, `Hostel Mess`, `Home`, `Bakery`).
2. **List Surplus**: Enter food item name, preparation type (`Cooked` or `Raw`), dietary classification (`Pure Veg`, `Non-Veg`, `Eggetarian`), weight in kg, estimated servings, safe consumption window (hours), and handling instructions.
3. **Receive Requests**: Review incoming connection requests from local NGOs.
4. **Confirm & Handover**: Confirm the booking. Upon physical arrival of the pickup volunteer, show the **6-Digit Handover OTP**.
5. **Reward & Repeat**: Once verified, view the thank-you screen, earn Karma Points, unlock badges, and check your rank on the Leaderboard.

### The Acceptor (NGO/Shelter) Journey
1. **Sign-In / Register**: Choose category (`Registered NGO`, `Shelter Home`, `Community Kitchen`, `Orphanage`, `Individual in Need`).
2. **Browse Surplus**: Explore nearby donations ranked by distance and expiry urgency. Filter by dietary requirement.
3. **Connect in One-Tap**: Send a connect request for a suitable food listing.
4. **Pickup & Close Handover**: Arrive at pickup, inspect food freshness, obtain the 6-digit OTP from the Donor, and enter it into the app.
5. **Raise Demand**: Raise an urgent food need specifying required servings and urgency level.

### Trust by Design: OTP Handover
To prevent fake claims and verify real distribution, Annadatu uses an OTP verification handshake:
```text
Donor publishes food ------------> System generates secure 6-Digit OTP (e.g., 631902)
                                                 |
Acceptor claims food  ------------> Donor confirms pickup request
                                                 |
Physical Handover     ------------> Donor shares OTP with Acceptor
                                                 |
Transaction Closed    ------------> Acceptor enters OTP -> System validates -> Impact Recorded!
```

### Hyperlocal & Urgency-Aware Matching

#### 1. Haversine Distance Formula:
$$\Delta\sigma = 2 \arcsin \sqrt{\sin^2\left(\frac{\Delta\phi}{2}\right) + \cos(\phi_1)\cos(\phi_2)\sin^2\left(\frac{\Delta\lambda}{2}\right)}$$
$$d = R \cdot \Delta\sigma \quad (\text{where } R = 6371 \text{ km})$$

#### 2. Composite Match Score Formula:
$$\text{Score} = \text{Urgency Factor (up to 60 pts)} + \text{Proximity Factor (up to 40 pts)} + \text{Portion Fit (10 pts)}$$
- If hours left $\le 2.0\text{h} \implies 60$ pts *(Perishable Priority)*
- Closer distance yields higher proximity score: $\max\left(0, \frac{R_{\text{max}} - d}{R_{\text{max}}} \times 40\right)$

### Gamification & Environmental Impact Math
- **Karma Points**:
  - $1 \text{ meal rescued} = 10 \text{ points}$
  - Cooked hot food bonus $= +20 \text{ points}$
  - Fast turnaround bonus $= +30 \text{ points}$
  - Bulk rescue ($>20 \text{ kg}$) bonus $= +50 \text{ points}$
- **Milestone Badges**:
  - 🌟 **First Spark**: 1+ meals saved
  - 🥉 **Bronze Caregiver**: 50+ meals saved
  - 🥈 **Silver Guardian**: 200+ meals saved
  - 🥇 **Gold Champion**: 500+ meals saved
  - 🛡️ **Zero-Waste Titan**: 1,000+ meals saved
- **Environmental Metrics**:
  - **CO₂e Emissions Averted**: $\text{Food Weight (kg)} \times 2.5 \text{ kg CO}_2\text{e}$
  - **Virtual Water Saved**: $\text{Food Weight (kg)} \times 800 \text{ Liters}$

---

## 📁 Project Directory Structure

```text
oops project/
│
├── build.bat                   # 1-click Windows compilation script
├── run.bat                     # Application launcher
├── README.md                   # Complete documentation
├── annadatu.exe                # Compiled 64-bit standalone executable
│
├── data/                       # Persistent file storage
│   ├── users.txt               # User accounts (Donors & Acceptors)
│   ├── donations.txt           # Food donation listings & OTPs
│   └── requests.txt            # Acceptor demand requests
│
├── include/                    # Header files (.hpp) - Declarations
│   ├── Common.hpp              # Enums, custom exceptions & utility functions
│   ├── Location.hpp            # Location class header (Haversine logic)
│   ├── FoodItem.hpp            # FoodItem class header
│   ├── User.hpp                # Abstract Base User class
│   ├── Donor.hpp               # Derived Donor class (gamification & badges)
│   ├── Acceptor.hpp            # Derived Acceptor class (beneficiaries)
│   ├── DonationListing.hpp     # Food donation listing & OTP lifecycle
│   ├── FoodRequest.hpp         # Acceptor need / demand request
│   ├── MatchingEngine.hpp      # Strategy pattern interface & classes
│   ├── GamificationEngine.hpp  # Points, badges & carbon calculator
│   ├── StorageManager.hpp      # File I/O persistence header
│   ├── UIHelper.hpp            # Terminal styling, banners & input validation
│   └── AnnadatuApp.hpp         # Central controller header
│
└── src/                        # Source files (.cpp) - Implementations
    ├── Location.cpp            # Distance calculation & coordinate parsing
    ├── FoodItem.cpp            # Food packaging & quantity logic
    ├── User.cpp                # Base user implementation
    ├── Donor.cpp               # Donor profile, badges & karma points
    ├── Acceptor.cpp            # Acceptor profile & receipt tracking
    ├── DonationListing.cpp     # Freshness meter & OTP verification
    ├── FoodRequest.cpp         # Demand listing implementation
    ├── MatchingEngine.cpp      # Urgency & proximity algorithms
    ├── GamificationEngine.cpp  # Community leaderboard generator
    ├── StorageManager.cpp      # File serialization & demo data seeder
    ├── UIHelper.cpp            # Robust UI & input handling
    ├── AnnadatuApp.cpp         # Main application workflows
    └── main.cpp                # Program entry point & exception boundary
```

---

## 🚀 How to Run the Application

The executable `annadatu.exe` is already compiled and ready to run.

### Option A: Windows PowerShell (Recommended)
Open PowerShell in the project directory and execute:
```powershell
.\run.bat
```
*(or run the binary directly: `.\annadatu.exe`)*

### Option B: Windows Command Prompt (CMD)
Open Command Prompt in the project directory and execute:
```cmd
run.bat
```
*(or `annadatu.exe`)*

### Recompiling the Code
If you modify any source files in `src/` or headers in `include/`, recompile easily:
- In PowerShell:
  ```powershell
  .\build.bat
  ```
- In CMD:
  ```cmd
  build.bat
  ```
*(The build script automatically detects Visual Studio MSVC 2022 C++17 compiler and builds `annadatu.exe` with optimizations).*

---

## 🧪 Pre-Seeded Demo Personas (Instant Testing)

To test the application immediately without manually typing new records, select **Option 1 ("Quick Demo Login")** from the main menu:

| Role | Entity Name | Type | Location | Active Listings / Role |
| :--- | :--- | :--- | :--- | :--- |
| **Donor** | **Haldirams CP Flagship** | Restaurant | Connaught Place, New Delhi | Snack Combo (Idli, Dhokla, Sandwiches) |
| **Donor** | **Bikanervala Grand Banquets** | Event Caterer | Karol Bagh, New Delhi | Shahi Paneer, Jeera Pulao & Roti |
| **Donor** | **IIT Delhi Mess Committee** | Hostel Mess | Hauz Khas, New Delhi | Fresh Rajma Chawal & Salad |
| **Donor** | **Sharma Family Residence** | Home | Rohini, New Delhi | Pantry surplus flour & lentils |
| **Acceptor**| **Robin Hood Army Delhi** | Registered NGO | Connaught Place, New Delhi | Evening food drive for workers |
| **Acceptor**| **Aasra Community Shelter** | Shelter Home | Old Delhi Railway Station | Shelter for 70 homeless elders |
| **Acceptor**| **Nanhe Parinde Welfare** | Orphanage | Lajpat Nagar, New Delhi | Care home for 45 children |

### Quick 2-Minute Test Walkthrough:
1. Run `.\run.bat`.
2. Select **`1`** (Quick Demo Login) $\to$ Choose **`4`** (Robin Hood Army Delhi - Acceptor).
3. Choose **`1`** (Browse Nearby Surplus Food) $\to$ Observe how **Haldirams CP Flagship** is ranked #1 due to proximity (0.7 km) and urgency (only 2 hours left).
4. Choose **`2`** (Connect / Request Surplus Food) $\to$ Enter `LST_003`.
5. Choose **`7`** (Logout) $\to$ Choose **`1`** $\to$ Choose **`2`** (Haldirams CP - Donor).
6. Choose **`3`** (Review Connection Requests) $\to$ Confirm the request from Robin Hood Army.
7. Choose **`4`** (View Handover OTP) $\to$ Note the OTP: `631902`.
8. Choose **`7`** (Logout) $\to$ Choose **`1`** $\to$ Choose **`4`** (Robin Hood Army).
9. Choose **`3`** (Complete Pickup & Verify Handover using Donor's OTP) $\to$ Enter `LST_003` and OTP `631902`.
10. 🎉 **Impact Screen**: Watch the transaction verify, diverting 15 kg from landfills, feeding 35 people, preventing 37.5 kg CO₂e, and awarding +400 Karma Points to the Donor!

---

## 🔮 Future Roadmap

- **Phase 1 (Post-MVP)**: Onboard 5–10 local NGOs; pilot in one urban zone.
- **Phase 2 (3–6 Months)**: Volunteer delivery network integration; food-safety checklists with digital temperature logs.
- **Phase 3 (6–12 Months)**: AI demand prediction forecasting where surplus will appear based on banquet and catering schedules.
- **Phase 4 (Year 2)**: Corporate Social Responsibility (CSR) impact reports, FSSAI compliance verification, and nationwide multi-city scale.

---
> *"Less waste. More meals. Greater impact."*  
> **ANNADATU — Technology connecting surplus with needs.**
