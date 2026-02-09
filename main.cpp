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

namespace fs = std::filesystem;

// ------------------ COUNT POINTS ------------------
int countPoints(const std::vector<std::string>& hand) {
    int sum = 0;
    int aceCount = 0;

    for (const auto& c : hand) {
        // Prendi la parte dopo l'ultimo '_' -> "A", "2", ..., "J", "Q", "K"
        size_t pos = c.rfind('_');
        if (pos == std::string::npos) continue; // sicurezza
        std::string value = c.substr(pos + 1);

        int val = 0;
        if (value == "A") {
            val = 11;
            aceCount++;
        }
        else if (value == "J" || value == "Q" || value == "K") val = 10;
        else {
            try {
                val = std::stoi(value);
            } catch (...) {
                val = 0; // sicurezza, non crasha
            }
        }

        sum += val;
    }

    // Riduci gli assi se bust
    while (sum > 21 && aceCount > 0) {
        sum -= 10;
        aceCount--;
    }

    return sum;
}

// ------------------ MAIN ------------------
int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "Blackjack");

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Font error\n";
        return 1;
    }

    // ------------------ LOAD CARDS ------------------
    std::string cardsFolder =
    "C:/Users/Xhulio/CLionProjects/Blackjack1/Cards/PNG/Cards (large)/";

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Sprite> cardSprites;

    // Carica tutte le carte tranne card_back
    for (const auto& e : fs::directory_iterator(cardsFolder)) {
        if (e.path().extension() == ".png") {
            std::string name = e.path().stem().string();
            if (name == "card_back") continue; // salta card_back dal mazzo
            textures[name].loadFromFile(e.path().string());
            cardSprites[name] = sf::Sprite(textures[name]);
        }
    }

    // Carica la card_back separatamente
    sf::Texture backTexture;
    if (!backTexture.loadFromFile(cardsFolder + "card_back.png")) {
        std::cerr << "Errore caricamento card_back\n";
        return 1;
    }
    sf::Sprite backSprite(backTexture);

    // Crea il mazzo solo con le carte giocabili
    std::vector<std::string> deck;
    for (auto& c : cardSprites) deck.push_back(c.first);
    std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device{}()));

    // Funzione per dare una carta
    auto dealCard = [&](std::vector<sf::Sprite>& hand,
                        std::vector<std::string>& names,
                        float y) {
        if (deck.empty()) return;
        std::string c = deck.back();
        deck.pop_back();

        sf::Sprite s = cardSprites[c];
        s.setPosition(100 + hand.size() * 140, y);
        hand.push_back(s);
        names.push_back(c);
    };


    // ------------------ BET SELECTION ------------------
    int balance = 100;
    int bet = 5;
    bool startHand = false;

    sf::RectangleShape bet5({100, 50}); bet5.setPosition(100, 650);
    sf::RectangleShape bet10({100, 50}); bet10.setPosition(250, 650);
    sf::RectangleShape bet25({100, 50}); bet25.setPosition(400, 650);
    sf::RectangleShape startBtn({150, 60}); startBtn.setPosition(600, 650);

    bet5.setFillColor(sf::Color::White);
    bet10.setFillColor(sf::Color::White);
    bet25.setFillColor(sf::Color::White);
    startBtn.setFillColor(sf::Color::Magenta);

    sf::Text t5("5", font, 24); t5.setPosition(140, 660);
    sf::Text t10("10", font, 24); t10.setPosition(280, 660);
    sf::Text t25("25", font, 24); t25.setPosition(430, 660);
    sf::Text tStart("START", font, 24); tStart.setPosition(635, 665);

    t5.setFillColor(sf::Color::Black);
    t10.setFillColor(sf::Color::Black);
    t25.setFillColor(sf::Color::Black);
    tStart.setFillColor(sf::Color::Black);

    sf::Text betText("Bet: $5", font, 24);
    betText.setPosition(950, 250);

    while (window.isOpen() && !startHand) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                return 0;

            if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                if (bet5.getGlobalBounds().contains(m)) bet = 5;
                if (bet10.getGlobalBounds().contains(m)) bet = 10;
                if (bet25.getGlobalBounds().contains(m)) bet = 25;
                if (startBtn.getGlobalBounds().contains(m)) startHand = true;
                betText.setString("Bet: $" + std::to_string(bet));
            }
        }

        window.clear(sf::Color::Cyan);
        window.draw(bet5); window.draw(bet10); window.draw(bet25);
        window.draw(startBtn);
        window.draw(t5); window.draw(t10); window.draw(t25); window.draw(tStart);
        window.draw(betText);
        window.display();
    }

    // ------------------ GAME STATE ------------------
    std::vector<sf::Sprite> playerHand, dealerHand;
    std::vector<std::string> playerNames, dealerNames;

    dealCard(playerHand, playerNames, 600);
    dealCard(dealerHand, dealerNames, 100);
    dealCard(playerHand, playerNames, 600);
    dealCard(dealerHand, dealerNames, 100);

    bool playerTurn = true;
    bool dealerTurn = false;
    bool hideDealer = true;
    bool busted = false;

    int pPoints = countPoints(playerNames);

    // ------------------ BUTTONS ------------------
    sf::RectangleShape hit({100, 50}); hit.setPosition(900, 650);
    sf::RectangleShape stand({100, 50}); stand.setPosition(1040, 650);

    hit.setFillColor(sf::Color::Blue);
    stand.setFillColor(sf::Color::Red);

    sf::Text hitT("HIT", font, 24); hitT.setPosition(930, 660);
    sf::Text standT("STAND", font, 24); standT.setPosition(1055, 660);

    sf::Text result("", font, 36);
    result.setPosition(500, 400);

    // ------------------ GAME LOOP ------------------
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                return 0;

            if (ev.type == sf::Event::MouseButtonPressed && playerTurn && !busted) {
                sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);

                if (hit.getGlobalBounds().contains(m)) {
                    dealCard(playerHand, playerNames, 600);
                    pPoints = countPoints(playerNames);

                    if (pPoints > 21) {
                        busted = true;
                        playerTurn = false;
                        dealerTurn = true;
                        hideDealer = false;
                        result.setString("BUST!");
                        balance -= bet;
                    }
                }

                if (stand.getGlobalBounds().contains(m)) {
                    playerTurn = false;
                    dealerTurn = true;
                    hideDealer = false;
                }
            }
        }

        if (dealerTurn && !busted) {

            while (countPoints(dealerNames) < 17) {
                dealCard(dealerHand, dealerNames, 100);
            }

            int d = countPoints(dealerNames);

            if (d > 21 || pPoints > d) {
                result.setString("YOU WIN");
                balance += bet;
            }
            else if (pPoints < d) {
                result.setString("YOU LOSE");
                balance -= bet;
            }
            else {
                result.setString("TIE");
            }

            dealerTurn = false;
        }


        window.clear(sf::Color::Green);

        for (size_t i = 0; i < dealerHand.size(); ++i) {
            if (i == 0 && hideDealer) {
                backSprite.setPosition(dealerHand[i].getPosition());
                window.draw(backSprite);
            } else {
                window.draw(dealerHand[i]);
            }


            for (auto& c : playerHand) window.draw(c);

            if (playerTurn) {
                window.draw(hit); window.draw(hitT);
                window.draw(stand); window.draw(standT);
            }

            window.draw(betText);
            window.draw(result);
            window.display();
        }

        return 0;
    }
}



