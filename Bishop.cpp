#include "Bishop.h"

using namespace std;

Bishop::Bishop(char name, char colour) : ChessSpecial(name, colour) {}

bool Bishop::isValid(array<int,PAIR> const &pos, 
                    array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE],
                    vector<std::array<int,PAIR>> const &agList) {
    
    if (isValidDiagonal(pos, mov, board)) {
        return true;
    }

    return false;
}

void Bishop::aggressiveMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]) {
    
    aggressiveDiagonal(row, col, addVector, board);
}

void Bishop::possibleMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        vector<array<int,PAIR>> const &aggrMov) {
    
    //possible moves same as aggressive moves for Bishop
    aggressiveDiagonal(row, col, addVector, board);
}

Bishop::~Bishop() {}