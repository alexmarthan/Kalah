
#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole) {
    if (nHoles < 1) {
        nHoles = 1;
    }

    if (nInitialBeansPerHole < 0) {
        nInitialBeansPerHole = 0;
    }

    num_holes = nHoles;

    m_totalBeans = (num_holes) * nInitialBeansPerHole * 2; // nb of holes multiplied by 2 for each side (multiplied by beansperhole)

    //Initializing both pots with 0 beans

    m_north.push_back(0);
    m_south.push_back(0);

    for (int i = 1; i <= num_holes; i++) { // initializing the rest of the holes
        m_north.push_back(nInitialBeansPerHole);
        m_south.push_back(nInitialBeansPerHole);
    }
}

int Board::holes() const {
    return num_holes;
}

int Board::beans(Side s, int hole) const {

    // check if out of bounds
    if (!isValidHole(hole)) {
        return -1;
    }
    if (s == SOUTH) { //south
        return m_south[hole];
    }
    else { //north
        return m_north[hole];
    }
}

int Board::beansInPlay(Side s) const {

    int counter(0);
    if (s == NORTH){
        for (int i = 1; i <= num_holes; i++){ // starts at 1 to not add the beans in the pot
            counter+= m_north[i]; // keep adding the nb of beans in each hole to the counter
        }
    }
    else {
        for (int i = 1; i <= num_holes; i++){ // only counts beans that aren't in pot
            counter+= m_south[i];
        }
    }
    return counter;
}

int Board::totalBeans() const {
    return m_totalBeans;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {

    if (!isValidHole(hole) || hole == POT) {
        // Check if the hole is valid (within the range of holes) and not the pot
        // Return false if the hole is invalid
        return false;
    }

    if (s == SOUTH && m_south[hole] == 0) {
        // Check if the chosen hole on the SOUTH side has no beans
        // Return false as it is an invalid move
        return false;
    }

    if (s == NORTH && m_north[hole] == 0) {
        // Check if the chosen hole on the NORTH side has no beans
        // Return false as it is an invalid move
        return false;
    }

    int currentH(hole);
    Side currentS(s);

    if (s == SOUTH) {
        int beansToDistribute = m_south[hole]; // Number of beans to distribute
        m_south[hole] = 0; // Clear the current hole on the SOUTH side

        while (beansToDistribute > 0) {

            SwitchSouthtoNorth(currentH, currentS);

            if (currentS == SOUTH) {
                South4South(currentH); //iterating though south when you are south player
            }
            else {
                North4South(currentH, currentS); //iterating through south when you are north player
            }
            beansToDistribute--;
        }
    }
    else { // Playing on the NORTH side
        int beansToDistribute = m_north[hole];
        m_north[hole] = 0; // Clear the current hole on the NORTH side

        while (beansToDistribute > 0) {

            SwitchNorthToSouth(currentH, currentS);

            if (currentS == SOUTH) {
                South4North(currentH, currentS); //iterating though south when you are north player
            }
            else {
                North4North(currentH); //iterating through north when you are north player
            }
            beansToDistribute--;
        }
    }
    
    endSide = currentS;

    if (currentH == num_holes + 1) {
        // If the current hole is the pot, set the current hole index to 0 (representing the pot)
        currentH = 0;
    }

    // Setting the parameter variables for the end hole and end side
    endHole = currentH;

    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner) {

    if (!isValidHole(hole) || (s != NORTH && s != SOUTH)) {
        return false;
    }
    if (hole == 0) {
        return false;
    }

    int numBeans = beans(s,hole); // stores nb of beans in that hole

    // set the nb of beans in the hole to zero
    if (s == NORTH){m_north[hole] = 0;}
    if (s == SOUTH){m_south[hole] = 0;}

    incrementPot(potOwner, numBeans); // adds beans to the pot

    return true;
}

bool Board::setBeans(Side s, int hole, int beans) { // set nb of beans in a hole to a nb, adjust m_totalBeans
    if (!isValidHole(hole) || (s != NORTH && s != SOUTH)) {
        return false;
    }
    if (beans < 0) {
        return false;
    }
    int difference(0);

    if (s == NORTH){
        difference = beans - m_north[hole] ;
        m_north[hole] = beans;
    }

    if (s == SOUTH){
        difference = beans - m_south[hole];
        m_south[hole] = beans;
    }

    m_totalBeans += difference;

    return true;
}

bool Board::isValidHole(int hole) const { // check if hole is valid
    for (int i = 0; i <= num_holes; i++) {
        if (hole == i) {
            return true;
        }
    }
    return false;
}

void Board::incrementPot(Side s, int value){
    if (s == NORTH){
        m_north[0] += value;
    }
    if (s == SOUTH){
        m_south[0] += value;
    }
}

// SOW ALGORITHMS

void Board::South4South(int& hole){
    if (hole <= num_holes - 1) {
        // Increment the hole index and distribute the beans on the SOUTH side
        hole++;
        m_south[hole]++; // Increment the beans in the current hole on the SOUTH side
    }
    else {
        // If the next hole is the pot, switch to the NORTH side and distribute the bean in the pot
        hole = 0;
        m_south[hole]++; // Increment the beans in the pot on the SOUTH side
    }
}


void Board::North4North(int& hole){
    
    // General algorithm for the NORTH side
    if (hole > 0) {
        // Decrement the hole index and distribute the beans on the NORTH side
        hole--;
        m_north[hole]++; // Increment the beans in the previous hole on the NORTH side
    }
}


void Board::North4South(int& hole, Side& side){
    // Decrementing through the NORTH side
    if (hole > 1) {
        // Decrement the hole index and distribute the beans on the NORTH side
        hole--;
        m_north[hole]++; // Increment the beans in the current hole on the NORTH side
    }
    else {
        // Skip the pot on the NORTH side and go back to the SOUTH side
        side = SOUTH;
        m_south[hole]++; // Increment the beans in the last hole on the NORTH side
    }
}


void Board::South4North(int& hole, Side& side){
    if (hole <= num_holes - 1) {
        // Increment the hole index and distribute the beans on the SOUTH side
        hole++;
        m_south[hole]++; // Increment the beans in the current hole on the SOUTH side
    }
    else {
        // Skip the pot on the SOUTH side and switch to the NORTH side
        side = NORTH;
        m_north[hole]++; // Increment the beans in the current hole on the NORTH side
    }
}

void Board::SwitchSouthtoNorth(int& hole, Side& side){
    if (hole == 0) {
        // Switch to the NORTH side when reaching the pot on the SOUTH side
        side = NORTH;
        hole = num_holes + 1; // Set the current hole to the index of the last hole on the NORTH side
    }
}

void Board::SwitchNorthToSouth(int& hole, Side& side){
    if (hole == 0) {
        // Switch to the SOUTH side when reaching the pot on the NORTH side
        side = SOUTH;
    }
}

