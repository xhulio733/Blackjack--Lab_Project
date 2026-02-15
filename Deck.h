//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_DECK_H
#define BLACKJACK_DECK_H

#include <vector>
#include "Carta.h"

class Deck {

private:
    // Vettore che contiene tutte le carte del mazzo (52 carte)
    std::vector<Carta> carte;
    // Indice della prossima carta da pescare.
    int nextCardindex;

public:
    //Crea un mazzo di 52 carte
    Deck();
    //Mescola le carte e fa ripartire a 0 index
    void mix();
    //Restituisce la prossima carta
    Carta nextCard();
    //Controlla se il mazzo è terminato.
    bool endDeck() const;
};


#endif //BLACKJACK_DECK_H