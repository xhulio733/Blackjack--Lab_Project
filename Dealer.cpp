//
// Created by Xhulio on 2/5/2026.
//

#include "Dealer.h"   // Include la definizione della classe Dealer
#include <iostream>

/*
    Metodo che mostra la mano del Dealer.
    Parametro:
        showAll → se true mostra tutte le carte,
                  se false nasconde la prima carta.
*/
void Dealer::showHand(bool showAll) const {

    // Stampa intestazione
    std::cout << "Mano Dealer: ";

    // Se showAll è true, mostra tutte le carte
    if (showAll) {

        // Ciclo su tutte le carte della mano
        for (const auto& c : hand) {
            // Stampa valore e seme
            std::cout << c.getValue() << c.getType() << " ";
        }
        // Mostra il punteggio totale
        std::cout << "| Punti: " << countPoints() << std::endl;

    } else {
        // La prima carta viene coperta
        if (!hand.empty()) {
            // Mostra carta nascosta
            std::cout << "[??] ";
            // Se esiste una seconda carta, la mostra
            if (hand.size() > 1) {
                std::cout << hand[1].getValue()
                          << hand[1].getType() << " ";
            }
        }
        // Non mostra il punteggio con carta coperta
        std::cout << std::endl;
    }
}
/*
    Metodo che gestisce il turno automatico del Dealer.
    Il dealer pesca finché il punteggio è minore di 17.
*/
void Dealer::playTurn(Deck &deck) {
    while (countPoints() < 17) {
        // Prende la prossima carta dal mazzo
        Carta c = deck.nextCard();
        // Aggiunge la carta alla mano del dealer
        hit(c);
        // Stampa la carta pescata
        std::cout << "Dealer pesca: "
                  << c.getValue()
                  << c.getType()
                  << std::endl;
    }
    // Mostra mano completa dopo il turno
    showHand(true);

    // Se ha superato 21 → sballa
    if (bust())
        std::cout << "Dealer sballa!" << std::endl;
    else
        std::cout << "Dealer si ferma." << std::endl;
}
