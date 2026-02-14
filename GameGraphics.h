//
// Created by Xhulio on 2/14/2026.
//

#ifndef BLACKJACK_GAMEGRAPHICS_H
#define BLACKJACK_GAMEGRAPHICS_H
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include "GameLogic.h"

namespace fs = std::filesystem;

class GameGraphics {
public:
    GameGraphics();
    void run();

private:
    GameLogic logic;
};

#endif

//BLACKJACK_GAMEGRAPHICS_H