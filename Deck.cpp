//
// Created by Xhulio on 2/5/2026.
//

#include "Deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() : nextCardindex(0) {
    const std::string type[4] = {"♠", "♥", "♣", "♦"};
    const std::string value[13] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

    for (const auto& t : type) {
        for (int i = 0; i < 13; ++i) {
            int point;
            if (value[i] == "A") point = 11;
            else if (value[i] == "J" || value[i] == "Q" || value[i] == "K") point = 10;
            else point = i+1;

            carte.emplace_back(t, value[i], point);
        }
    }
}

void Deck::mix() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(carte.begin(), carte.end(), std::default_random_engine(seed));
    nextCardindex = 0;
}

Carta Deck::nextCard() {
    if (nextCardindex < (int)carte.size()) return carte[nextCardindex++];
    return carte[0];
}

bool Deck::endDeck() const {
    return nextCardindex >= (int)carte.size();
}
