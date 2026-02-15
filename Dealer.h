//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_DEALER_H
#define BLACKJACK_DEALER_H

#include "Player.h"
#include "Deck.h"
//La classe Dealer eridita i metodi della calsse player hand, hit(), countPoints(), bust()...
class Dealer : public Player {

public:
    //Chiama il costruttore della classe Player e crea Dealer senza saldo
    Dealer() : Player(0) {}
    //Se bool true mostra carte e punteggio se e false nasconde una carta del dealer
    void showHand(bool revealAll) const;
    //Gestione mano del dealer automatica. Riceve deck come parametro per chiamare la prossima carta
    void playTurn(Deck& deck);
};


#endif //BLACKJACK_DEALER_H