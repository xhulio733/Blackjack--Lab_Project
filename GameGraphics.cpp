//
// Created by Xhulio on 2/14/2026.
//
#include "GameGraphics.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

// Costruttore vuoto della classe GameGraphics
GameGraphics::GameGraphics() {}

// Funzione principale che avvia tutta la parte grafica del gioco
void GameGraphics::run() {

    // Crea la finestra di gioco 1200x800 con titolo
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Blackjack Pro");
    // Limita il frame rate a 60 FPS
    window.setFramerateLimit(60);

    // Caricamento font
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Font error\n"; return;
    }

    // Cartella dove si trovano le immagini delle carte
    std::string cardsFolder = "C:/Users/Xhulio/CLionProjects/Blackjack1/Cards/PNG/Cards (large)/";
    // Mappa nome carta → texture
    std::unordered_map<std::string, sf::Texture> textures;
    // Mappa nome carta → sprite (oggetto disegnabile)
    std::unordered_map<std::string, sf::Sprite> cardSprites;

    // Controlla che la cartella esista
    if (!fs::exists(cardsFolder)) {
        std::cerr << "Errore cartella non trovata\n"; return;
    }

    // Carica tutte le carte PNG nella mappa
    for (const auto& e : fs::directory_iterator(cardsFolder)) {
        if (e.path().extension() == ".png") {
            std::string name = e.path().stem().string();
            // Salta la carta retro
            if (name == "card_back")
                continue;
            // Carica la texture
            textures[name].loadFromFile(e.path().string());
            // Crea sprite collegato alla texture
            cardSprites[name] = sf::Sprite(textures[name]);
        }
    }

    // Carica la texture del retro carta
    sf::Texture backTexture;
    if (!backTexture.loadFromFile(cardsFolder + "card_back.png"))
        return;

    // Crea sprite retro carta
    sf::Sprite backSprite(backTexture);

    int balance = 1000;
    bool playing = true;

    // Lambda per distribuire una carta
    auto dealCard = [&](std::vector<sf::Sprite>& hand,
                        std::vector<std::string>& names,
                        std::vector<std::string>& deck,
                        std::unordered_map<std::string, sf::Sprite>& cardSprites,
                        float y) {
        if (deck.empty()) return;

        // Prende ultima carta dal mazzo
        std::string c = deck.back(); deck.pop_back();
        // Crea sprite copia
        sf::Sprite s = cardSprites[c];
        // Posiziona carta
        s.setPosition(100 + hand.size() * 140, y);
        // Aggiunge carta alla mano
        hand.push_back(s);
        // Salva nome carta per calcolo punteggio
        names.push_back(c);
    };

    // Ciclo principale del gioco
    while (playing && balance > 0) {
        // Crea mazzo
        std::vector<std::string> deck;
        for (auto& c : cardSprites) deck.push_back(c.first);
        // Crea mazzo
        std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device{}()));

        int bet = 0;
        bool betSelected = false; // Flag se puntata confermata

        // Creazione bottoni puntata
        sf::RectangleShape bet5({80,50});  bet5.setPosition(50,650);
        sf::RectangleShape bet10({80,50}); bet10.setPosition(150,650);
        sf::RectangleShape bet25({80,50}); bet25.setPosition(250,650);
        sf::RectangleShape bet50({80,50}); bet50.setPosition(350,650);

        sf::RectangleShape resetBtn({100, 50}); resetBtn.setPosition(480, 650);
        sf::RectangleShape startBtn({150,60});  startBtn.setPosition(650,645);

        // Colori bottoni
        bet5.setFillColor(sf::Color::White);
        bet10.setFillColor(sf::Color::White);
        bet25.setFillColor(sf::Color::White);
        bet50.setFillColor(sf::Color::Cyan);
        resetBtn.setFillColor(sf::Color(255, 140, 0));
        startBtn.setFillColor(sf::Color::Magenta);

        // Testi bottoni
        sf::Text t5("5", font, 24);   t5.setPosition(80,660);   t5.setFillColor(sf::Color::Black);
        sf::Text t10("10", font, 24); t10.setPosition(175,660); t10.setFillColor(sf::Color::Black);
        sf::Text t25("25", font, 24); t25.setPosition(275,660); t25.setFillColor(sf::Color::Black);
        sf::Text t50("50", font, 24); t50.setPosition(375,660); t50.setFillColor(sf::Color::Black);

        sf::Text tReset("RESET", font, 20); tReset.setPosition(495, 662); tReset.setFillColor(sf::Color::Black);
        sf::Text tStart("DEAL", font, 24);  tStart.setPosition(690,660);  tStart.setFillColor(sf::Color::Black);

        sf::Text betText("Bet: $0", font, 24); betText.setPosition(950,250);
        sf::Text balanceText("Balance: $" + std::to_string(balance), font, 24); balanceText.setPosition(950,200);

        // FASE SCELTA PUNTATA
        while (window.isOpen() && !betSelected) {
            sf::Event ev;

            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) return;

                if (ev.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);

                    // Se clic su bottoni puntata
                    if (bet5.getGlobalBounds().contains(m) && balance >= bet + 5)
                        bet += 5;

                    if (bet10.getGlobalBounds().contains(m) && balance >= bet + 10)
                        bet += 10;

                    if (bet25.getGlobalBounds().contains(m) && balance >= bet + 25)
                        bet += 25;

                    if (bet50.getGlobalBounds().contains(m) && balance >= bet + 50)
                        bet += 50;

                    if (resetBtn.getGlobalBounds().contains(m))
                        bet = 0;

                    // Conferma puntata
                    if (startBtn.getGlobalBounds().contains(m) && bet > 0) {
                        betSelected = true;
                        balance -= bet;
                    }

                    betText.setString("Bet: $" + std::to_string(bet));
                }
            }

            //CLEAR del frame precedente
            window.clear(sf::Color(0, 100, 0));
            //Disegna i vari bottoni
            window.draw(bet5);
            window.draw(bet10);
            window.draw(bet25);
            window.draw(bet50);
            window.draw(startBtn);
            window.draw(resetBtn);
            window.draw(t5);
            window.draw(t10);
            window.draw(t25);
            window.draw(t50);
            window.draw(tStart);
            window.draw(tReset);
            window.draw(betText);
            window.draw(balanceText);
            //Mostra tutto
            window.display();
        }

        // Mani giocatore e dealer
        std::vector<sf::Sprite> playerHand, dealerHand;
        std::vector<std::string> playerNames, dealerNames;

        // Distribuisce carte iniziali
        dealCard(playerHand, playerNames, deck, cardSprites, 600);
        dealCard(dealerHand, dealerNames, deck, cardSprites, 100);
        dealCard(playerHand, playerNames, deck, cardSprites, 600);
        dealCard(dealerHand, dealerNames, deck, cardSprites, 100);

        bool playerTurn = true;
        bool dealerTurn = false;
        bool hideDealer = true;
        bool handOver = false;
        int pPoints = logic.countPoints(playerNames);

        // BOTTONI AZIONE
        sf::RectangleShape hit({100,50}); hit.setPosition(800,650);
        sf::RectangleShape stand({100,50}); stand.setPosition(940,650);
        sf::RectangleShape dbl({100,50}); dbl.setPosition(1080,650);

        hit.setFillColor(sf::Color::Blue);
        stand.setFillColor(sf::Color::Red);
        dbl.setFillColor(sf::Color(255, 200, 0));

        sf::Text hitT("HIT", font, 24); hitT.setPosition(830,660);
        sf::Text standT("STAND", font, 24); standT.setPosition(950,660);
        sf::Text dblT("DBL", font, 24); dblT.setPosition(1105,660); dblT.setFillColor(sf::Color::Black);

        sf::Text result("", font, 36); result.setPosition(500,400); result.setFillColor(sf::Color::Yellow);
        sf::Text pScore("", font, 30); pScore.setPosition(100, 550); pScore.setFillColor(sf::Color::White);
        sf::Text dScore("", font, 30); dScore.setPosition(100, 50); dScore.setFillColor(sf::Color::White);

        /*
            Lambda drawScene.
            Questa funzione si occupa di disegnare TUTTA la scena di gioco:
            carte, bottoni, punteggi, saldo, risultato finale.
            Viene richiamata ad ogni frame.
        */
        auto drawScene = [&]() {
            // Pulisce la finestra con sfondo verde (tavolo da blackjack)
            window.clear(sf::Color(0, 100, 0));

            //Disegno carte del dealer.
            for (size_t i = 0; i < dealerHand.size(); ++i) {
                // Se è la prima carta e il dealer è nascosto
                if (i == 0 && hideDealer) {
                    // Posiziona la carta coperta sopra la carta reale
                    backSprite.setPosition(dealerHand[i].getPosition());
                    // Disegna il retro carta
                    window.draw(backSprite);
                } else {
                    // Disegna normalmente la carta del dealer
                    window.draw(dealerHand[i]);
                }
            }
            //Disegno tutte le carte del giocatore
            for (auto& c : playerHand) {
                window.draw(c);
            }
            //Disegno Pulsanti Principali
            window.draw(hit);
            window.draw(stand);
            window.draw(hitT);
            window.draw(standT);
            //Mostra il Double secondo le regole
            if (playerTurn && playerHand.size() == 2 && balance >= bet) {
                window.draw(dbl);
                window.draw(dblT);
            }
            //Aggiorno testo puntata e saldo
            betText.setString("Bet: $" + std::to_string(bet));
            balanceText.setString("Balance: $" + std::to_string(balance));
            //Li metto in mostra
            window.draw(betText);
            window.draw(balanceText);
            //Calcolo e mostro punteggio carte giocatore
            pScore.setString("Player: " + std::to_string(logic.countPoints(playerNames)));
            window.draw(pScore);
            //Calcolo e mostro punteggio dealer
            int dPointsVis = 0;

            if (hideDealer) {
                //Se dealer ha carta coperta fo un vett temp per salvare punteggio iniziale
                std::vector<std::string> temp;

                for(size_t i=1; i<dealerNames.size(); i++) { //ignora la carta coperta (sa gia il valore)
                    temp.push_back(dealerNames[i]); //Aggiunge la carta visibile in un nuovo vettore chiamato temp
                }//cosi da contare solo la carta visibile che ce nel temp
                //Calcolo punteggio
                dPointsVis = logic.countPoints(temp);

            } else {
                // Se il dealer è scoperto, calcolo tutto normalmente
                dPointsVis = logic.countPoints(dealerNames);
            }
            //Aggiorno punteggio Dealer
            dScore.setString("Dealer: " + std::to_string(dPointsVis));
            window.draw(dScore);

            if (handOver)
                window.draw(result);

            window.display();
        };

        // Se Blackjack immediato
        if (pPoints == 21) {

            playerTurn = false;
            dealerTurn = false;
            hideDealer = false;
            handOver = true;

            result.setString("BLACKJACK! Payout 3:2");
            balance += (bet + (bet * 1.5));
            hitT.setString("PLAY"); standT.setString("EXIT");
        }

        bool restartHand = false;

        // CICLO MANO
        while (window.isOpen() && !restartHand) {

            sf::Event ev;
            while (window.pollEvent(ev)) {
                // Se l’utente chiude la finestra, termina il programma
                if (ev.type == sf::Event::Closed)
                    return;
                    /*
                        Se viene premuto un pulsante del mouse,
                        recuperiamo la posizione del click.
                    */
                if (ev.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                         /*
                             Se la mano NON è ancora terminata,
                             significa che siamo nella fase attiva di gioco.
                        */
                    if (!handOver) {

                        if (playerTurn) {
                            //Gestione del HIT
                            if (hit.getGlobalBounds().contains(m)) {
                                // Il giocatore pesca una carta
                                dealCard(playerHand, playerNames, deck, cardSprites, 600);
                                pPoints = logic.countPoints(playerNames);
                                //Se supera 21 fa Bust
                                if (pPoints > 21) {
                                    playerTurn = false;
                                    handOver = true;
                                    hideDealer = false;
                                    result.setString("BUST!");
                                    hitT.setString("PLAY");
                                    standT.setString("EXIT");
                                    //Se fa 21 tocca al dealer
                                } else if (pPoints == 21) {
                                    playerTurn = false;
                                    dealerTurn = true;
                                    hideDealer = false;
                                }
                            }

                            // STAND
                            if (stand.getGlobalBounds().contains(m)) {
                                playerTurn = false;
                                dealerTurn = true;
                                hideDealer = false;
                            }
                            //Gestione del Double. Solo se Player ha 2 carte e Balance sufficente
                            if (playerHand.size() == 2 && balance >= bet && dbl.getGlobalBounds().contains(m)) {
                                balance -= bet;
                                bet *= 2;
                                dealCard(playerHand, playerNames, deck, cardSprites, 600);
                                pPoints = logic.countPoints(playerNames);
                                //Bust del Double
                                if (pPoints > 21) {
                                    playerTurn = false;
                                    handOver = true;
                                    hideDealer = false;
                                    result.setString("BUST (DOUBLE)!");
                                    hitT.setString("PLAY");
                                    standT.setString("EXIT");
                                } else {
                                    playerTurn = false;
                                    dealerTurn = true;
                                    hideDealer = false;
                                }
                            }
                        }
                            //Cambio buttoni alla fine Hit diventa Play e Stand Exit
                    } else {
                        if (hit.getGlobalBounds().contains(m))
                            restartHand = true;
                        if (stand.getGlobalBounds().contains(m))
                            return;
                    }
                }
            }

            // TURNO DEALER dopo Il player
            if (dealerTurn && !handOver) {
                hideDealer = false; // Mostra la prima carta
                drawScene();
                std::this_thread::sleep_for(std::chrono::milliseconds(800)); //Ritardo in ms per male di cuore

                int dealerPoints = logic.countPoints(dealerNames);

                while (dealerPoints < 17 && !deck.empty()) {
                    dealCard(dealerHand, dealerNames, deck, cardSprites, 100);

                    dealerPoints = logic.countPoints(dealerNames);
                    drawScene();
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }

                if (dealerPoints > 21) {
                    result.setString("DEALER BUST! YOU WIN");
                    balance += (bet * 2);
                }
                else if (pPoints > dealerPoints) {
                    result.setString("YOU WIN");
                    balance += (bet * 2);
                }
                else if (pPoints < dealerPoints) {
                    result.setString("YOU LOSE");
                }
                else {
                    result.setString("PUSH (TIE)");
                    balance += bet;
                }

                dealerTurn = false;
                handOver = true;
                hitT.setString("PLAY");
                standT.setString("EXIT");
            }

            drawScene();
        }
    }
}


