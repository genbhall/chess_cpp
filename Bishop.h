#ifndef BISHOP_H
#define BISHOP_H

#include "ChessSpecial.h"

class Bishop : public ChessSpecial {

    public:

    //constructor
    Bishop(char name, char colour); 

    //checks if move is valid
    bool isValid(std::array<int,PAIR> const &pos, 
                std::array<int,PAIR> const &mov,
                ChessPiece* const board[][BOARD_SIZE],
                std::vector<std::array<int,PAIR>> const &aggrMov) override;

    //adds aggressive  moves for piece to vector
    void aggressiveMoves(int const row, int const col, 
                        std::vector<std::array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]) override;

    //adds possible moves for piece to vector
    void possibleMoves(int const row, int const col, 
                        std::vector<std::array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE],
                        std::vector<std::array<int,PAIR>> const &aggrMov) override;
   
    //override destructor
    ~Bishop() override;
};

#endif