#include "ChessBoard.h"

using namespace std;

ChessBoard::ChessBoard() {

    //first initialise the board to nullptr
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
         chessboard[row][col] = nullptr;
        }
    }

    //then create the pieces for the board
    createPieces();
}

void ChessBoard::resetBoard() {
    turn = WHITE;
    gameLive = true;
    aggrMov.clear();
    deletePieces();
    createPieces();
}

void ChessBoard::submitMove(string const& position, string const& move) {

    if (gameLive == false) {
        cout << "Game is over - please reset board to play again" << endl;
        return;
    } 

    //check if coordinate inputs are ok - nb error message in checkV. func. 
    if ((!checkValidCoordinate(position)) || (!checkValidCoordinate(move))) {
        return;
    }
    
    //create two pair arrays holding x,y position coordinates and assign them
    array<int,PAIR> pos, mov;
    stringToArray(position, pos);
    stringToArray(move, mov);

    if (chessboard[pos[ROW]][pos[COL]] == nullptr) {
        cout << "There is no piece at position " << position << "!\n";
        return;
    }

    //check that the piece moved matches colour with turn
    if (!isColourTurn(chessboard[pos[ROW]][pos[COL]]->getColour())) {
        cout << "It is not ";
        (turn == BLACK) ? colourPrint(WHITE) : colourPrint(BLACK);
        cout << "'s turn to move!" << endl;
        return;
    }

    //update aggressive move list for opponents turn
    char opponent;
    (turn == BLACK) ? opponent = WHITE : opponent = BLACK;
    updateAggressiveMap(opponent);

    //if a normal move is valid, try moving it. If it does not 
    //leave the king in check, then commit to move the piece
    if (chessboard[pos[ROW]][pos[COL]]->
        isValid(pos, mov, chessboard, aggrMov)) {
        
        //TRY MOVE - move the piece
        ChessPiece* holder = chessboard[mov[ROW]][mov[COL]];
        movePiece(pos, mov);

        //then update the aggressive map for opponent colour
        updateAggressiveMap(opponent);

        //check if current player in check with new move. 
        //If true, return piece and give error message
        if (isInCheck(turn)) {
            movePiece(mov,pos);
            chessboard[mov[ROW]][mov[COL]] = holder;
            holder = nullptr;
            cout << "Invalid move - cannot move King into check / leave King" 
                << " in check at the end of the move" << endl;
            return;
        }

        //if move valid and doesn't leave King checked, move piece officially
        //amnd return respective message
        commitMove(pos, mov, position, move, holder);    
        
        //update so aggr list now holds aggression for active colour
        updateAggressiveMap(turn);

    //if move not considered to be valid, then return error message 
    } else {
        colourPrint(chessboard[pos[ROW]][pos[COL]]->getColour());
        cout << "'s ";
        namePrint(chessboard[pos[ROW]][pos[COL]]->getName());
        cout << " cannot move to " << move << "!" << endl;
        return;
    }

    //check game conditions to see if any valid moves for opponent
    //this ensures only need to run checkIfGameOver once
    bool gameOver = checkIfGameOver(opponent);

    //if is check and gameOver, check mate. If not in check and 
    //game over, then stale mate
    if (isInCheck(opponent)) {
        if (gameOver) {
            colourPrint(opponent);
            cout << " is in checkmate" << endl;
            return;        
        }
        colourPrint(opponent);
        cout << " is in check" << endl;
    } else {
        if (gameOver) {
            cout << "Stale mate" << endl;
            return;
        }
    }

    //change turn colour
    endTurn();
}

void ChessBoard::endTurn() {
    (turn == BLACK) ? turn = WHITE : turn = BLACK;
}

void ChessBoard::deletePieces() {
    for (int row = 0; row < BOARD_SIZE; row++) {
       for (int col = 0; col < BOARD_SIZE; col++) {
            if (chessboard[row][col] != nullptr) {
                delete chessboard[row][col];
                chessboard[row][col] = nullptr;
            }
        }
    }
}

