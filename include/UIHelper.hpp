#ifndef ANNADATU_UIHELPER_HPP
#define ANNADATU_UIHELPER_HPP

#include "Common.hpp"

namespace ANNADATU {

class UIHelper {
public:
    static void clearScreen();
    static void pause();

    static void printMainBanner();
    static void printTeamCredits();
    static void printSectionHeader(const std::string& title);
    static void printSuccess(const std::string& msg);
    static void printWarning(const std::string& msg);
    static void printError(const std::string& msg);
    static void printInfo(const std::string& msg);

    static int getMenuChoice(int minOption, int maxOption);
    static std::string readLine(const std::string& prompt);
    static double readDouble(const std::string& prompt, double minVal = 0.0);
    static int readInt(const std::string& prompt, int minVal = 0);
};

} // namespace ANNADATU

#endif // ANNADATU_UIHELPER_HPP
