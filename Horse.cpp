#include "Horse.h"
#include <iostream>
#include <stdlib.h> 

using namespace std;

Horse::Horse(char name, char colour) : ChessPiece(name, colour) {}

bool Horse::isValid(array<int,PAIR> const &pos, 
                    array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE],
                    vector<std::array<int,PAIR>> const &agList) {
    
    //if move space occupied by same colour piece, return false
    if (posMovSameCol(pos, mov, board)) {
        return false;
    } 

    //knight moves one small move and one big move
    int const small = 1, big = 2;
    
    //if move is 2 squares vert and 1 square horizontal OR
    //if move is 1 square vert and 2 square horizontal from pos, move valid
    if (((abs(pos[ROW] - mov[ROW]) == big) &&
        (abs(pos[COL] - mov[COL]) == small)) || 
        ((abs(pos[ROW] - mov[ROW]) == small) &&
        (abs(pos[COL] - mov[COL]) == big))) {
        return true;
    }
    
    //if not, it's an invalid move
    return false;
}

void Horse::aggressiveMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE])  {
    
    //movement direction
    int const direct[PAIR] = {1, -1};
    int const small = 1, big = 2;

    //initialise aggressive mov
    array<int,PAIR> agMov;

    for (int hor = 0; hor < PAIR; hor++) {
        for (int vert = 0; vert < PAIR; vert++) {

            //check movement 2 horiontal 1 vert
            agMov[ROW] = row+big*direct[hor];
            agMov[COL] = col+small*direct[vert];

            //if move is within board range, empty cell or occupied by
            //opponent, then add to the aggressive move vector
            if  ((insideBoard(agMov[ROW], agMov[COL])) &&
                ((board[agMov[ROW]][agMov[COL]] == nullptr) || 
                (board[agMov[ROW]][agMov[COL]]-> getColour() != colour))) {
                addVector.push_back(agMov);
            }

            //update protection on friendly pieces
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() == colour)) {
                board[agMov[ROW]][agMov[COL]]->setProtected(true);
            }

            //check movement 1 horiontal 2 vert
            agMov[ROW] = row+small*direct[hor];
            agMov[COL] = col+big*direct[vert];

            //if move is within board range, empty cell or occupied by
            //opponent, then add to the aggressive move vector
            if  ((insideBoard(agMov[ROW], agMov[COL])) &&
                ((board[agMov[ROW]][agMov[COL]] == nullptr) || 
                (board[agMov[ROW]][agMov[COL]]-> getColour() != colour))) {
                addVector.push_back(agMov);
            } 

            //update protection on friendly pieces
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() == colour)) {
                board[agMov[ROW]][agMov[COL]]->setProtected(true);
            }
        }
    }
}

//adds possible moves for piece to vector
void Horse::possibleMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        vector<array<int,PAIR>> const &aggrMov) {
    
    //possible moves same as aggressive moves for knight
    aggressiveMoves(row, col, addVector, board);
}



Horse::~Horse() {}