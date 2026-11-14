#include "ANNADATUApp.hpp"
#include <iostream>

int main() {
    try {
        ANNADATU::ANNADATUApp app;
        app.run();
    } catch (const std::exception& ex) {
        std::cerr << "\n[Fatal Application Exception]: " << ex.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "\n[Unknown Fatal Exception encountered]\n";
        return 1;
    }
    return 0;
}
