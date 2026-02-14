//
// Created by Xhulio on 2/14/2026.
//

#include "GameLogic.h"
#include <cctype>

int GameLogic::countPoints(const std::vector<std::string>& hand) const {
    int sum = 0;
    int aceCount = 0;

    for (const auto& name : hand) {
        int val = 0;
        if (name.find('J') != std::string::npos) val = 10;
        else if (name.find('Q') != std::string::npos) val = 10;
        else if (name.find('K') != std::string::npos) val = 10;
        else if (name.find('A') != std::string::npos && name.find("10") == std::string::npos) {
            val = 11; aceCount++;
        }
        else {
            std::string numStr = "";
            for (char c : name) { if (isdigit(c)) numStr += c; }
            if (!numStr.empty()) {
                try { val = std::stoi(numStr); }
                catch (...) { val = 0; }
            }
        }
        sum += val;
    }

    while (sum > 21 && aceCount > 0) {
        sum -= 10;
        aceCount--;
    }

    return sum;
}

