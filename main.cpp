#include <SFML/Graphics.hpp> // include tutto ciò che serve per graphics, window, system
//Finestra di SFML che parte con un cerchio rosso
/*
int main() {
    sf::RenderWindow window(sf::VideoMode(200, 200), "Blackjack");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
*/
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <thread> // Serve per l'animazione (sleep)
#include <chrono> // Serve per il tempo

namespace fs = std::filesystem;

// ------------------ COUNT POINTS ------------------
// Funzione robusta per leggere i punti dai nomi dei file
int countPoints(const std::vector<std::string>& hand) {
    int sum = 0;
    int aceCount = 0;

    for (const auto& name : hand) {
        int val = 0;

        // Controllo Figure
        if (name.find('J') != std::string::npos) val = 10;
        else if (name.find('Q') != std::string::npos) val = 10;
        else if (name.find('K') != std::string::npos) val = 10;
        // Controllo Asso
        else if (name.find('A') != std::string::npos && name.find("10") == std::string::npos) {
            val = 11;
            aceCount++;
        }
        else {
            // Cerca numeri
            std::string numStr = "";
            for (char c : name) { if (isdigit(c)) numStr += c; }
            if (!numStr.empty()) { try { val = std::stoi(numStr); } catch (...) { val = 0; } }
        }
        sum += val;
    }

    while (sum > 21 && aceCount > 0) { sum -= 10; aceCount--; }
    return sum;
}

