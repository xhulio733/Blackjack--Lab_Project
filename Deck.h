//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_DECK_H
#define BLACKJACK_DECK_H

#include <vector>
#include "Carta.h"

class Deck {

private:
    std::vector<Carta> carte;
    int nextCardindex;

public:
    Deck();
    void mix();
    Carta nextCard();
    bool endDeck() const;
};


#endif //BLACKJACK_DECK_H