void ChessBoard::createPieces() {

    //set up starting positions for pieces - const variables
    int const white_pawns = 1;
    int const white_special = 0;
    int const black_pawns = 6;
    int const black_special = 7;
    
    //create the white pawns
    for (int i = 0; i < NUM_PAWNS; i++) {
        chessboard[white_pawns][i] = new Pawn(PAWN, WHITE);
    }

    //create the black pawns
    for (int i = 0; i < NUM_PAWNS; i++) {
        chessboard[black_pawns][i] = new Pawn(PAWN, BLACK);
    }

    //create the bishops
    chessboard[white_special][2] = new Bishop(BISHOP, WHITE);
    chessboard[white_special][5] = new Bishop(BISHOP, WHITE);
    chessboard[black_special][2] = new Bishop(BISHOP, BLACK);
    chessboard[black_special][5] = new Bishop(BISHOP, BLACK);

    //create the knights
    chessboard[white_special][1] = new Horse(HORSE, WHITE);
    chessboard[white_special][6] = new Horse(HORSE, WHITE);
    chessboard[black_special][1] = new Horse(HORSE, BLACK);
    chessboard[black_special][6] = new Horse(HORSE, BLACK);

    //create the Rooks
    chessboard[white_special][0] = new Rook(ROOK, WHITE);
    chessboard[white_special][7] = new Rook(ROOK, WHITE);
    chessboard[black_special][7] = new Rook(ROOK, BLACK);
    chessboard[black_special][0] = new Rook(ROOK, BLACK);

    //create the Queens
    chessboard[white_special][3] = new Queen(QUEEN, WHITE);
    chessboard[black_special][3] = new Queen(QUEEN, BLACK);

    //create the Kings
    chessboard[white_special][4] = new King(KING, WHITE);
    chessboard[black_special][4] = new King(KING, BLACK);

    cout << "A new chess game is started!" << endl;
}

void ChessBoard::commitMove(array<int,PAIR> const &pos, 
                            array<int,PAIR> const &mov, 
                            string const& position, string const& move,
                            ChessPiece* &holder) {

    //print the output statement
    colourPrint(chessboard[mov[ROW]][mov[COL]]->getColour());
    cout << "'s ";
    namePrint(chessboard[mov[ROW]][mov[COL]]->getName());
    cout << " moves from " << position << " to " << move;
    
    //print if castling
    if (isCastling(pos, mov)) {
        cout << " (castling)";
    }

    //print if piece taken
    if (holder != nullptr) {
        cout << " taking ";
        colourPrint(holder->getColour());
        cout << "'s "; 
        namePrint(holder->getName());
        delete holder;
        holder = nullptr;
    }

    //turn firstmove off
    chessboard[mov[ROW]][mov[COL]]->firstMoveOff();
    cout << endl;
}

void ChessBoard::movePiece(array<int,PAIR> const &pos, 
            array<int,PAIR> const &mov) {
    
    //move piece from position to move
    chessboard[mov[ROW]][mov[COL]] = chessboard[pos[ROW]][pos[COL]];
    chessboard[pos[ROW]][pos[COL]] = nullptr;
}

void ChessBoard::stringToArray(string const& position, 
                                    array<int,PAIR> &array) {
    array[ROW] = position[1] - '1';
    array[COL] = position[0] - 'A';
}

bool const ChessBoard::checkValidCoordinate(string const& coordinate) {
    if (((coordinate[0] >= 'A') && (coordinate[0] <= 'H')) &&
        ((coordinate[1] >= '1') && (coordinate[1] <= '8')) &&
        (coordinate.length() == 2)) {
            return true;
    }
    cout << coordinate << " is not a valid position" << endl;
    return false;
}

bool const ChessBoard::isColourTurn(char pieceColour) {
    if (turn == pieceColour) {
        return true;
    }
    return false;
}

void ChessBoard::updateAggressiveMap(char colour) {

    //clear the old Aggressive Map
    aggrMov.clear();

    //reset all protection in pieces to false
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((chessboard[row][col] != nullptr) &&
                (chessboard[row][col]->getColour() == colour)) {
                chessboard[row][col]->setProtected(false);
            }
        }
    }

    //iterate through board and add all aggressive moves to vector
    //this should also update for all protection
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((chessboard[row][col] != nullptr) &&
                (chessboard[row][col]->getColour() == colour)) {
                chessboard[row][col]
                    ->aggressiveMoves(row, col, aggrMov, chessboard);
            }
        }
    }
}

bool const ChessBoard::checkIfGameOver(char colour) {

    //create vector of arrays of possible moves per piece and
    //char representing opponent colour
    vector<array<int,PAIR>> possMov;
    array<int,PAIR> position, move;
    char opponent;
    (colour == BLACK) ? opponent = WHITE : opponent = BLACK;

    //iterate through board and add all aggressive moves to vector
    //this should also update for all protection
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if ((chessboard[row][col] != nullptr) &&
                (chessboard[row][col]->getColour() == colour)) {

                //clear the possMov vector for each new piece
                possMov.clear();

                //add all possible moves from piece to vector
                chessboard[row][col]
                    ->possibleMoves(row, col, possMov, chessboard, aggrMov);

                //for each possible move, try it and see if 
                //King still stays in check
                for (auto i = 0u; i < possMov.size(); i++) {
                    position[ROW] = row;
                    position[COL] = col;
                    move[ROW] = possMov[i][ROW];
                    move[COL] = possMov[i][COL];
  
                    ChessPiece* holder = chessboard[move[ROW]][move[COL]];
                    movePiece(position, move);
                    updateAggressiveMap(opponent);

                    //if not in check, move piece back and return false
                    if (!isInCheck(colour)) {
                        movePiece(move, position);
                        chessboard[move[ROW]][move[COL]] = holder;
                        updateAggressiveMap(opponent);
                        holder = nullptr;
                        return false;
                    }

                    //if in check, move piece back and iterate onto next piece
                    movePiece(move, position);
                    chessboard[move[ROW]][move[COL]] = holder;
                    updateAggressiveMap(opponent);
                    holder = nullptr;
                }
            }
        }
    }

    //if whole for loop finishes and no return, game is Over
    gameLive = false;
    return true;
}

