
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Side.h"
#include "Board.h"
#include <iostream>

class Timer;

class Player {
    
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();

private:
    string m_name;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player {
public:
    BadPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
};


class SmartPlayer: public Player {
public:
    SmartPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;

private:
    void miniMax(const Board& b, Side s, int & bestHole, int& value, int depth, JumpyTimer& myTime) const;
    int evaluate(const Board& b) const;
    bool moveSimulator(Side s, int hole, Board &b, Side& endSide, int& endHole) const;
};

#endif

