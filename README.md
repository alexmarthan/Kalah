# Kalah Game

A C++ implementation of the classic Kalah board game, also known as Mancala. This project provides a complete implementation of the game with support for different types of players and a flexible game board configuration.

## Game Overview

Kalah is a two-player strategy board game where players take turns sowing seeds (beans) from one of their pits into subsequent pits, including their own store (pot). The goal is to capture more seeds than your opponent.

## Features

- Configurable board size and initial bean count
- Support for multiple player types:
  - Human Player: Interactive gameplay for human users
  - Smart Player: AI player with strategic decision-making
  - Bad Player: Simple AI player for testing
- Complete game logic implementation including:
  - Bean sowing mechanics
  - Capture rules
  - Turn management
  - Win condition checking

## Project Structure

- `Board.h/cpp`: Implements the game board and core game mechanics
- `Game.h/cpp`: Manages the game state and player interactions
- `Player.h/cpp`: Defines the player interface and implementations
- `Side.h`: Defines the game sides (North/South)
- `main.cpp`: Contains test cases and game initialization

## Building and Running

To compile the project:

```bash
g++ -o kalah *.cpp
```

To run the game:

```bash
./kalah
```

## Game Rules

1. The game is played on a board with two rows of pits and a store (pot) for each player
2. Players take turns sowing beans from one of their pits
3. Beans are distributed one by one into subsequent pits, including the player's own store
4. If the last bean lands in the player's store, they get another turn
5. If the last bean lands in an empty pit on the player's side, they capture all beans in the opposite pit
6. The game ends when one player has no beans left in their pits
7. The player with the most beans in their store wins

## Testing

The project includes comprehensive test cases in `main.cpp` that verify:
- Board operations
- Player moves
- Game state management
- Win condition checking

## Implementation Details

The project uses object-oriented design principles with clear separation of concerns:
- `Board` class manages the game state and bean distribution
- `Game` class handles turn management and game flow
- `Player` class hierarchy provides different player implementations
- `Side` enum defines the two sides of the board