// ------------------ MAIN ------------------
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Blackjack");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Font error\n"; return 1;
    }

    // ------------------ LOAD CARDS ------------------
    std::string cardsFolder = "C:/Users/Xhulio/CLionProjects/Blackjack1/Cards/PNG/Cards (large)/";
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Sprite> cardSprites;

    if (!fs::exists(cardsFolder)) {
        std::cerr << "Errore cartella non trovata\n"; return 1;
    }

    for (const auto& e : fs::directory_iterator(cardsFolder)) {
        if (e.path().extension() == ".png") {
            std::string name = e.path().stem().string();
            if (name == "card_back") continue;
            textures[name].loadFromFile(e.path().string());
            cardSprites[name] = sf::Sprite(textures[name]);
        }
    }

    sf::Texture backTexture;
    if (!backTexture.loadFromFile(cardsFolder + "card_back.png")) { return 1; }
    sf::Sprite backSprite(backTexture);

    // ------------------ GAME VARIABLES ------------------
    int balance = 100;
    bool playing = true;

    auto dealCard = [&](std::vector<sf::Sprite>& hand,
                        std::vector<std::string>& names,
                        std::vector<std::string>& deck,
                        std::unordered_map<std::string, sf::Sprite>& cardSprites,
                        float y) {
        if (deck.empty()) return;
        std::string c = deck.back(); deck.pop_back();
        sf::Sprite s = cardSprites[c];
        s.setPosition(100 + hand.size() * 140, y);
        hand.push_back(s);
        names.push_back(c);
    };

    while (playing && balance > 0) {
        // ------------------ SHUFFLE ------------------
        std::vector<std::string> deck;
        for (auto& c : cardSprites) deck.push_back(c.first);
        std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device{}()));

        // ------------------ BET SELECTION ------------------
        int bet = 0;
        bool betSelected = false;

        sf::RectangleShape bet5({100,50}); bet5.setPosition(100,650);
        sf::RectangleShape bet10({100,50}); bet10.setPosition(250,650);
        sf::RectangleShape bet25({100,50}); bet25.setPosition(400,650);
        sf::RectangleShape startBtn({150,60}); startBtn.setPosition(600,650);

        bet5.setFillColor(sf::Color::White); bet10.setFillColor(sf::Color::White);
        bet25.setFillColor(sf::Color::White); startBtn.setFillColor(sf::Color::Magenta);

        sf::Text t5("5", font, 24); t5.setPosition(140,660); t5.setFillColor(sf::Color::Black);
        sf::Text t10("10", font, 24); t10.setPosition(280,660); t10.setFillColor(sf::Color::Black);
        sf::Text t25("25", font, 24); t25.setPosition(430,660); t25.setFillColor(sf::Color::Black);
        sf::Text tStart("START", font, 24); tStart.setPosition(635,665); tStart.setFillColor(sf::Color::Black);

        sf::Text betText("Bet: $0", font, 24); betText.setPosition(950,250);
        sf::Text balanceText("Balance: $" + std::to_string(balance), font, 24); balanceText.setPosition(950,200);

        while (window.isOpen() && !betSelected) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) return 0;
                if (ev.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                    if (bet5.getGlobalBounds().contains(m) && balance >= bet + 5) bet += 5;
                    if (bet10.getGlobalBounds().contains(m) && balance >= bet + 10) bet += 10;
                    if (bet25.getGlobalBounds().contains(m) && balance >= bet + 25) bet += 25;
                    if (startBtn.getGlobalBounds().contains(m) && bet > 0) betSelected = true;
                    betText.setString("Bet: $" + std::to_string(bet));
                }
            }
            window.clear(sf::Color(0, 100, 0));
            window.draw(bet5); window.draw(bet10); window.draw(bet25); window.draw(startBtn);
            window.draw(t5); window.draw(t10); window.draw(t25); window.draw(tStart);
            window.draw(betText); window.draw(balanceText);
            window.display();
        }

        // ------------------ DEAL INITIAL ------------------
        std::vector<sf::Sprite> playerHand, dealerHand;
        std::vector<std::string> playerNames, dealerNames;

        dealCard(playerHand, playerNames, deck, cardSprites, 600);
        dealCard(dealerHand, dealerNames, deck, cardSprites, 100);
        dealCard(playerHand, playerNames, deck, cardSprites, 600);
        dealCard(dealerHand, dealerNames, deck, cardSprites, 100);

        bool playerTurn = true;
        bool dealerTurn = false;
        bool hideDealer = true;
        bool busted = false;
        bool handOver = false;
        int pPoints = countPoints(playerNames);

        sf::RectangleShape hit({100,50}); hit.setPosition(900,650);
        sf::RectangleShape stand({100,50}); stand.setPosition(1040,650);
        hit.setFillColor(sf::Color::Blue); stand.setFillColor(sf::Color::Red);

        sf::Text hitT("HIT", font, 24); hitT.setPosition(930,660);
        sf::Text standT("STAND", font, 24); standT.setPosition(1055,660);
        sf::Text result("", font, 36); result.setPosition(500,400); result.setFillColor(sf::Color::Yellow);

        // Testi per i punteggi (Es: "16", "20")
        sf::Text playerScoreText("", font, 30); playerScoreText.setPosition(100, 550); // Sopra carte Player
        sf::Text dealerScoreText("", font, 30); dealerScoreText.setPosition(100, 50);  // Sopra carte Dealer
        playerScoreText.setFillColor(sf::Color::White);
        dealerScoreText.setFillColor(sf::Color::White);

        // Funzione per disegnare tutto
        auto drawScene = [&]() {
            window.clear(sf::Color(0, 100, 0));

            // Disegna Dealer
            for (size_t i = 0; i < dealerHand.size(); ++i) {
                if (i == 0 && hideDealer) {
                    backSprite.setPosition(dealerHand[i].getPosition());
                    window.draw(backSprite);
                } else {
                    window.draw(dealerHand[i]);
                }
            }
            // Disegna Player
            for (auto& c : playerHand) window.draw(c);

            // UI
            window.draw(hit); window.draw(stand);
            window.draw(hitT); window.draw(standT);

            betText.setString("Bet: $" + std::to_string(bet));
            balanceText.setString("Balance: $" + std::to_string(balance));
            window.draw(betText); window.draw(balanceText);

            // --- MOSTRA I PUNTEGGI ---

            // Punteggio Giocatore (Sempre visibile)
            int currentPPoints = countPoints(playerNames);
            playerScoreText.setString("Player: " + std::to_string(currentPPoints));
            window.draw(playerScoreText);

            // Punteggio Dealer (Logica nascondi carta)
            int currentDPoints = 0;
            if (hideDealer) {
                // Calcola solo le carte visibili (tutte tranne la prima)
                std::vector<std::string> visibleCards;
                for(size_t i = 1; i < dealerNames.size(); i++) {
                    visibleCards.push_back(dealerNames[i]);
                }
                currentDPoints = countPoints(visibleCards);
            } else {
                // Mostra totale vero
                currentDPoints = countPoints(dealerNames);
            }
            dealerScoreText.setString("Dealer: " + std::to_string(currentDPoints));
            window.draw(dealerScoreText);
            // ------------------------

            if (handOver) window.draw(result);
            window.display();
        };

        if (pPoints == 21) {
            playerTurn = false;
            dealerTurn = false;
            hideDealer = false;
            handOver = true;
            result.setString("BLACKJACK! Payout 3:2");
            balance += (bet * 1.5);
            hitT.setString("PLAY"); standT.setString("EXIT");
        }

        bool restartHand = false;

        while (window.isOpen() && !restartHand) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) return 0;
                if (ev.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);

                    if (!handOver) {
                        if (playerTurn && !busted) {
                            if (hit.getGlobalBounds().contains(m)) {
                                dealCard(playerHand, playerNames, deck, cardSprites, 600);
                                pPoints = countPoints(playerNames);
                                if (pPoints > 21) {
                                    busted = true;
                                    playerTurn = false;
                                    hideDealer = false;
                                    handOver = true;
                                    result.setString("BUST!");
                                    balance -= bet;
                                    hitT.setString("PLAY"); standT.setString("EXIT");
                                } else if (pPoints == 21) {
                                    playerTurn = false;
                                    dealerTurn = true;
                                    hideDealer = false;
                                }
                            }
                            if (stand.getGlobalBounds().contains(m)) {
                                playerTurn = false;
                                dealerTurn = true;
                                hideDealer = false;
                            }
                        }
                    } else {
                        if (hit.getGlobalBounds().contains(m)) restartHand = true;
                        if (stand.getGlobalBounds().contains(m)) return 0;
                    }
                }
            }

            if (dealerTurn && !handOver) {
                hideDealer = false;
                drawScene();
                std::this_thread::sleep_for(std::chrono::milliseconds(800));

                int dealerPoints = countPoints(dealerNames);
                while (dealerPoints < 17 && !deck.empty()) {
                    dealCard(dealerHand, dealerNames, deck, cardSprites, 100);
                    dealerPoints = countPoints(dealerNames);
                    drawScene();
                    std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }

                if (dealerPoints > 21) {
                    result.setString("DEALER BUST! WIN");
                    balance += bet;
                } else if (pPoints > dealerPoints) {
                    result.setString("YOU WIN");
                    balance += bet;
                } else if (pPoints < dealerPoints) {
                    result.setString("YOU LOSE");
                    balance -= bet;
                } else {
                    result.setString("TIE");
                }
                dealerTurn = false;
                handOver = true;
                hitT.setString("PLAY"); standT.setString("EXIT");
            }
            drawScene();
        }
    }
    return 0;
}