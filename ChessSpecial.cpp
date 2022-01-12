#include "ChessSpecial.h"
#include <iostream>

using namespace std;

ChessSpecial::ChessSpecial(char name, char colour) : ChessPiece(name, colour) {}

bool const ChessSpecial::isValidDiagonal(array<int,PAIR> const &pos, 
                                        array<int,PAIR> const &mov,
                                        ChessPiece* const board[][BOARD_SIZE]) const {

   //if move space occupied by same colour piece, return false
    //this also solves inputing mov = pos
    if (posMovSameCol(pos, mov, board)) {
        return false;
    }

    //initialise two direction size variables for board
    int rowDifference = mov[ROW] - pos[ROW];
    int colDifference = mov[COL] - pos[COL];

    //diagonal movements should be equal; if not, false
    if (abs(rowDifference) != abs(colDifference)) {
        return false;
    }
    
    //set the directional variables based on relative pos to mov
    int rowDirect = rowDifference/abs(rowDifference);
    int colDirect = colDifference/abs(colDifference);

    //relative movement direction for mov vs pos 
    //if squares between pos and mov are not empty, return false
    for (int i = 1; i < abs(rowDifference); i++) {
        if (board[pos[ROW]+i*rowDirect][pos[COL]+i*colDirect] != nullptr) {
            return false;
        }
    }

    //if above conditions passed, then valid move
    return true;
}

bool const ChessSpecial::isValidStraight(array<int,PAIR> const &pos, 
                                        array<int,PAIR> const &mov,
                                        ChessPiece* const board[][BOARD_SIZE]) const {

    //if move space occupied by same colour piece, return false
    //this also solves inputing mov = pos
    if (posMovSameCol(pos, mov, board)) {
        return false;
    }

    //either row or col must be equal
    if ((pos[ROW] != mov[ROW]) && (pos[COL] != mov[COL])) {
        return false;
    }

    //initialise the counter for number of squares moved, row or col
    //and up or down movements in three variables movCount, rowMov and movDir
    int moveCount = 0;
    bool rowMov = false;

    if (mov[ROW] != pos[ROW]) {
        moveCount = mov[ROW] - pos[ROW];
        rowMov = true;
    } else {
        moveCount = mov[COL] - pos[COL];
    }

    int moveDirect = 0;
    moveDirect = moveCount/abs(moveCount);

    //if squares between mov and pos are not nullptr - then move invalid
    for (int i = 1; i < abs(moveCount); i++) {
        if (board[pos[ROW]+i*moveDirect*rowMov][pos[COL]+i*moveDirect*!rowMov]
            != nullptr) {
            return false;
        }
    }

    //if all conditions above pass, return true
    return true;
}

void ChessSpecial::aggressiveDiagonal(int const row, int const col, 
                        vector<array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]) {

    //movement direction
    int direct[PAIR] = {1, -1};
    
    //initialise aggressive mov
    array<int,PAIR> agMov;

    //cycle through empty diagonal boxes to base position
    //first two for loops track four possible mov directions
    for (int rowMov = 0; rowMov < PAIR; rowMov++) {
        for (int colMov = 0; colMov < PAIR; colMov++) {
            
            //initialise movement counter
            int count = 1;

            //while loop tracks how many empty squares possible
            while (count < BOARD_SIZE) {
                agMov[ROW] = row+count*direct[rowMov];
                agMov[COL] = col+count*direct[colMov];

                //if move valid and occupied by opponent's colour add to list
                if ((insideBoard(agMov[ROW], agMov[COL])) &&
                    (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                    (board[agMov[ROW]][agMov[COL]]->getColour() != colour)) {
                    addVector.push_back(agMov);
                    break;
                }

                //if friendly piece, add protection and break loop
                if ((insideBoard(agMov[ROW], agMov[COL])) &&
                    (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                    (board[agMov[ROW]][agMov[COL]]->getColour() == colour)) {
                    board[agMov[ROW]][agMov[COL]]->setProtected(true);
                    break;
                }

                //if square is an empty space, add it to the list, continue loop
                if ((insideBoard(agMov[ROW], agMov[COL])) &&
                    (board[agMov[ROW]][agMov[COL]] == nullptr)) {
                    addVector.push_back(agMov);
                }
                count++;
            }
        }
    }
}

void ChessSpecial::aggressiveStraight(int const row, int const col, 
                    std::vector<std::array<int,PAIR>> &addVector,
                    ChessPiece* const board[][BOARD_SIZE]) {

    //movement direction
    int direct[PAIR] = {1, -1};

    //initialise aggressive mov
    array<int,PAIR> agMov;
    
    //cycle through empty vert boxes to base position
    for (int i = 0; i < PAIR; i++) {
    
        //initialise movement counter
        int count = 1;

        //first while loop for vertical movement in direction i
        while (count < BOARD_SIZE) {

            agMov[ROW] = row+count*direct[i];
            agMov[COL] = col;

            //if move valid and occupied by opponent, add to list and break
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() != colour)) {
                addVector.push_back(agMov);
                break;
            }

            //if move valid and occupied by friendly, mark protected and break 
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() == colour)) {
                board[agMov[ROW]][agMov[COL]]->setProtected(true);
                break;
            }

            //if empty space add to list, continue while loop
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] == nullptr)) {
                addVector.push_back(agMov);
            } 
            
            count++;
        }
    }

    //cycle through empty horizontal boxes to base position
    for (int i = 0; i < PAIR; i++) {
    
        //initialise movement counter
        int count = 1;

        //first while loop for horizontal movement in direction i
        while (count < BOARD_SIZE) {

            agMov[ROW] = row;
            agMov[COL] = col+count*direct[i];

            //if move valid and occupied by opponent, add to list 
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() != colour)) {
                addVector.push_back(agMov);
                break;
            }

            //if move valid and occupied by friendly, mark protected and break 
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] != nullptr) &&
                (board[agMov[ROW]][agMov[COL]]->getColour() == colour)) {
                board[agMov[ROW]][agMov[COL]]->setProtected(true);
                break;
            }

            //if empty space, add to list - otherwise break while
            if ((insideBoard(agMov[ROW], agMov[COL])) &&
                (board[agMov[ROW]][agMov[COL]] == nullptr)) {
                addVector.push_back(agMov);
            }

            count++;
        }
    }
}