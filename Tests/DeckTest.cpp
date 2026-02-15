//
// Created by Xhulio on 2/15/2026.
//
#include <gtest/gtest.h>
#include "Deck.h"
/*
    Test Fixture per Deck.
    Ogni test crea un mazzo nuovo e pulito.
*/
class DeckTest : public ::testing::Test {
protected:
    Deck deck;
};

// TEST 1
/*
    Verifica che sia possibile pescare esattamente 52 carte
    prima che il mazzo termini.
*/
TEST_F(DeckTest, DeckHas52Cards) {
    int count = 0;
    while (!deck.endDeck()) {
        deck.nextCard();
        count++;
    }
    EXPECT_EQ(count, 52);
}

// TEST 2
/*
    Dopo aver pescato 52 carte,
    endDeck() deve diventare true.
*/
TEST_F(DeckTest, EndDeckBecomesTrueAfter52Cards) {
    for (int i = 0; i < 52; i++)
        deck.nextCard();
    EXPECT_TRUE(deck.endDeck());
}

// TEST 3
/*
    Subito dopo la creazione,
    il mazzo non deve essere terminato.
*/
TEST_F(DeckTest, EndDeckInitiallyFalse) {
    EXPECT_FALSE(deck.endDeck());
}

// TEST 4
/*
    Verifica che chiamare nextCard()
    faccia avanzare lo stato interno del mazzo.
*/
TEST_F(DeckTest, NextCardAdvancesDeck) {
    deck.nextCard();
    EXPECT_FALSE(deck.endDeck());
}

// TEST 5 - mix resetta il mazzo
/*
    Dopo aver pescato alcune carte,
    chiamare mix() deve:
    - rimescolare
    - riportare l'indice a 0
    - rendere endDeck() false
*/
TEST_F(DeckTest, MixResetsDeck) {
    for (int i = 0; i < 10; i++)
        deck.nextCard();
    deck.mix();
    EXPECT_FALSE(deck.endDeck());
}
