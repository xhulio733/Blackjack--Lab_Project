//
// Created by Xhulio on 2/5/2026.
//

#include "Player.h"

#include "Player.h"
#include <iostream>

Player::Player(int saldo) : balance(saldo), currentBet(0) {}

void Player::hit(const Carta &c) {
    hand.push_back(c);
}

void Player::clearHand() {
    hand.clear();
}

int Player::countPoints() const {
    int sum = 0;
    int aces = 0;
    for (const auto& c : hand) {
        sum += c.getPoint();
        if (c.getValue() == "A") aces++;
    }
    while (sum > 21 && aces > 0) {
        sum -= 10;
        aces--;
    }
    return sum;
}

bool Player::bust() const {
    return countPoints() > 21;
}

bool Player::placeBet(int amount) {
    if (amount > 0 && amount <= balance) {
        currentBet = amount;
        balance -= amount;
        return true;
    }
    return false;
}

int Player::getBalance() const { return balance; }
void Player::deposit(int amount) { balance += amount; }
void Player::withdraw(int amount) { if (amount <= balance) balance -= amount; }
int Player::getBet() const { return currentBet; }
void Player::clearBet() { currentBet = 0; }
const std::vector<Carta>& Player::getHand() const { return hand; }

void Player::showHand() const {
    std::cout << "Mano: ";
    for (const auto& c : hand) std::cout << c.getValue() << c.getType() << " ";
    std::cout << "| Punti: " << countPoints() << std::endl;
}