bool const ChessBoard::isInCheck(char colour) {
    
    //tracks while loops until king is found
    bool stillLooking = true;

    int row = 0, col = 0;

    //search for king position
    while ((stillLooking) && (row < BOARD_SIZE)) {
        
        //reset col to 0 each time row is incremented
        col = 0;

        while ((stillLooking) && (col < BOARD_SIZE)) {   
            if ((chessboard[row][col] != nullptr) &&
                (chessboard[row][col]->getName() == KING) &&
                (chessboard[row][col]->getColour() == colour)) {
                
                stillLooking = false;
                
                //decrement needed as when exiting while loop
                //col and row will be incremented a last time 
                col--;
                row--;
            }
            col++;
        }
        row++;
    }

    //check if the opponent king is in aggr set
    for (auto i = 0u; i < aggrMov.size(); i++) {
        if ((aggrMov[i][ROW] == row) &&
            (aggrMov[i][COL] == col)) {
            return true;
        }
    }
    return false;
}

bool const ChessBoard::isCastling(array<int,PAIR> const &pos, 
                        array<int,PAIR> const &mov) {
    
    int const count = mov[COL] - pos[COL];
    int const direct = count/abs(count);

    //if piece that moved is king and moved two places, return true
    //and move castle to appropriate position
    if ((chessboard[mov[ROW]][mov[COL]] != nullptr) &&
        (chessboard[mov[ROW]][mov[COL]]->getName() == KING) &&
        (abs(count) == 2)) {
        
        //move the rook to the appropriate side of King
        if (direct < 0) {
            chessboard[mov[ROW]][mov[COL]-direct] = chessboard[mov[ROW]][0];
            chessboard[pos[ROW]][0] = nullptr;
        } else {
            chessboard[mov[ROW]][mov[COL]-direct] 
                                    = chessboard[pos[ROW]][BOARD_SIZE-1];
            chessboard[pos[ROW]][BOARD_SIZE-1] = nullptr;
        }
        return true;
    }
    return false;
}

void ChessBoard::colourPrint(char colour) {
    switch(colour) {
        case BLACK: cout << "Black"; break;
        case WHITE: cout << "White"; break;
    }
}

void ChessBoard::namePrint(char name) {
    switch(name) {
        case PAWN: cout << "Pawn"; break;
        case BISHOP: cout << "Bishop"; break;
        case HORSE: cout << "Knight"; break;
        case ROOK: cout << "Rook"; break;
        case KING: cout << "King"; break;
        case QUEEN: cout << "Queen"; break;
    }
}

void ChessBoard::printLine(int first_col, int second_col, int row) {
    
    cout << row + 1 << " |";

    //print top line with square colour
    for (int i = 0u; i < BOARD_SIZE; i++) {
        if (i%2 == 0) {
            cout << " " << static_cast<char>(first_col) << "    |";        
        } else {
            cout << " " << static_cast<char>(second_col) << "    |";
        }
    }
    cout << endl;

    //print second line with pieces 
    cout << "  |";
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << "   ";
        if (chessboard[row][i] != nullptr) {
            cout << chessboard[row][i]->getColour();
            cout << chessboard[row][i]->getName();
        } else {
            cout << "  ";
        }
        cout << " |";        
    }

    cout << endl;
    cout << "  +";

    //print line underneath
    for (int i = 0u; i < BOARD_SIZE; i++) {
        cout << "------+";
    }

    cout << endl;
}

void ChessBoard::printBoard() {
    
    //print top letter tracker
    cout << endl << "   ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << "   " << static_cast<char>('A'+i) << "   ";
    }
    
    cout << endl;
    cout << "  +";
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << "------+";
    }
    
    cout << endl;

    //print board lines
    for (int i = BOARD_SIZE-1; i >= 0; i--) {
        if (i%2 == 0) {
            printLine(BLACK, WHITE, i);
        } else {
            printLine(WHITE, BLACK, i);            
        }
    }
    
    cout << endl;
}

ChessBoard::~ChessBoard() {
    deletePieces();
}