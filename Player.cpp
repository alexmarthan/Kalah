
#include "Player.h"
#include "Side.h"
#include <climits>

using namespace std;

//BASE CLASS
Player::Player(std::string name) :m_name(name) {}

Player::~Player() {}

std::string Player::name() const {
    return m_name;
}

bool Player::isInteractive() const {
    return false;
}

//HUMAN PLAYER

HumanPlayer::HumanPlayer(std::string name): Player(name){}

bool HumanPlayer::isInteractive() const{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
    int yourMove;
    
    do {
        std::cout << "What's your move? " << '\n';
        std::cin >> yourMove;
    }
    while (yourMove > b.holes() || yourMove <= 0); // move cant be smaller or equal to 0 or bigger than the nb of holes
    
    return yourMove;
}

//BAD PLAYER

BadPlayer::BadPlayer(std::string name): Player(name){}

int BadPlayer::chooseMove(const Board& b, Side s) const{ // always chooses the first hole with beans in it
    for (int i = 1; i <= b.holes(); i++){
        if (b.beans(s, i) != 0){
            return i;
        }
    }
    return -999; // should never be this (Game's move function checks that there are available moves to be made)
}

//SMART PLAYER

SmartPlayer::SmartPlayer(std::string name) : Player(name) {}

int SmartPlayer::chooseMove(const Board &b, Side s) const {
    int bestHole, value, depth(0); // depth starts at 0
    JumpyTimer myTime(1000);
    miniMax(b, s, bestHole, value, depth, myTime);
    return bestHole;
}



int SmartPlayer::evaluate(const Board& b) const {
    int PotDiff = b.beans(SOUTH, POT) - b.beans(NORTH, POT);

    if (b.beansInPlay(SOUTH) == 0 && b.beansInPlay(NORTH) == 0) { // changed from ||
        if (PotDiff > 0) {
            return INT_MAX;
        } else if (PotDiff == 0) {
            return 0;
        } else {
            return INT_MIN;
        }
    }
    return PotDiff;
}

bool SmartPlayer::moveSimulator(Side s, int hole, Board &b, Side& endSide, int& endHole) const {
    bool moveCompleted = b.sow(s, hole, endSide, endHole);
    
    if (moveCompleted) {
        if (endHole == 0) {
            // The player gets another turn as the last sow ended in their pot
            return false;
        }
        else if (endSide == s) {
            int playerBeans = b.beans(s, endHole);
            int opponentBeans = b.beans(opponent(s), endHole);
            
            // Capture condition: There is only one bean in the last hole and the opponent has beans in the corresponding hole
            if (opponentBeans > 0 && playerBeans == 1) {
                b.moveToPot(SOUTH, endHole, s); // Capture opponent's beans
                b.moveToPot(NORTH, endHole, s); // Move the player's last bean to their pot
            }
        }
        
        return true; // The move was successfully completed
    }
    
    return false; // The move was invalid
}

void SmartPlayer::miniMax(const Board& board, Side s, int& bestHole, int& value, int searchDepth, JumpyTimer& myTime) const {
    // Base case: If the search depth limit is reached or no beans are in play for the current side,
    // set bestHole to -1 and evaluate the board to determine the value.
    if (searchDepth > 6 || board.beansInPlay(s) == 0 /*|| myTime.elapsed() > 1000*/) {
        bestHole = -1;
        value = evaluate(board);
        return;
    }
    
    if (myTime.elapsed() > 4800) { // staying on the safe side
        bestHole = -1;
        value = evaluate(board);
        return;
    }

    bestHole = -1;
    value = (s == SOUTH) ? INT_MIN : INT_MAX;
    
    // Iterate through the holes to find the best move for the current side.
    // Iterate through the holes to find the best move for the current side.
    int validHoleIndex = 0;

    for (int i = 1; i <= board.holes(); i++) {
        // Check if the current hole has beans.
        if (board.beans(s, i) != 0) {
            validHoleIndex++;

            // Create temporary variables for move simulation.
            int endHole, newValue, newHole1, newHole2;
            Side endSide;
            Board tempBoard(board);

            // Simulate the move on the temporary board and evaluate the resulting board.
            if (!moveSimulator(s, i, tempBoard, endSide, endHole)) {
                // If unable to make the move, retry with a different move.
                miniMax(tempBoard, s, newHole2, newValue, searchDepth, myTime);
            } else {
                // If the move is successful, recursively evaluate the resulting board.
                miniMax(tempBoard, opponent(s), newHole1, newValue, searchDepth + 1, myTime);
            }

            // Update the value and bestHole if this is the first valid hole encountered in the loop
            // or a better move is found based on the current side's perspective.
            if (validHoleIndex == 1 || (s == SOUTH && newValue > value) || (s == NORTH && newValue < value)) {
                value = newValue;
                bestHole = i;
            }
        }
    }
}
