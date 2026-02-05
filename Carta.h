//
// Created by Xhulio on 2/5/2026.
//

#ifndef BLACKJACK_CARTA_H
#define BLACKJACK_CARTA_H

#include <string>
#include <iostream>

class Carta {

private:
    std::string type;
    std::string value;
    int point;

public:
    Carta(const std::string& type, const std::string& value, int point);

    std::string getType() const;
    std::string getValue() const;
    int getPoint() const;
    std::string toString() const;
    void stampa() const;
};

#endif //BLACKJACK_CARTA_H