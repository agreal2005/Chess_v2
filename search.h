#include "eval.h"
#pragma once

struct EvalParams
{
    string f; // fen
    vector<vector<char>> board; // from fen.h
    vector<Piece> pieces; // from moves.h
    vector<Piece> oppPieces; // from moves.h
    vector<vector<vector<Piece>>> controlSquares; // from moves.h
    vector<vector<vector<Piece>>> oppControlSquares; // from moves.h
    bool turn, isEnPassant; // from fen.h
    int castling; // from fen.h
    string epSquare; // from fen.h
    vector<Piece> trappedPieces; // from moves.h
    vector<Piece> trappedOppPieces; // from moves.h
    vector<string> validMoves; // from moves.h
    vector<string> validOppMoves; // from moves.h
};

class EvalBar
{
    private:
    Board_FEN fen; // Contains info about the board, turn, ep Square, Castling, etc.
    Moves m; // Contains info about pieces controlling squares, valid Moves, etc.
    const int depth = 5;
    
    public:
    EvalBar(string f = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    string playOneMove(string move, vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms); // Return FEN
    /*
        Assuming string move is always a valid legal move
        Update all the boolean params
        if valid Move is enPassant, then capture that pawn also
        and see if after Valid move, there is en passant
        Castling also has to be updated
        Turn also has to be updated
    */
    pair<string, double> evalTree(string f, int d, bool isOpp = false);  // d = depth, f = fen, isOpp = is it Opponent's turn or not
    // First take the fen 'f' and send it to playOneMove
    // Retrieve the parameters in the function
    // Make the vectors for pieces, oppPieces, controlSquares, oppControlSquares, validMoves, validOppMoves
    // See fen.h & moves.h for reference
    // pass everything to complete_eval including isOpp
    double complete_eval(EvalParams pr, bool isOpp = false); // To be called when eval reaches its depth
    
};