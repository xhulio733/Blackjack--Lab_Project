//
// Created by Xhulio on 2/15/2026.
//
#include <gtest/gtest.h>
#include "Dealer.h"
#include "Deck.h"

/*
    Test Fixture per Dealer.
    Ogni test crea un Dealer e un Deck nuovi.
*/
class DealerTest : public ::testing::Test {
protected:
    Dealer dealer;
    Deck deck;
    void SetUp() override {
        deck.mix();  // Mischia prima di ogni test
    }
};

// TEST 1
/*
    Verifica che il dealer continui a pescare
    finché il punteggio è inferiore a 17.
*/
TEST_F(DealerTest, DealerStopsAt17OrMore) {
    dealer.playTurn(deck);
    EXPECT_GE(dealer.countPoints(), 17);
}

// TEST 2
/*
    Verifica che, se supera 21,
    bust() restituisca true.
*/
TEST_F(DealerTest, DealerCanBust) {
    dealer.playTurn(deck);
    if (dealer.countPoints() > 21) {
        EXPECT_TRUE(dealer.bust());
    } else {
        EXPECT_FALSE(dealer.bust());
    }
}

// TEST 3
/*
    Se il dealer ha già 17 o più,
    non dovrebbe pescare ulteriormente.
*/
TEST_F(DealerTest, DealerDoesNotDrawIfAlready17OrMore) {
    // Forziamo una mano iniziale forte
    dealer.hit(Carta("♠", "K", 10));
    dealer.hit(Carta("♥", "7", 7));
    int before = dealer.countPoints();
    dealer.playTurn(deck);
    int after = dealer.countPoints();
    EXPECT_EQ(before, after);
}
