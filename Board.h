
#ifndef BOARD_H
#define BOARD_H

#include "Side.h"
#include <vector>

using namespace std;

class Board {
    
public:
    Board(int nHoles, int nInitialBeansPerHole);
    
    int holes() const;

    int beans(Side s, int hole) const;

    int beansInPlay(Side s) const;

    int totalBeans() const;

    bool sow(Side s, int hole, Side& endSide, int& endHole);

    bool moveToPot(Side s, int hole, Side potOwner);

    bool setBeans(Side s, int hole, int beans);

private:
    vector<int> m_north;
    vector<int> m_south;
    int num_holes; //number of holes in each side
    int m_totalBeans;
    bool isValidHole(int hole) const; // checks if the hole is valid (includes pot)
    void incrementPot(Side s, int value); // adds value to the side's pot
    void South4South(int& currentH);
    void North4North(int& currentH);
    void North4South(int& currentH, Side& currentS);
    void South4North(int& currentH, Side& currentS);
    void SwitchNorthToSouth(int& hole, Side& side);
    void SwitchSouthtoNorth(int& hole, Side& side);

    
};
#endif 
