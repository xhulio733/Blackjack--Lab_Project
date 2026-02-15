//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_PLAYER_H
#define BLACKJACK_PLAYER_H

#include <vector>
#include "Carta.h"

class Player {

protected:
    // Vettore che contiene tutte le carte del giocatore
    std::vector<Carta> hand;
    // Saldo totale del giocatore
    int balance;
    // Puntata corrente della mano in corso
    int currentBet;

public:
    //Costrutore che inizializa il Player con saldo iniziale
    Player(int saldo);
    //Aggiunge una carta in mano al plyer se hit
    void hit(const Carta& c);
    //Svuota la mano . Play Again
    void clearHand();
    int countPoints() const;
    //True se e >21
    bool bust() const;
    //I bet del giocatore
    bool placeBet(int amount);
    //Saldo attuale
    int getBalance() const;
    //Aggiunge soldi al saldo se Win
    void deposit(int amount);
    //Leva soldi se Lose
    void withdraw(int amount);
    //La puntata fatta
    int getBet() const;
    //Pulisce la puntata corrente quando si riparte
    void clearBet();
    //Ritorna la mano del player. Come const per evitare modifiche non volute
    const std::vector<Carta>& getHand() const;
    //Stampa punteggio totale e la mano
    void showHand() const;
};

#endif //BLACKJACK_PLAYER_H