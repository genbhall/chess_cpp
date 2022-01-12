#include "ChessPiece.h"

using namespace std;

ChessPiece::ChessPiece(char name, char colour) 
            : colour(colour), name(name) {}

char const ChessPiece::getName() const {
    return name;
}

char const ChessPiece::getColour() const {
    return colour;
}

bool const ChessPiece::getFirstMove() const {
    return firstmove;
}

bool const ChessPiece::getProtected() const {
    return isProtected;
}

void ChessPiece::setProtected(bool protect) {
    isProtected = protect;
}

void ChessPiece::firstMoveOff() {
    firstmove = false;
}
 
bool const ChessPiece::insideBoard(int const row, int const col) {
    if ((row >= BOARD_SIZE) || (col >= BOARD_SIZE) ||
        (row < 0) || (col < 0)) {
        return false;
    }
    return true;
}

bool const ChessPiece::posMovSameCol(array<int,PAIR> const &pos, 
                array<int,PAIR> const &mov, 
                ChessPiece* const board[][BOARD_SIZE]) const {

    if ((board[mov[ROW]][mov[COL]] != nullptr) &&
        (board[mov[ROW]][mov[COL]]->getColour() == colour)) {
        return true;
    }
    return false;
}

ChessPiece::~ChessPiece() {}