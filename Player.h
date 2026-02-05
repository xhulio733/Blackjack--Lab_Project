//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_PLAYER_H
#define BLACKJACK_PLAYER_H

#include <vector>
#include "Carta.h"

class Player {
protected:
    std::vector<Carta> hand;
    int balance;
    int currentBet;

public:
    Player(int saldo);
    void hit(const Carta& c);
    void clearHand();
    int countPoints() const;
    bool bust() const;
    bool placeBet(int amount);
    int getBalance() const;
    void deposit(int amount);
    void withdraw(int amount);
    int getBet() const;
    void clearBet();
    const std::vector<Carta>& getHand() const;
    void showHand() const;
};


#endif //BLACKJACK_PLAYER_H