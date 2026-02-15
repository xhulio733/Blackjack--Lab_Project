//
// Created by Xhulio on 2/14/2026.
//

#include "GameLogic.h"
#include <cctype>  // Necessario per usare la funzione isdigit()
#include <iostream>
/*
    Metodo della classe GameLogic che calcola il punteggio totale
    di una mano nel Blackjack.
    Parametro:
        hand → vettore di stringhe che rappresentano le carte
    Ritorna:
        Il punteggio totale della mano secondo le regole del Blackjack.
*/
int GameLogic::countPoints(const std::vector<std::string>& hand) const {

    // Variabile che accumula la somma totale dei punti
    int sum = 0;

    // Conta quanti assi sono presenti nella mano.
    int aceCount = 0;

    /*
        Ciclo su ogni carta della mano.
        "nome" rappresenta il nome della carta.
    */
    for (const auto& name : hand) {

        // Valore numerico della singola carta corrente
        int val = 0;

        /*
            Controllo se la carta è una figura (J, Q, K).
            Nel Blackjack le figure valgono 10 punti.
        */
        if (name.find('J') != std::string::npos)
            val = 10;

        else if (name.find('Q') != std::string::npos)
            val = 10;

        else if (name.find('K') != std::string::npos)
            val = 10;

        /*
            Controllo se la carta è un Asso.
            L'Asso viene inizialmente considerato 11 punti.
            name.find("10") == npos è una protezione aggiuntiva
            per evitare ambiguità nel parsing.
        */
        else if (name.find('A') != std::string::npos &&
                 name.find("10") == std::string::npos) {

            val = 11;      // L'asso parte come 11
            aceCount++;    // Incremento il numero di assi presenti
        }

        /*
            Se non è figura né asso,
            allora è una carta numerica (2–10).
            Estraggo solo le cifre dalla stringa.
        */
        else {

            // Stringa temporanea che conterrà solo le cifre
            std::string numStr = "";

            // Ciclo su ogni carattere della carta
            for (char c : name) {
                // Se il carattere è una cifra (0–9), lo aggiungo alla stringa
                if (isdigit(c))
                    numStr += c;
            }

            /*
                Se la stringa numerica non è vuota,
                provo a convertirla in intero.
            */
            if (!numStr.empty()) {
                try {
                    // Converte la stringa numerica in intero
                    val = std::stoi(numStr);
                }
                catch (...) {
                    // In caso di errore di conversione,
                    // assegno 0 per evitare crash del programma
                    val = 0;
                }
            }
        }

        // Aggiungo il valore della carta alla somma totale
        sum += val;
    }
    /*
        Gestione dinamica degli assi:
        Se la somma supera 21 e ci sono assi conteggiati come 11,
        converto un asso alla volta da 11 a 1.
    */
    while (sum > 21 && aceCount > 0) {
        sum -= 10;     // Trasforma un asso da 11 a 1
        aceCount--;
    }

    // Restituisco il punteggio finale
    return sum;
}
