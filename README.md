# 🍲 ANNADATU: Surplus Food Redistribution System
> **"Ann + Setu – The Bridge Against Hunger."**  
> *"Every meal saved is a meal served!"*

---

## 👥 Authors
- **Aman Pokharia**
- **Roshan Choudhary**
- **Khushi Gupta**

---

## 📖 Project Overview
In India, millions of people face hunger daily while tonnes of edible, wholesome food is thrown away from restaurants, banquets, university messes, and homes. 

**ANNADATU** is a terminal-based C++ application designed to connect food **Donors** (restaurants, hostels, caterers) with **Acceptors** (NGOs, shelters, community kitchens) so that edible surplus food can be collected and served before it spoils.

---

## 🎓 Learning Outcome
> **"Learned to apply core Object-Oriented Programming (OOP) concepts in C++—including Classes, Inheritance, Polymorphism, Encapsulation, and File Handling—to build a real-world surplus food management system."**

---

## 🏛️ OOPS Concepts Implemented

This project demonstrates the core syllabus concepts of Object-Oriented Programming:

### 1. Classes and Objects
- **`User`**: Base class representing common user profile information.
- **`Donor`**: Represents entities providing surplus food (restaurants, mess halls, homes).
- **`Acceptor`**: Represents entities receiving food (NGOs, orphanages, shelters).
- **`FoodItem`**: Stores food title, type (cooked/raw), diet (veg/non-veg), and quantity.
- **`DonationListing`**: Represents an active food donation with pickup location and expiry window.
- **`Location`**: Stores address and geographical coordinates.

### 2. Inheritance
A clean inheritance hierarchy models users in the system:
```text
                  +-------------------+
                  |    User (Base)    |
                  +-------------------+
                            ^
                            |
             +--------------+--------------+
             |                             |
    +-----------------+           +-------------------+
    |  Donor (Child)  |           |  Acceptor (Child) |
    +-----------------+           +-------------------+
```

### 3. Polymorphism & Virtual Functions
- The base class `User` defines virtual functions:
  ```cpp
  virtual void displayProfile() const = 0;
  virtual void showDashboard() const = 0;
  virtual std::string getRoleSpecificType() const = 0;
  ```
- Both `Donor` and `Acceptor` override these functions to provide role-specific screens.

### 4. Encapsulation & Data Hiding
- All class member variables are kept `private` or `protected`.
- Access and modification are handled through getter and setter methods with input validation.

### 5. File Handling & Data Persistence
- Data is stored in text files inside the `data/` folder:
  - `data/users.txt`: Stores registered users.
  - `data/donations.txt`: Stores active and completed food listings.
  - `data/requests.txt`: Stores NGO demand requests.
- When the application starts, it automatically loads saved data from disk.

---

## 🔄 How the System Works

### 1. Easy One-Click Login
- No complicated passwords or tokens needed for testing.
- The main menu allows you to choose an existing **Donor** (e.g. *Haldirams*, *Bikanervala*, *IIT Mess*) or **Acceptor** (e.g. *Robin Hood Army*, *Aasra Shelter*) with just one number.

### 2. Donor Actions:
- **Post Food**: Add excess food details (name, cooked/raw, veg/non-veg, weight in kg, servings, safe consumption hours).
- **View My Listings**: Check current status of published food.
- **Approve Requests**: View and accept pickup requests from nearby NGOs.
- **Profile & Badges**: View total meals saved and earned karma badges.

### 3. Acceptor (NGO) Actions:
- **Browse Nearby Surplus Food**: See available donations ranked by distance and freshness.
- **Claim Food Donation**: Send a request to collect a donation listing.
- **Confirm Collection**: Mark food as received with 1 step once collected.
- **Post Food Need**: Register a food demand for shelter beneficiaries.

### 4. Leaderboard & Impact:
- Calculates **meals served**, **kg of food diverted from landfills**, and **CO₂ emissions saved**.
- Ranks donors on a community leaderboard based on meals donated.

---

## 📁 Project Structure

```text
oops project/
│
├── build.bat               # Simple 1-click compile script
├── run.bat                 # 1-click launch script
├── README.md               # Project documentation
├── annadatu.exe            # Executable file
│
├── data/                   # Saved data text files
│   ├── users.txt
│   ├── donations.txt
│   └── requests.txt
│
├── include/                # Header files (.hpp) - Declarations
│   ├── User.hpp
│   ├── Donor.hpp
│   ├── Acceptor.hpp
│   ├── FoodItem.hpp
│   ├── DonationListing.hpp
│   ├── FoodRequest.hpp
│   ├── Location.hpp
│   ├── MatchingEngine.hpp
│   ├── GamificationEngine.hpp
│   ├── StorageManager.hpp
│   ├── UIHelper.hpp
│   └── AnnadatuApp.hpp
│
└── src/                    # Source files (.cpp) - Implementations
    ├── User.cpp
    ├── Donor.cpp
    ├── Acceptor.cpp
    ├── FoodItem.cpp
    ├── DonationListing.cpp
    ├── FoodRequest.cpp
    ├── Location.cpp
    ├── MatchingEngine.cpp
    ├── GamificationEngine.cpp
    ├── StorageManager.cpp
    ├── UIHelper.cpp
    ├── AnnadatuApp.cpp
    └── main.cpp
```

---

## 🚀 How to Run

### In Windows PowerShell:
```powershell
.\run.bat
```
*(or run the binary directly: `.\annadatu.exe`)*

### In Windows Command Prompt (CMD):
```cmd
run.bat
```

### To Recompile (if you make any changes):
```powershell
.\build.bat
```

---

## 🧪 Quick Test (Try it in 1 minute!)
1. Run `.\run.bat`.
2. Select **`2`** (Login as Acceptor) $\to$ Select **`1`** (*Robin Hood Army Delhi*).
3. Select **`2`** (Claim / Request Food Donation) $\to$ Enter `LST_001`.
4. Select **`3`** (Confirm Food Collection) $\to$ Select `1`.
5. 🎉 Watch the food get successfully delivered, diverted from landfills, and added to the donor's leaderboard score!
