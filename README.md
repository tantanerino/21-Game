# Blackjack Game (21-Game)

Blackjack game written in C++. Supports multiplayer (Locally), AI opponents, and tracks scores across multiple rounds.

---

## Setup Instructions

### Requirements
- A C++ compiler (e.g., `g++`)  
- Works on macOS, Linux, and Windows

### How to Compile

#### On macOS/Linux:
g++ src/21-Game.cpp -o blackjack
./blackjack

#### On Windows

g++ src/21-Game.cpp -o blackjack.exe
blackjack.exe

### How to Play

Enter number of players – Must be an integer
1. Enter names for each player
2. Enter number of AI opponents – Also an integer
3. Game begins

Each player can choose to:
- Press h → Hit (take another card)
- Press s → Stay (end their turn)

Once all players and AIs have played, the results for the round are shown.

After each round:
- Press y → Play again
- Press any other key → Exit the game

At the end, a final scoreboard is shown with player performance.
