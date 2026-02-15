# Blackjack--Lab_Project
Project Overview

Project Structure
- Carta -> Represents a single card
- Deck -> Manages the 52-card deck
- Player -> Handles player state, balance, bets, and hand
- Dealer -> Inherits behavior from Player and implements dealer rules
- GameLogic -> Handles Blackjack scoring logic

Graphical Interface :
- GameGraphics -> Manages rendering, buttons, user input, and game flow
- Built using **SFML**

Game Features
- Full 52-card deck
- Card shuffling
- Player betting system
- Hit / Stand / Double actions
- Dealer automatic play (stands at 17)
- Blackjack 3:2 payout
- Bust detection
- Push (tie) handling
- Balance tracking
- Graphical interface with clickable buttons

Unit Testing

Implemented using **Google Test**
- Tests cover:
 
  - Card logic
  - Deck behavior
  - Player mechanics
  - Dealer rules
  - Blackjack scoring logic

Game Rules (Implemented Version):

The goal is to obtain a hand value as close as possible to 21 without exceeding it, and to beat the Dealer’s hand.

  Card Values :
  
    Number cards (2–10) -> Face value
    Jack (J), Queen (Q), King (K) -> 10 points
    Ace (A) -> 11 points by default
    If total exceeds 21, Ace automatically becomes 1

Betting System :

  The player starts with a balance of 1000.
  Available betting buttons: 5, 10, 25, 50.
  The selected bet is deducted from the balance when the round starts.
  Balance updates automatically after each round.

Player Actions :

  HIT -> Draw another card.
  STAND -> End turn and let the Dealer play.
  
  DOUBLE (DBL) : 
  
    Doubles the bet. 
    Only available with exactly 2 cards.
    Draws one final card.
    Automatically ends the player’s turn.

Dealer Rules :

  The Dealer receives two cards (one hidden initially).
  The Dealer must draw cards until reaching 17 or more.
  The hidden card is revealed when the player's turn ends.

Winning Conditions :

  Blackjack (A + 10-value card) -> Pays 3:2
  Player bust (>21) -> Player loses the bet.
  Dealer bust (>21) -> Player wins (payout 2:1)
  Higher score than Dealer -> Player wins (payout 2:1)
  Lower score than Dealer -> Player loses.
  Equal score (Push) -> Bet is returned.
  
End of Round

After the result:

Player can choose PLAY to start a new hand.
Or EXIT to close the game.



