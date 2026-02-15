//
// Created by Xhulio on 2/5/2026.
//

#include "Carta.h"

Carta::Carta(const std::string &type, const std::string &value, int point)
    : type(type), value(value), point(point) {}

std::string Carta::getType() const {
    return type;
}

std::string Carta::getValue() const {
    return value;
}

int Carta::getPoint() const {
    return point;
}

std::string Carta::toString() const {
    return value + " di " + type;
}

void Carta::stampa() const {
    std::cout << value << type << std::endl;
}