

#include"Game.h"
#include"Player.h"
#include"Board.h"

using namespace std;

Game::Game(const Board& b, Player* south, Player* north): myBoard(b), northP(north), southP(south) {}

int Game::beans(Side s, int hole) const {
    return myBoard.beans(s, hole);
}

void Game::display() const{
    std::cout << "NORTH" << '\n' << '\n';
    std::cout << "         " << northP->name() << '\n';
    
    std::cout << "Holes   ";
    
    for (int i = 1; i <= myBoard.holes(); i++){
        std::cout << ' ' << i << ' ';
    }
    
    std::cout << '\n' << "Beans   ";
    for (int i = 1; i <= myBoard.holes(); i++){
        std::cout << ' ' << myBoard.beans(NORTH, i) << ' ';
    }
    std::string spaces (2*myBoard.holes() + 2,' '); // 9 spaces
    
    std::cout << '\n' << "Pots    " << myBoard.beans(NORTH,0) << spaces << myBoard.beans(SOUTH,0) << '\n';
    
    std::cout << "Holes   ";
    for (int i = 1; i <= myBoard.holes(); i++){
        std::cout << ' ' << i << ' ';
    }
    
    std::cout << '\n' << "Beans   ";

    for (int i = 1; i <= myBoard.holes(); i++){
        std::cout << ' ' << myBoard.beans(SOUTH, i) << ' ';
    }
    
    std::cout << '\n' <<  "         " << southP->name();
    std::cout << '\n' << '\n' << "SOUTH" << '\n';
    std::cout << "---------------------------" << '\n';
}

void Game::play(){
    
    bool over(false), hasWinner(false);
    Side s;
    
    if(northP->isInteractive() || southP->isInteractive()){
        std::cout << northP->name() << " (playing north)" << " VS " << southP->name() << " (playing south)" << '\n';
        display();
        
    }
    
    do {
        over = !move(SOUTH);     // south player always moves first
        status(over,hasWinner,s);

        if (!over){ // this checks if south's move ended the game
            over = !move(NORTH);
            status(over,hasWinner,s);
        }
        
    } while(!over); // keep playing while its not over
    
    if (hasWinner){
        if (s == SOUTH){
            std::cout << "The winner is " << southP->name() << '\n'; // prints out winner
        }
        else {std::cout << "The winner is " << northP->name() << '\n';}
    }
    else { std::cout << "Tie Game! " << '\n';}
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
    
    if (myBoard.beansInPlay(NORTH) != 0 && myBoard.beansInPlay(SOUTH) != 0) { //game continues
        hasWinner = false;
        over = false;
        return;
    }
    over = true;

    if (myBoard.beans(NORTH, POT) == myBoard.beans(SOUTH, POT)) { //tie game
        hasWinner = false;
    }
    else {
        hasWinner = true;
        winner = beans(NORTH,POT) > beans(SOUTH,POT) ? NORTH : SOUTH;
    }
    
    return;
}

bool Game::move(Side s) {

    if (s == SOUTH) {
        if (southP->isInteractive() && !northP->isInteractive()) {
            // Display the board if SOUTH player is interactive and NORTH player is not
            display();
        }

        int chosenHole = southP->chooseMove(myBoard, s);

        if (!southP->isInteractive()) {
            // Indicate the move chosen by the computer player
            cout << "SOUTH player picks hole " << chosenHole << '\n';
        }
        
        int endHole;
        Side endSide;

        if (myBoard.sow(s, chosenHole, endSide, endHole)) {
            if (endHole == 0) {
                if (myBoard.beansInPlay(SOUTH) > 0) {
                    // SOUTH player gets another turn if the last bean lands in the pot
                    cout << southP->name() << " plays again!" << '\n';
                    display();
                    return move(s); // Call move again
                }
            }
            else if (endSide == s) {
                captureMove(s, endHole, myBoard);
            }
        }
    }
    else { // North's move
        if (!southP->isInteractive() && northP->isInteractive()) {
            // Display the board if NORTH player is interactive and SOUTH player is not
            display();
        }

        int chosenHole = northP->chooseMove(myBoard, s);

        if (!northP->isInteractive()) {
            // Indicate the move chosen by the computer player
            cout << "NORTH player picks hole " << chosenHole << '\n';
        }
        
        int endHole;
        Side endSide;
        
        if (myBoard.sow(s, chosenHole, endSide, endHole)) {
            if (endHole == 0) {
                if (myBoard.beansInPlay(NORTH) > 0) {
                    // NORTH player gets another turn if the last seed lands in the pot
                    cout << northP->name() << " plays again!" << '\n';
                    display();
                    return move(s); // Call move again
                }
            } else if (endSide == s) {
                captureMove(s,endHole, myBoard);

            }
        }
    }

    bool over, hasWinner;
    Side winner;
    status(over, hasWinner, winner); // Check the status of the game

    if (over) {
        // Game is finished
        for (int i = 1; i <= myBoard.holes(); i++) {
            // Add opponent's holes to opponent's pot
            myBoard.moveToPot(opponent(s), i, opponent(s));
        }
        
        display();

        std::cout << "Final result is " << northP->name() << " with " << myBoard.beans(NORTH, POT);
        std::cout << ", " << southP->name() << " with " << myBoard.beans(SOUTH, POT) << '\n';
        return false;
    }

    return true;
}

void Game::captureMove(Side& s, int& hole, Board& myBoard){
    if (myBoard.beans(s, hole) == 1 && myBoard.beans(opponent(s), hole) > 0) {
        // Capture move: only one seed in the ending hole and opponent's hole is not empty
        myBoard.moveToPot(opponent(s), hole, s);
        myBoard.moveToPot(s, hole, s);
    }
}
