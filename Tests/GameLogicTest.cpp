//
// Created by Xhulio on 2/14/2026.
//

#include <gtest/gtest.h>
#include "GameLogic.h"

/*
    Questa classe è una "Test Fixture".
    Una fixture serve quando vogliamo usare lo stesso oggetto
    in più test senza riscriverlo ogni volta.
    Ogni TEST_F crea una nuova istanza pulita di GameLogic.
*/
class GameLogicTest : public ::testing::Test {
protected:
    GameLogic logic;  // Oggetto che stiamo testando
};

//  TEST 1 - Carte numeriche semplici
/*
    Verifica che le carte numeriche vengano sommate correttamente.
    7 + 8 = 15
*/
TEST_F(GameLogicTest, NumericCardsSumCorrectly) {
    std::vector<std::string> hand = {"7H", "8D"};
    EXPECT_EQ(logic.countPoints(hand), 15);
}

//  TEST 2 - Il 10 come numero
/*
    Verifica che "10" venga letto correttamente come numero 10
    e non venga interpretato male come "1".
*/
TEST_F(GameLogicTest, TenCardWorksCorrectly) {
    std::vector<std::string> hand = {"10H", "9D"};
    EXPECT_EQ(logic.countPoints(hand), 19);
}

//  TEST 3 - Figure valgono 10
/*
    J, Q, K devono valere 10 punti ciascuna.
*/
TEST_F(GameLogicTest, FaceCardsAreTen) {
    std::vector<std::string> hand = {"JH", "QD"};
    EXPECT_EQ(logic.countPoints(hand), 20);
}
TEST_F(GameLogicTest, KingIsTen) {
    std::vector<std::string> hand = {"KH"};
    EXPECT_EQ(logic.countPoints(hand), 10);
}

//  TEST 4 - Asso come 11
/*
    Se non si supera 21, l'Asso deve valere 11.
    A + 9 = 20
*/
TEST_F(GameLogicTest, SingleAceAsEleven) {
    std::vector<std::string> hand = {"AH", "9D"};
    EXPECT_EQ(logic.countPoints(hand), 20);
}

//  TEST 5 - Asso diventa 1 se si supera 21
/*
    A + 9 + 5 = 25 (sarebbe bust)
    L'asso deve trasformarsi in 1:
    1 + 9 + 5 = 15
*/
TEST_F(GameLogicTest, AceTurnsIntoOneWhenBust) {
    std::vector<std::string> hand = {"AH", "9D", "5S"};
    EXPECT_EQ(logic.countPoints(hand), 15);
}

//  TEST 6 - Più assi insieme
/*
    AH + AD + 9C
    Se contassimo entrambi come 11:
    11 + 11 + 9 = 31 (troppo)
    Uno deve diventare 1:
    11 + 1 + 9 = 21
*/
TEST_F(GameLogicTest, MultipleAcesHandledCorrectly) {
    std::vector<std::string> hand = {"AH", "AD", "9C"};
    EXPECT_EQ(logic.countPoints(hand), 21);
}

//  TEST 7 - Blackjack naturale
/*
    Asso + Figura = 21
    Deve risultare 21.
*/
TEST_F(GameLogicTest, BlackjackIsTwentyOne) {
    std::vector<std::string> hand = {"AH", "KH"};
    EXPECT_EQ(logic.countPoints(hand), 21);
}

//  TEST 8 - Bust
/*
    10 + 9 + 5 = 24
    Deve essere maggiore di 21.
    Usiamo EXPECT_GT (Greater Than).
*/
TEST_F(GameLogicTest, BustIsGreaterThanTwentyOne) {
    std::vector<std::string> hand = {"10H", "9D", "5S"};
    EXPECT_GT(logic.countPoints(hand), 21);
}

//  TEST 9 - Mano vuota
/*
    Se la mano è vuota,
    il punteggio deve essere 0.
*/
TEST_F(GameLogicTest, EmptyHandIsZero) {
    std::vector<std::string> hand = {};
    EXPECT_EQ(logic.countPoints(hand), 0);
}

//  TEST 10 - Singola carta numerica
/*
    Una singola carta numerica deve restituire il suo valore.
*/
TEST_F(GameLogicTest, SingleNumericCard) {
    std::vector<std::string> hand = {"5H"};
    EXPECT_EQ(logic.countPoints(hand), 5);
}

//  TEST 11 - Quattro assi
/*
    AH + AD + AC + AS
    Se tutti fossero 11:
    44 (impossibile)
    Devono diventare:
    11 + 1 + 1 + 1 = 14
*/
TEST_F(GameLogicTest, FourAcesHandledCorrectly) {
    std::vector<std::string> hand = {"AH", "AD", "AC", "AS"};
    EXPECT_EQ(logic.countPoints(hand), 14);
}

//  TEST 12 - Stringa non valida
/*
    Se la stringa non contiene numeri o figure,
    deve valere 0 e non crashare.
*/
TEST_F(GameLogicTest, InvalidCardDoesNotCrash) {
    std::vector<std::string> hand = {"XYZ"};
    EXPECT_EQ(logic.countPoints(hand), 0);
}
