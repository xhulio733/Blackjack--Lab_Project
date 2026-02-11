#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

// ------------------ HELPER POINTS ------------------
int countPoints(const std::vector<std::string>& hand) {
    int sum = 0;
    int aceCount = 0;

    for (const auto& name : hand) {
        int val = 0;
        if (name.find('J') != std::string::npos) val = 10;
        else if (name.find('Q') != std::string::npos) val = 10;
        else if (name.find('K') != std::string::npos) val = 10;
        else if (name.find('A') != std::string::npos && name.find("10") == std::string::npos) {
            val = 11; aceCount++;
        }
        else {
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
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Blackjack Pro");
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
    int balance = 1000;
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

        // --- DEFINIZIONE BOTTONI PUNTATA ---
        // Ho spostato le coordinate X per farci stare il 50
        sf::RectangleShape bet5({80,50});  bet5.setPosition(50,650);
        sf::RectangleShape bet10({80,50}); bet10.setPosition(150,650);
        sf::RectangleShape bet25({80,50}); bet25.setPosition(250,650);
        sf::RectangleShape bet50({80,50}); bet50.setPosition(350,650); // NUOVO

        // Pulsante RESET e START
        sf::RectangleShape resetBtn({100, 50}); resetBtn.setPosition(480, 650);
        sf::RectangleShape startBtn({150,60});  startBtn.setPosition(650,645);

        // Colori
        bet5.setFillColor(sf::Color::White);
        bet10.setFillColor(sf::Color::White);
        bet25.setFillColor(sf::Color::White);
        bet50.setFillColor(sf::Color::Cyan); // Ciano per il 50 per distinguerlo
        resetBtn.setFillColor(sf::Color(255, 140, 0)); // Arancione
        startBtn.setFillColor(sf::Color::Magenta);

        // Testi Pulsanti
        sf::Text t5("5", font, 24);   t5.setPosition(80,660);   t5.setFillColor(sf::Color::Black);
        sf::Text t10("10", font, 24); t10.setPosition(175,660); t10.setFillColor(sf::Color::Black);
        sf::Text t25("25", font, 24); t25.setPosition(275,660); t25.setFillColor(sf::Color::Black);
        sf::Text t50("50", font, 24); t50.setPosition(375,660); t50.setFillColor(sf::Color::Black); // NUOVO

        sf::Text tReset("RESET", font, 20); tReset.setPosition(495, 662); tReset.setFillColor(sf::Color::Black);
        sf::Text tStart("DEAL", font, 24);  tStart.setPosition(690,660);  tStart.setFillColor(sf::Color::Black);

        sf::Text betText("Bet: $0", font, 24); betText.setPosition(950,250);
        sf::Text balanceText("Balance: $" + std::to_string(balance), font, 24); balanceText.setPosition(950,200);

        while (window.isOpen() && !betSelected) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) return 0;
                if (ev.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);

                    // Aggiungi Fiches
                    if (bet5.getGlobalBounds().contains(m) && balance >= bet + 5) bet += 5;
                    if (bet10.getGlobalBounds().contains(m) && balance >= bet + 10) bet += 10;
                    if (bet25.getGlobalBounds().contains(m) && balance >= bet + 25) bet += 25;
                    if (bet50.getGlobalBounds().contains(m) && balance >= bet + 50) bet += 50; // NUOVO

                    // RESET
                    if (resetBtn.getGlobalBounds().contains(m)) bet = 0;

                    // START
                    if (startBtn.getGlobalBounds().contains(m) && bet > 0) {
                        betSelected = true;
                        balance -= bet; // Soldi sul tavolo
                    }

                    betText.setString("Bet: $" + std::to_string(bet));
                }
            }
            window.clear(sf::Color(0, 100, 0));
            // Disegna tutto
            window.draw(bet5); window.draw(bet10); window.draw(bet25); window.draw(bet50);
            window.draw(startBtn); window.draw(resetBtn);

            window.draw(t5); window.draw(t10); window.draw(t25); window.draw(t50);
            window.draw(tStart); window.draw(tReset);

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
        bool handOver = false;
        int pPoints = countPoints(playerNames);

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

        auto drawScene = [&]() {
            window.clear(sf::Color(0, 100, 0));
            for (size_t i = 0; i < dealerHand.size(); ++i) {
                if (i == 0 && hideDealer) {
                    backSprite.setPosition(dealerHand[i].getPosition());
                    window.draw(backSprite);
                } else {
                    window.draw(dealerHand[i]);
                }
            }
            for (auto& c : playerHand) window.draw(c);

            window.draw(hit); window.draw(stand);
            window.draw(hitT); window.draw(standT);

            if (playerTurn && playerHand.size() == 2 && balance >= bet) {
                window.draw(dbl);
                window.draw(dblT);
            }

            betText.setString("Bet: $" + std::to_string(bet));
            balanceText.setString("Balance: $" + std::to_string(balance));
            window.draw(betText); window.draw(balanceText);

            pScore.setString("Player: " + std::to_string(countPoints(playerNames)));
            window.draw(pScore);

            int dPointsVis = 0;
            if (hideDealer) {
                 std::vector<std::string> temp;
                 for(size_t i=1; i<dealerNames.size(); i++) temp.push_back(dealerNames[i]);
                 dPointsVis = countPoints(temp);
            } else {
                 dPointsVis = countPoints(dealerNames);
            }
            dScore.setString("Dealer: " + std::to_string(dPointsVis));
            window.draw(dScore);

            if (handOver) window.draw(result);
            window.display();
        };

        if (pPoints == 21) {
            playerTurn = false; dealerTurn = false; hideDealer = false; handOver = true;
            result.setString("BLACKJACK! Payout 3:2");
            balance += (bet + (bet * 1.5));
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
                        if (playerTurn) {
                            // HIT
                            if (hit.getGlobalBounds().contains(m)) {
                                dealCard(playerHand, playerNames, deck, cardSprites, 600);
                                pPoints = countPoints(playerNames);
                                if (pPoints > 21) {
                                    playerTurn = false; handOver = true; hideDealer = false;
                                    result.setString("BUST!");
                                    hitT.setString("PLAY"); standT.setString("EXIT");
                                } else if (pPoints == 21) {
                                    playerTurn = false; dealerTurn = true; hideDealer = false;
                                }
                            }
                            // STAND
                            if (stand.getGlobalBounds().contains(m)) {
                                playerTurn = false; dealerTurn = true; hideDealer = false;
                            }
                            // DOUBLE
                            if (playerHand.size() == 2 && balance >= bet && dbl.getGlobalBounds().contains(m)) {
                                balance -= bet;
                                bet *= 2;
                                dealCard(playerHand, playerNames, deck, cardSprites, 600);
                                pPoints = countPoints(playerNames);

                                if (pPoints > 21) {
                                    playerTurn = false; handOver = true; hideDealer = false;
                                    result.setString("BUST (DOUBLE)!");
                                    hitT.setString("PLAY"); standT.setString("EXIT");
                                } else {
                                    playerTurn = false; dealerTurn = true; hideDealer = false;
                                }
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
                    drawScene(); std::this_thread::sleep_for(std::chrono::milliseconds(800));
                }

                if (dealerPoints > 21) { result.setString("DEALER BUST! YOU WIN"); balance += (bet * 2); }
                else if (pPoints > dealerPoints) { result.setString("YOU WIN"); balance += (bet * 2); }
                else if (pPoints < dealerPoints) { result.setString("YOU LOSE"); }
                else { result.setString("PUSH (TIE)"); balance += bet; }

                dealerTurn = false; handOver = true;
                hitT.setString("PLAY"); standT.setString("EXIT");
            }
            drawScene();
        }
    }
    return 0;
}