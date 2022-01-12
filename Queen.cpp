#include "Queen.h"

using namespace std;

Queen::Queen(char name, char colour) : ChessSpecial(name, colour) {}

bool Queen::isValid(array<int,PAIR> const &pos, 
                    array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE],
                    vector<std::array<int,PAIR>> const &agList) {

    //if can move diagonally or horizontally, move is valid - else false                    
    if ((isValidDiagonal(pos, mov, board)) || (isValidStraight(pos, mov, board))) {
        return true;
    }
    return false;
}

void Queen::aggressiveMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE])  {
    
    //queen has diagonal and straight moves
    aggressiveDiagonal(row, col, addVector, board);
    aggressiveStraight(row, col, addVector, board);
}


void Queen::possibleMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        vector<array<int,PAIR>> const &aggrMov)  {
    
    //possible moves same as agg moves for queen
    aggressiveMoves(row, col, addVector, board);
}

Queen::~Queen() {}