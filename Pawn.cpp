#include "Pawn.h"
#include <iostream>

using namespace std;

Pawn::Pawn(char name, char colour) : ChessPiece(name, colour) {}

bool Pawn::isValid(array<int,PAIR> const &pos, 
                    array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE],
                    vector<std::array<int,PAIR>> const &agList) {
    
    //create directional variable for pawn
    int direct = 0;
    (colour == WHITE) ? direct = 1 : direct = -1;

    //if square infront is empty, can move forward 1
    if ((mov[ROW] == pos[ROW]+direct) && (mov[COL] == pos[COL])
        && (board[mov[ROW]][mov[COL]] == nullptr)) {
        return true;
    }

    //if first move and both square infront is empty, can move forward 2
    if ((mov[ROW] == pos[ROW]+2*direct) && (mov[COL] == pos[COL])
        && (board[pos[ROW]+2*direct][pos[COL]] == nullptr) 
        && (board[pos[ROW]+direct][pos[COL]] == nullptr)
        && (firstmove)) {
        return true;
    }


    //if diagonal adjacent pieces occupied by opponent colour, can move in
    if ((mov[ROW] == pos[ROW]+direct) 
        && ((mov[COL] == pos[COL]+direct) || (mov[COL] == pos[COL]-direct))
        && (board[mov[ROW]][mov[COL]] != nullptr)
        && (board[mov[ROW]][mov[COL]]->getColour() != colour)) {
        return true;
    }

    return false;
}

void Pawn::aggressiveMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE])  {

    //create directional variable
    int direct = 1;
    int adjArray[PAIR] = {1,-1};

    //change directional variable to negative if black
    if (colour == BLACK) {
        direct = -1;
    }

    //initialise aggressive mov
    array<int,PAIR> agMov;

    //add valid diagonal adjacent moves to vector
    for (int i = 0; i < PAIR; i++) {
        agMov[ROW] = row+direct;
        agMov[COL] = col+adjArray[i];
        if (insideBoard(agMov[ROW], agMov[COL])) {
            addVector.push_back(agMov);

            //add protection to friendly pieces
            if ((board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() == colour)) {
                board[agMov[ROW]][agMov[COL]]->setProtected(true);
            }
        }
    }
}

void Pawn::possibleMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        vector<array<int,PAIR>> const &aggrMov) {
    
    //create directional variable for pawn
    int direct = 0;
    int adjArray[PAIR] = {1,-1};
    (colour == WHITE) ? direct = 1 : direct = -1;

    //initialise possible mov
    array<int,PAIR> possMov;

    //if square infront is empty, add move forward 1
    if ((insideBoard(row+direct, col)) &&
        (board[row+direct][col] == nullptr)) {
        possMov[ROW] = row+direct;
        possMov[COL] = col;
        addVector.push_back(possMov);
    }

    //if first move and both square in front empty, add move forward 2
    if ((insideBoard(row+2*direct, col))
        && (board[row+2*direct][col] == nullptr) 
        && (board[row+direct][col] == nullptr)
        && (firstmove)) {
        possMov[ROW] = row+2*direct;
        possMov[COL] = col;
        addVector.push_back(possMov);
    }

    //if diagonal forward adjacent occupied by opponent piece
    for (int i = 0; i < PAIR; i++) { 
        int rowMov = row+direct;
        int colMov = col+adjArray[i];

        if ((insideBoard(rowMov, colMov)) 
            && (board[rowMov][colMov] != nullptr)
            && (board[rowMov][colMov]->getColour() != colour)) {
            possMov[ROW] = rowMov;
            possMov[COL] = colMov;
            addVector.push_back(possMov);
        }
    }
}


Pawn::~Pawn() {}