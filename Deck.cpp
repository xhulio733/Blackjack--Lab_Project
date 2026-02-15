//
// Created by Xhulio on 2/5/2026.
//

#include "Deck.h"        // Include la definizione della classe Deck
#include <algorithm>     // Necessario per std::shuffle
#include <random>        // Necessario per generare numeri casuali
#include <chrono>        // Necessario per creare un seed basato sul tempo

/*
    Costruttore della classe Deck.
    Inizializza il mazzo di carte standard (52 carte).
*/
Deck::Deck() : nextCardindex(0) {   // nextCardindex parte da 0 (prima carta)

    // Array dei 4 semi delle carte
    const std::string type[4] = {"♠", "♥", "♣", "♦"};

    // Array dei valori possibili delle carte
    const std::string value[13] = {
        "A","2","3","4","5","6","7","8","9","10","J","Q","K"
    };
    /*
        Doppio ciclo:
        - Primo ciclo: scorre i semi
        - Secondo ciclo: scorre i valori
        In questo modo vengono generate 4 × 13 = 52 carte
    */
    for (const auto& t : type) {
        for (int i = 0; i < 13; ++i) {

            int point;  // Punteggio della carta

            // Se la carta è un Asso vale 11 inizialmente
            if (value[i] == "A")
                point = 11;

            // Se è una figura (J, Q, K) vale 10
            else if (value[i] == "J" || value[i] == "Q" || value[i] == "K")
                point = 10;

            // Altrimenti è una carta numerica (2–10)
            else
                point = i + 1;  // Perché l'indice parte da 0
            /*
                Crea un oggetto Carta e lo aggiunge al vettore "carte"
                emplace_back costruisce l'oggetto direttamente nel vettore
                senza creare copie
            */
            carte.emplace_back(t, value[i], point);
        }
    }
}

void Deck::mix() {
    /*
        Genera un seed casuale usando il tempo attuale.
        Ogni volta che il programma parte, il mazzo sarà diverso.
    */
    unsigned seed =std::chrono::system_clock::now().time_since_epoch().count();

    // Mischia le carte usando std::shuffle
    std::shuffle(carte.begin(), carte.end(),std::default_random_engine(seed));

    // Dopo il mix si riparte dalla prima carta
    nextCardindex = 0;
}
/*
    Restituisce la prossima carta disponibile nel mazzo.
*/
Carta Deck::nextCard() {

    // Se non siamo arrivati alla fine del mazzo
    if (nextCardindex < (int)carte.size())
        return carte[nextCardindex++];  // Restituisce carta e incrementa indice
    // Se il mazzo è finito, restituisce la prima carta
    return carte[0];
}
/*
    Controlla se il mazzo è terminato.
    Ritorna true se non ci sono più carte disponibili.
*/
bool Deck::endDeck() const {
    return nextCardindex >= (int)carte.size();
}
