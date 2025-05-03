
#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <iostream>

class Player;

class Game {
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;
    
private:
    Board myBoard;
    Player* northP;
    Player* southP;
    void captureMove(Side& s,int& hole, Board& m_board);
};

#endif 

