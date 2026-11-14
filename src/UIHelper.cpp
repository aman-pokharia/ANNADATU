#include "UIHelper.hpp"

namespace ANNADATU {

void UIHelper::clearScreen() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::system("clear");
#endif
}

void UIHelper::pause() {
    std::cout << "\nPress Enter to continue...";
    std::string dummy;
    if (!std::getline(std::cin, dummy)) {
        if (std::cin.eof()) {
            std::cin.clear();
        }
    }
}

void UIHelper::printMainBanner() {
    std::cout << "========================================================================================\n";
    std::cout << "                                  \" A N N A D A T U \"                                  \n";
    std::cout << "                         Ann + Setu - The Bridge Against Hunger                         \n";
    std::cout << "             \"Connecting Surplus Food to Hungry Hearts Worldwide - Zero Waste\"          \n";
    std::cout << "========================================================================================\n";
}

void UIHelper::printTeamCredits() {
    std::cout << "----------------------------------------------------------------------------------------\n";
    std::cout << " Project Authors / Contributors:\n";
    std::cout << "   * Aman Pokharia\n";
    std::cout << "   * Roshan Choudhary\n";
    std::cout << "   * Khushi Gupta\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
}

void UIHelper::printSectionHeader(const std::string& title) {
    std::cout << "\n>>> " << title << "\n";
    std::cout << std::string(title.length() + 4, '-') << "\n";
}

void UIHelper::printSuccess(const std::string& msg) {
    std::cout << "\n[SUCCESS] " << msg << "\n";
}

void UIHelper::printWarning(const std::string& msg) {
    std::cout << "\n[WARNING] " << msg << "\n";
}

void UIHelper::printError(const std::string& msg) {
    std::cout << "\n[ERROR] " << msg << "\n";
}

void UIHelper::printInfo(const std::string& msg) {
    std::cout << "\n[INFO] " << msg << "\n";
}

int UIHelper::getMenuChoice(int minOption, int maxOption) {
    int choice = -1;
    while (true) {
        std::cout << "Select Option (" << minOption << "-" << maxOption << "): ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            return maxOption; // Return Exit option on EOF
        }
        // Trim whitespace & CR/LF
        size_t first = input.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) {
            continue; // Skip empty newline inputs
        }
        size_t last = input.find_last_not_of(" \t\r\n");
        std::string clean = input.substr(first, (last - first + 1));

        try {
            choice = std::stoi(clean);
            if (choice >= minOption && choice <= maxOption) {
                return choice;
            }
        } catch (...) {}
        std::cout << "Invalid selection. Please enter a number between " 
                  << minOption << " and " << maxOption << ".\n";
    }
}

std::string UIHelper::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    if (!std::getline(std::cin, input)) {
        return "";
    }
    // Trim leading/trailing whitespace
    size_t first = input.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = input.find_last_not_of(" \t\r\n");
    return input.substr(first, (last - first + 1));
}

double UIHelper::readDouble(const std::string& prompt, double minVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) return minVal;
        try {
            double val = std::stod(input);
            if (val >= minVal) return val;
        } catch (...) {}
        std::cout << "Invalid input. Please enter a valid decimal number (>= " << minVal << ").\n";
    }
}

int UIHelper::readInt(const std::string& prompt, int minVal) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) return minVal;
        try {
            int val = std::stoi(input);
            if (val >= minVal) return val;
        } catch (...) {}
        std::cout << "Invalid input. Please enter a valid whole number (>= " << minVal << ").\n";
    }
}

} // namespace ANNADATU
