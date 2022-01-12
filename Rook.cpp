#include "Rook.h"

using namespace std;

Rook::Rook(char name, char colour) : ChessSpecial(name, colour) {}

bool Rook::isValid(array<int,PAIR> const &pos, 
                    array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE],
                    vector<std::array<int,PAIR>> const &agList) {

    //if is valid move, then switch rook first move off, return true
    if (isValidStraight(pos, mov, board)) {
        return true;
    }
    return false;
} 

void Rook::aggressiveMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]) {

    aggressiveStraight(row, col, addVector, board);
}


void Rook::possibleMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        vector<array<int,PAIR>> const &aggrMov) {

    //possible moves same as aggressive moves for Rook
    aggressiveStraight(row, col, addVector, board);
}


Rook::~Rook() {}
