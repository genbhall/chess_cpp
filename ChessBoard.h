#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "Pawn.h"
#include "Bishop.h"
#include "Horse.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "helper.h"


//chessboard class owns the pieces that are on it - pieces will be destroyed
//when chessboard is destroyed

class ChessBoard {

    //to get board access for pieces
    friend class ChessPiece;

    private:
    
    //tracks who's turn it is - starts on white
    char turn = WHITE;

    //track's whether game is over or not
    bool gameLive = true;

    //tracks game pieces positional state
    ChessPiece* chessboard[BOARD_SIZE][BOARD_SIZE];    

    //list of aggressive positions for white and black (all
    //empty spaces under attack AND king threat)
    std::vector<std::array<int,PAIR>> aggrMov;

    public:
    
    // constructor
    ChessBoard();

    //MAIN FUNCTIONS

    //resets board to starting positions and piece states
    void resetBoard();

    //takes in string coordinates for position and move
    //checks if move is valid, if valid moves the piece and then checks
    //whether the game is over or not
    void submitMove(std::string const& position, std::string const& move);

    //AUXILLIARY FUNCTIONS

    //changes active colour at the end of the a turn
    void endTurn();

    //deletes all the pieces from the board
    void deletePieces();

    //creates full set of pieces for White and Black in starting position
    void createPieces();

    //prints appropriate output message and cleans up holder piece
    void commitMove(std::array<int,PAIR> const &pos, 
                    std::array<int,PAIR> const &mov, 
                    std::string const &position, std::string const &move,
                    ChessPiece* &holder);

    //moves piece from position pos to position mov
    void movePiece(std::array<int,PAIR> const &pos, 
                    std::array<int,PAIR> const &mov);

    //converts input coordinate string into integer array
    void stringToArray(std::string const& position, 
                    std::array<int,PAIR> &array);

    //checks if the coordinate inputed is a valid position on the board
    bool const checkValidCoordinate(std::string const& coordinate);

    //returns true is colour argument is same as current turn
    bool const isColourTurn(char pieceColour);

    //refreshes aggrMove vector to contain set of arrays that the
    //respective colour can potentially attack. Also checks protection
    //on friendly pieces (used for King move)
    void updateAggressiveMap(char colour);

    //checks if game is over by testing all potential moves by
    //colour and seeing if they are valid and do not leave the respective
    //king in check
    bool const checkIfGameOver(char colour);
    
    //returns true if colour King is in check
    bool const isInCheck(char colour);

    //returns true if move is a castling move
    bool const isCastling(std::array<int,PAIR> const &pos, 
                    std::array<int,PAIR> const &mov);

    //prints "colour" for argument colour
    void colourPrint(char colour);
    
    //prints full name for 1 character label
    void namePrint(char name);
    
    //print board in cout
    void printLine(int first_col, int second_col, int row);
    void printBoard();

    //destructor
    ~ChessBoard();
};

#endif