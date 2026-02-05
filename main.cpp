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

// Count points with proper ace handling
int countPoints(const std::vector<std::string>& hand, const std::unordered_map<std::string,int>& cardValues) {
    int sum = 0;
    int aceCount = 0;
    for (const auto& cardName : hand) {
        std::string value = cardName.substr(0, cardName.size() - 1); // remove suit
        auto it = cardValues.find(value);
        if (it != cardValues.end()) {
            sum += it->second;
            if (value == "A") aceCount++;
        }
    }
    while (sum > 21 && aceCount > 0) {
        sum -= 10;
        aceCount--;
    }
    return sum;
}

int main() {
    // --- MENU INIZIALE ---
    sf::RenderWindow menuWindow(sf::VideoMode({800, 800}), "Blackjack Menu");

    sf::RectangleShape startButton(sf::Vector2f(200.f, 80.f));
    startButton.setFillColor(sf::Color::Green);
    startButton.setPosition(sf::Vector2f(500.f, 250.f));

    sf::RectangleShape exitButton(sf::Vector2f(200.f, 80.f));
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setPosition(sf::Vector2f(250.f, 250.f));

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Errore caricamento font\n";
        return 1;
    }

     // Text for menu buttons
    sf::Text startText("Start", font, 32);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(sf::Vector2f(startButton.getPosition().x + 50.f, startButton.getPosition().y + 20.f));

    sf::Text exitText("Exit", font,32);
    exitText.setFillColor(sf::Color::Black);
    exitText.setPosition(sf::Vector2f(exitButton.getPosition().x + 60.f, exitButton.getPosition().y + 20.f));

    bool startGame = false;

    while (menuWindow.isOpen() && !startGame) {
        sf::Event ev;
        while (menuWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                menuWindow.close();
                exit(0);
            } else if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                                      static_cast<float>(ev.mouseButton.y));
                if (startButton.getGlobalBounds().contains(mousePos))
                    startGame = true;
                else if (exitButton.getGlobalBounds().contains(mousePos)) {
                    menuWindow.close();
                    exit(0);
                }
            }
        }

        menuWindow.clear(sf::Color::Cyan);
        menuWindow.draw(startButton);
        menuWindow.draw(exitButton);
        menuWindow.draw(startText);
        menuWindow.draw(exitText);
        menuWindow.display();
    }

    // --- FINESTRA GIOCO ---
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Blackjack Table");

    std::string cardsFolder = "C:/Users/Xhulio/CLionProjects/BlackJackEsame/Cards/PNG/Cards (large)/";

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Sprite> cardMap;

    // Caricamento carte
    for(const auto& entry : fs::directory_iterator(cardsFolder)){
        if(!entry.is_regular_file() || entry.path().extension()!=".png") continue;
        std::string name = entry.path().stem().string();
        sf::Texture tex;
        if(!tex.loadFromFile(entry.path().string())) continue;
        textures[name] = tex;
        cardMap.emplace(name, sf::Sprite(textures.at(name))); // <-- fixed here
    }


    // Creazione mazzo
    std::vector<std::string> deck;
    for(const auto& p : cardMap) deck.push_back(p.first);
    std::shuffle(deck.begin(), deck.end(), std::mt19937(std::random_device{}()));

    // --- SALDO E PUNTATA ---
    int balance = 100;
    int bet = 5;
    bool startHand = false;

    sf::RectangleShape bet5(sf::Vector2f(100.f, 50.f)); bet5.setFillColor(sf::Color::White); bet5.setPosition(sf::Vector2f(100.f, 650.f));
    sf::RectangleShape bet10(sf::Vector2f(100.f, 50.f)); bet10.setFillColor(sf::Color::White); bet10.setPosition(sf::Vector2f(250.f, 650.f));
    sf::RectangleShape bet100(sf::Vector2f(100.f, 50.f)); bet100.setFillColor(sf::Color::White); bet100.setPosition(sf::Vector2f(400.f, 650.f));
    sf::RectangleShape startHandButton(sf::Vector2f(150.f, 60.f)); startHandButton.setFillColor(sf::Color::Magenta); startHandButton.setPosition(sf::Vector2f(600.f, 650.f));

    // Texts for bet buttons
    sf::Text bet5Text("5", font , 24); bet5Text.setFillColor(sf::Color::Black); bet5Text.setPosition(sf::Vector2f(bet5.getPosition().x + 35.f, bet5.getPosition().y + 10.f));
    sf::Text bet10Text("10", font , 24); bet10Text.setFillColor(sf::Color::Black); bet10Text.setPosition(sf::Vector2f(bet10.getPosition().x + 30.f, bet10.getPosition().y + 10.f));
    sf::Text bet100Text("25", font , 24); bet100Text.setFillColor(sf::Color::Black); bet100Text.setPosition(sf::Vector2f(bet100.getPosition().x + 20.f, bet100.getPosition().y + 10.f));
    sf::Text startHandText("50", font , 24); startHandText.setFillColor(sf::Color::Black); startHandText.setPosition(sf::Vector2f(startHandButton.getPosition().x + 35.f, startHandButton.getPosition().y + 15.f));

    sf::Text betCurrentText("Bet: $0",font , 24); betCurrentText.setFillColor(sf::Color::Yellow); betCurrentText.setPosition(sf::Vector2f(950.f, 250.f));

    // --- SCELTA PUNTATA ---
    while (window.isOpen() && !startHand) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            else if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                                      static_cast<float>(ev.mouseButton.y));

                if (bet5.getGlobalBounds().contains(mousePos)) bet = 5;
                else if (bet10.getGlobalBounds().contains(mousePos)) bet = 10;
                else if (bet100.getGlobalBounds().contains(mousePos)) bet = 100;
                else if (startHandButton.getGlobalBounds().contains(mousePos)) startHand = true;

                betCurrentText.setString("Bet: $" + std::to_string(bet));
            }
        }

        window.clear(sf::Color::Cyan);
        window.draw(bet5); window.draw(bet5Text);
        window.draw(bet10); window.draw(bet10Text);
        window.draw(bet100); window.draw(bet100Text);
        window.draw(startHandButton); window.draw(startHandText);
        window.draw(betCurrentText);
        window.display();
    }


    // --- INITIAL HANDS ---
    std::vector<sf::Sprite> playerHand, dealerHand;
    std::vector<std::string> playerHandNames, dealerHandNames;

    auto dealCard = [&](std::vector<sf::Sprite>& hand, std::vector<std::string>& names, float y){
        std::string cardName = deck.back(); deck.pop_back();
        sf::Sprite card = cardMap.at(cardName);
        card.setPosition(sf::Vector2f(100.f + hand.size()*150.f, y));
        hand.push_back(card);
        names.push_back(cardName);
    };

    dealCard(playerHand, playerHandNames, 600.f);
    dealCard(dealerHand, dealerHandNames, 100.f);
    dealCard(playerHand, playerHandNames, 600.f);
    dealCard(dealerHand, dealerHandNames, 100.f);

    sf::RectangleShape hitButton({100.f,50.f}); hitButton.setFillColor(sf::Color::Blue); hitButton.setPosition(sf::Vector2f(900.f,650.f));
    sf::Text hitText("HIT",font , 24); hitText.setFillColor(sf::Color::Black); hitText.setPosition(sf::Vector2f(hitButton.getPosition().x+25.f, hitButton.getPosition().y+10.f));

    sf::RectangleShape standButton({100.f,50.f}); standButton.setFillColor(sf::Color::Red); standButton.setPosition(sf::Vector2f(1040.f,650.f));
    sf::Text standText("STAND",font , 24); standText.setFillColor(sf::Color::Black); standText.setPosition(sf::Vector2f(standButton.getPosition().x+15.f, standButton.getPosition().y+10.f));

    sf::Text scoreText("",font , 24); scoreText.setFillColor(sf::Color::White); scoreText.setPosition(sf::Vector2f(950.f,150.f));
    sf::Text balanceText("",font , 24); balanceText.setFillColor(sf::Color::Yellow); balanceText.setPosition(sf::Vector2f(950.f,200.f));
    sf::Text resultText("", font , 36); resultText.setFillColor(sf::Color::Red); resultText.setPosition(sf::Vector2f(500.f,400.f));

    std::unordered_map<std::string,int> cardValues = {{"A",11},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8},{"9",9},{"10",10},{"J",10},{"Q",10},{"K",10}};

    bool playerTurn=true, playerBusted=false;
    bool dealerPlaying=false;
    bool dealerCardHidden = true;
    int playerPoints = countPoints(playerHandNames,cardValues);
    int dealerPoints = countPoints(dealerHandNames,cardValues);

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
            }
            else if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(static_cast<float>(ev.mouseButton.x),
                                      static_cast<float>(ev.mouseButton.y));

                // --- HIT ---
                if (playerTurn && !playerBusted) {
                    if(hitButton.getGlobalBounds().contains(mousePos) && !deck.empty()) {
                        dealCard(playerHand, playerHandNames, 600.f);
                        playerPoints = countPoints(playerHandNames, cardValues);

                        if(playerPoints > 21) {
                            playerBusted = true;
                            playerTurn = false;
                            dealerPlaying = true;
                            resultText.setString("BUST!");
                            balance -= bet;
                            dealerCardHidden = false;
                        }
                        else if(playerPoints == 21) {
                            playerTurn = false;
                            dealerPlaying = true;
                            dealerCardHidden = false;
                        }
                    }
                    else if(standButton.getGlobalBounds().contains(mousePos)) {
                        playerTurn = false;
                        dealerPlaying = true;
                        dealerCardHidden = false;
                    }
                }
            } // end MouseButtonPressed
        } // end pollEvent loop
    } // end window.isOpen()


    // --- Dealer Turn ---
    if(dealerPlaying && !playerBusted) {
        dealerPoints = countPoints(dealerHandNames, cardValues);
        if(dealerPoints < 17 && !deck.empty()) {
            dealCard(dealerHand, dealerHandNames, 100.f);
        } else {
            playerPoints = countPoints(playerHandNames, cardValues);
            dealerPoints = countPoints(dealerHandNames, cardValues);
            if(dealerPoints > 21 || playerPoints > dealerPoints) {
                resultText.setString("YOU WIN!");
                balance += bet;
            }
            else if(playerPoints < dealerPoints) {
                resultText.setString("YOU LOSE!");
                balance -= bet;
            }
            else {
                resultText.setString("TIE!");
            }
            dealerPlaying = false;
        }
    }

    // --- Draw everything ---
    scoreText.setString("Player Points: " + std::to_string(playerPoints));
    balanceText.setString("Balance: $" + std::to_string(balance));
    betCurrentText.setString("Bet: $" + std::to_string(bet));

    window.clear(sf::Color::Green);

    for(size_t i = 0; i < dealerHand.size(); ++i) {
        if(i == 0 && dealerCardHidden) {
            sf::RectangleShape hidden({100.f, 150.f});
            hidden.setFillColor(sf::Color::Blue);
            hidden.setPosition(dealerHand[i].getPosition());
            window.draw(hidden);
        } else {
            window.draw(dealerHand[i]);
        }
    }

    for(auto& c : playerHand) window.draw(c);

    if(playerTurn && !playerBusted) {
        window.draw(hitButton); window.draw(hitText);
        window.draw(standButton); window.draw(standText);
    }

    window.draw(scoreText); window.draw(balanceText); window.draw(betCurrentText);
    if(playerBusted || !resultText.getString().isEmpty()) window.draw(resultText);

    window.display();

    return 0;
}

