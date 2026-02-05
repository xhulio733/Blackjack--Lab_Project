//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include "Player.h"
#include "Deck.h"

class Dealer : public Player {

public:
    Dealer() : Player(0) {}
    void showHand(bool revealAll) const;
    void playTurn(Deck& deck);
};


#endif //BLACKJACK_DEALER_H