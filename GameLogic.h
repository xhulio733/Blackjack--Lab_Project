//
// Created by Xhulio on 2/14/2026.
//

#ifndef BLACKJACK_GAMELOGIC_H
#define BLACKJACK_GAMELOGIC_H

#include <vector>
#include <string>

class GameLogic {
public:
    int countPoints(const std::vector<std::string>& hand) const;
};

#endif