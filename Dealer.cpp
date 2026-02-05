//
// Created by Xhulio on 2/5/2026.
//

#include "Dealer.h"
#include <iostream>

void Dealer::showHand(bool showAll) const {
    std::cout << "Mano Dealer: ";

    if (showAll) {
        // Mostra tutte le carte e i punti
        for (const auto& c : hand) {
            std::cout << c.getValue() << c.getType() << " ";
        }
        std::cout << "| Punti: " << countPoints() << std::endl;
    } else {
        // Mostra la prima nascosta e la seconda visibile
        if (!hand.empty()) {
            std::cout << "[??] ";
            if (hand.size() > 1) {
                std::cout << hand[1].getValue() << hand[1].getType() << " ";
            }
        }
        std::cout << std::endl;  // nessun punteggio qui!
    }
}
void Dealer::playTurn(Deck &deck) {
    while (countPoints() < 17) {
        Carta c = deck.nextCard();
        hit(c);
        std::cout << "Dealer pesca: " << c.getValue() << c.getType() << std::endl;
    }
    showHand(true);
    if (bust()) std::cout << "Dealer sballa!" << std::endl;
    else std::cout << "Dealer si ferma." << std::endl;
}