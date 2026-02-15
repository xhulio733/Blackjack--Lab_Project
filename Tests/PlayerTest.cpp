//
// Created by Xhulio on 2/15/2026.
//

#include <gtest/gtest.h>
#include "Player.h"
#include "Carta.h"

/*
    Test Fixture per la classe Player.
    Ogni TEST_F crea una nuova istanza pulita di Player.
    Questo garantisce che i test siano indipendenti tra loro.
*/
class PlayerTest : public ::testing::Test {
protected:
    Player player{1000};  // Oggetto testato con saldo iniziale 1000
};

// COSTRUTTORE
/*
    Verifica che il saldo iniziale sia corretto.
*/
TEST_F(PlayerTest, StartsWithCorrectBalance) {
    EXPECT_EQ(player.getBalance(), 1000);
}

// BETTING
/*
    Verifica che una puntata valida:
    - venga accettata
    - riduca il saldo
    - venga salvata come puntata corrente
*/
TEST_F(PlayerTest, PlaceValidBet) {
    bool result = player.placeBet(200);

    EXPECT_TRUE(result);
    EXPECT_EQ(player.getBalance(), 800);
    EXPECT_EQ(player.getBet(), 200);
}

/*
    Non deve essere possibile puntare più del saldo.
*/
TEST_F(PlayerTest, CannotBetMoreThanBalance) {
    bool result = player.placeBet(2000);

    EXPECT_FALSE(result);
    EXPECT_EQ(player.getBalance(), 1000);
    EXPECT_EQ(player.getBet(), 0);
}

/*
    Non deve essere possibile puntare 0 o negativo.
*/
TEST_F(PlayerTest, CannotBetZeroOrNegative) {
    EXPECT_FALSE(player.placeBet(0));
    EXPECT_FALSE(player.placeBet(-50));

    EXPECT_EQ(player.getBalance(), 1000);
}

// MANO E PUNTEGGIO
/*
    Verifica che hit aggiunga carte alla mano.
*/
TEST_F(PlayerTest, HitAddsCardToHand) {
    player.hit(Carta("♠", "K", 10));

    EXPECT_EQ(player.getHand().size(), 1);
}

/*
    Verifica che il punteggio venga calcolato correttamente.
*/
TEST_F(PlayerTest, CountPointsCorrectly) {
    player.hit(Carta("♠", "K", 10));
    player.hit(Carta("♥", "9", 9));

    EXPECT_EQ(player.countPoints(), 19);
}

/*
    Verifica che bust() sia true quando supera 21.
*/
TEST_F(PlayerTest, BustReturnsTrueWhenOver21) {
    player.hit(Carta("♠", "K", 10));
    player.hit(Carta("♥", "Q", 10));
    player.hit(Carta("♦", "5", 5));

    EXPECT_TRUE(player.bust());
}

/*
    Verifica che bust() sia false quando <= 21.
*/
TEST_F(PlayerTest, BustReturnsFalseWhenUnder21) {
    player.hit(Carta("♠", "K", 10));
    player.hit(Carta("♥", "9", 9));

    EXPECT_FALSE(player.bust());
}

/*
    Verifica che clearHand svuoti la mano.
*/
TEST_F(PlayerTest, ClearHandRemovesAllCards) {
    player.hit(Carta("♠", "K", 10));
    player.hit(Carta("♥", "9", 9));

    player.clearHand();

    EXPECT_EQ(player.getHand().size(), 0);
}

// GESTIONE SALDO
/*
    Verifica che deposit aumenti il saldo.
*/
TEST_F(PlayerTest, DepositIncreasesBalance) {
    player.deposit(500);

    EXPECT_EQ(player.getBalance(), 1500);
}

/*
    Verifica che withdraw riduca il saldo.
*/
TEST_F(PlayerTest, WithdrawReducesBalance) {
    player.withdraw(300);

    EXPECT_EQ(player.getBalance(), 700);
}
