//
// Created by Xhulio on 2/14/2026.
//

#ifndef BLACKJACK_GAMEGRAPHICS_H
#define BLACKJACK_GAMEGRAPHICS_H
#include <SFML/Graphics.hpp>
#include <filesystem> //Libreria usata per lavorare con le cartelle
#include <unordered_map> //Contenitore chiave valore usato per transformare nome della carta in texture
#include <vector>
#include <string>
#include "GameLogic.h" //Include la classe GameLogic

namespace fs = std::filesystem; // Alias per rendere più corto std::filesystem

class GameGraphics {
public:
    //Costruttore della classe
    GameGraphics();
    //Avvia la grafica del gioco
    void run();

private:
    //Contiene la logica del gioco
    GameLogic logic;
};

#endif

//BLACKJACK_GAMEGRAPHICS_H