#include "ChessBoard.h"
#include "helper.h"

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <array>
#include <string>
#include <vector>

class ChessPiece {
    
    protected: 

    //piece characteristics
    char colour;
    char name;

    //first move tracker for pawn, king and rook
    bool firstmove = true;

    //is piece protected by friendly piece
    //initially starts on false when new game is set and piece created
    bool isProtected = false;

    public:

    //constructor
    ChessPiece(char name, char colour);
    
    //getter functions
    char const getName() const;
    char const getColour() const;
    bool const getFirstMove() const;
    bool const getProtected() const;

    //setter function
    void setProtected(bool protect);

    //turns first move off
    void firstMoveOff();

    //checks move space not occupied by a piece of the same colour
    //this also solves inputting move that is same as position
    bool const posMovSameCol(std::array<int,PAIR> const &pos, 
                    std::array<int,PAIR> const &mov, 
                    ChessPiece* const board[][BOARD_SIZE]) const;

    //checks if position is inside board
    bool const insideBoard(int const row, int const col);

    //virtual function to check if move is valid
    virtual bool isValid(std::array<int,PAIR> const &pos, 
                std::array<int,PAIR> const &mov,
                ChessPiece* const board[][BOARD_SIZE],
                std::vector<std::array<int,PAIR>> const &aggrMov) = 0;
    
    //adds aggressive  moves for piece to vector addVector
    virtual void aggressiveMoves(int const row, int const col, 
                        std::vector<std::array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]) = 0;

    //adds possible moves for piece to vector addVector
    virtual void possibleMoves(int const row, int const col, 
                        std::vector<std::array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        std::vector<std::array<int,PAIR>> const &aggrMov) = 0;

    //virtual destructor
    virtual ~ChessPiece();
};

#endif