//
// Created by Xhulio on 2/5/2026.
//

#include "Player.h"   // Include la definizione della classe Player
#include <iostream>
/*
    Costruttore della classe Player.
    Inizializza il saldo iniziale e azzera la puntata corrente.
*/
Player::Player(int saldo)
    : balance(saldo), currentBet(0) {}
/*
    Metodo hit:
    Aggiunge una carta alla mano del giocatore.
*/
void Player::hit(const Carta &c) {
    // Inserisce la carta nel vettore hand
    hand.push_back(c);
}
/*
    Metodo clearHand:
    Svuota completamente la mano del giocatore.
*/
void Player::clearHand() {
    hand.clear();
}
/*
    Metodo countPoints:
    Calcola il punteggio totale della mano del giocatore
*/
int Player::countPoints() const {

    int sum = 0;   // Somma totale punti
    int aces = 0;  // Conta quanti assi sono presenti
    // Ciclo su tutte le carte della mano
    for (const auto& c : hand) {
        // Aggiunge il valore numerico della carta
        sum += c.getPoint();
        // Se la carta è un Asso, incrementa il contatore
        if (c.getValue() == "A")
            aces++;
    }
    /*
        Se la somma supera 21 e ci sono assi,
        converte un asso da 11 a 1
    */
    while (sum > 21 && aces > 0) {
        sum -= 10;
        aces--;
    }

    return sum;   // Restituisce il punteggio finale
}
/*
    Metodo bust:
    Ritorna true se il giocatore ha superato 21 punti.
*/
bool Player::bust() const {
    return countPoints() > 21;
}
/*
    Metodo placeBet:
    Permette al giocatore di piazzare una puntata.
    Ritorna true se la puntata è valida.
*/
bool Player::placeBet(int amount) {
    // Controlla che la puntata sia positiva e non superiore al saldo
    if (amount > 0 && amount <= balance) {
        currentBet = amount;   // Salva puntata corrente
        balance -= amount;     // Scala dal saldo
        return true;
    }
    return false;   // Puntata non valida
}
/*
    Restituisce il saldo attuale del giocatore.
*/
int Player::getBalance() const {
    return balance;
}
/*
    Aggiunge denaro al saldo in caso di vittoria
*/
void Player::deposit(int amount) {
    balance += amount;
}
/*
    Rimuove denaro dal saldo (se sufficiente).
*/
void Player::withdraw(int amount) {
    if (amount <= balance)
        balance -= amount;
}
/*
    Restituisce la puntata corrente.
*/
int Player::getBet() const {
    return currentBet;
}
/*
    Azzera la puntata corrente.
*/
void Player::clearBet() {
    currentBet = 0;
}
/*
    Restituisce la mano del giocatore.
    Ritorna una reference costante al vettore.
*/
const std::vector<Carta>& Player::getHand() const {
    return hand;
}
/*
    Metodo showHand:
    Stampa a console tutte le carte del giocatore
    e il punteggio totale.
*/
void Player::showHand() const {
    std::cout << "Mano: ";
    // Ciclo su tutte le carte e le stampa
    for (const auto& c : hand)
        std::cout << c.getValue() << c.getType() << " ";
    // Mostra il totale punti
    std::cout << "| Punti: " << countPoints() << std::endl;
}