#include "King.h"
#include <iostream>

using namespace std;

King::King(char name, char colour) : ChessPiece(name, colour) {}

bool King::isValid(array<int,PAIR> const &pos, 
                    array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE],
                    vector<array<int,PAIR>> const &aggrMov) {
    
    //if move space occupied by same colour piece, return false
    if (posMovSameCol(pos, mov, board)) {
        return false;
    }

    //initialise two direction size variables
    int rowDifference = mov[ROW] - pos[ROW];
    int colDifference = mov[COL] - pos[COL];

    //castling move
    if (isCastleValid(pos, mov, board, aggrMov)) {
        return true;
    }

    //cannot move more than one square at a time
    if ((abs(rowDifference) > 1) || (abs(colDifference) > 1)) {
        return false;
    }

    // //cannot move into check (check if mov is not in set of aggressive pieces)
    // //helps reduce iterations vs try and commit move on board (which should
    // //catch if you're moving into check anyway)
    // if (isInAggrSet(mov, aggrMov)) {
    //     cout << "Cannot move into check" << endl;
    //     return false;
    // }

    //if passes conditions, return true
    //moving into check dealt with at board level with try/submit move.
    return true;
}

void King::aggressiveMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE])  {
    
    //King adjacent squares occupy 3x3 grid with pos in centre
    int const gridSize = 3;

    //initialise aggressive mov
    array<int,PAIR> agMov;

    //empty squares or those occupied by opponent pieces
    //in surrounding grid are aggressive
    for (int hor = 0; hor < gridSize; hor++) {
        for (int vert = 0; vert < gridSize; vert++) {
            
            // -1 because want to offset so [row, col] is middle of 3x3 grid
            agMov[ROW] = row + hor - 1;
            agMov[COL] = col + vert - 1;

            //if valid move
            if (insideBoard(agMov[ROW], agMov[COL])) {

                //empty squares or those occupied by opponent pieces AND
                //left unprotected
                if ((board[agMov[ROW]][agMov[COL]] == nullptr) ||
                    ((board[agMov[ROW]][agMov[COL]]->getColour() != colour) &&
                    (board[agMov[ROW]][agMov[COL]]->getProtected() == false))) {
                    addVector.push_back(agMov);
                } else {
                    //if not empty or opponent, must be friendly
                    board[agMov[ROW]][agMov[COL]]->setProtected(true);
                }
            }
        }
    }
}

void King::possibleMoves(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        vector<array<int,PAIR>> const &aggrMov) {
    
    //set of aggressive moves is subset of possible moves for King
    aggressiveMoves(row, col, addVector, board);

    //add castling moves
    int const direction[PAIR] = {1, -1};
    array<int,PAIR> currentMov;
    array<int,PAIR> possMov;

    currentMov[ROW] = row;
    currentMov[COL] = col;
    
    for (int i = 0; i < PAIR; i++) {
        possMov[COL] = col + 2*direction[i];
        
        if (colour == WHITE) {
            possMov[ROW] = 0;
            if (isCastleValid(currentMov, possMov, board, aggrMov)) {
                addVector.push_back(possMov);
            }
        } else {
            possMov[ROW] = BOARD_SIZE-1;
            if (isCastleValid(currentMov, possMov, board, aggrMov)) {
                addVector.push_back(possMov);
            }
        }
    }
}

bool const King::isCastleValid(array<int,PAIR> const &pos, 
            array<int,PAIR> const &mov,
            ChessPiece* const board[][BOARD_SIZE],
            vector<array<int,PAIR>> const &aggrMov) {

    //cannot castle in check
    if (isInAggrSet(pos, aggrMov)) {
        return false;
    }

    //set needed directional and distance variables
    int const count = mov[COL] - pos[COL];
    int const direct = count/abs(count);

    //check king movement is 2 squares horiz and 0 vert.
    //or if it is the king's first move
    if ((abs(count) != 2) ||
        (abs(mov[ROW] - pos[ROW]) != 0) ||
        (!firstmove)) {
        return false;
    }

    //check 2 adjacent squares for king are empty and not
    //in opponent's aggressive set
    for (int i = 1; i <= abs(count); i++) {
        
        //create array and assign coordinates
        array<int,PAIR> coord;
        coord[ROW] = pos[ROW];
        coord[COL] = pos[COL] + i*direct;

        if ((board[coord[ROW]][coord[COL]] != nullptr) ||
            (isInAggrSet(coord, aggrMov))) {
            return false;
        }
    }

    //if castling to left, check furthest left is rook and 
    //it hasn't moved, otherwise check furthest right
    if (direct < 0) {
        if ((board[pos[ROW]][0] != nullptr) &&
            (board[pos[ROW]][0]->getName() == ROOK) &&
            (board[pos[ROW]][0]->getFirstMove())) {
            return true;
        }
    } else {
        if ((board[pos[ROW]][BOARD_SIZE-1] != nullptr) &&
            (board[pos[ROW]][BOARD_SIZE-1]->getName() == ROOK) &&
            (board[pos[ROW]][BOARD_SIZE-1]->getFirstMove())) {
            return true;
        }
    }

    //if conditions above not passed, then return false
    return false;
}

bool const King::isInAggrSet(array<int,PAIR> const &mov,
                    vector<array<int,PAIR>> const &aggrMov) {
    for (auto i = 0u; i < aggrMov.size(); i++) {
        if ((aggrMov[i][ROW] == mov[ROW]) &&
            (aggrMov[i][COL] == mov[COL])) {
            return true;
        }
    }
    return false;
}


King::~King() {}
