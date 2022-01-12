#include "helper.h"
#include "ChessPiece.h"

#ifndef CHESSSPECIAL_H
#define CHESSSPECIAL_H

//class for Bishop, Rook and Queen containing move
//diagonal, move straight functions
class ChessSpecial : public ChessPiece {

    protected:

    ChessSpecial(char name, char colour);

    bool const isValidDiagonal(std::array<int,PAIR> const &pos, 
                                std::array<int,PAIR> const &mov,
                                ChessPiece* const board[][BOARD_SIZE]) const;


    bool const isValidStraight(std::array<int,PAIR> const &pos, 
                                std::array<int,PAIR> const &mov,
                                ChessPiece* const board[][BOARD_SIZE]) const;


    void aggressiveDiagonal(int const row, int const col, 
                        std::vector<std::array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]);

    void aggressiveStraight(int const row, int const col, 
                        std::vector<std::array<int,PAIR>> &addVector,
                        ChessPiece* const board[][BOARD_SIZE]);
};

#endif