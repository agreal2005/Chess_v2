#include "eval.h"
#pragma once

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
        Update all the boolean params
        if valid Move is enPassant, then capture that pawn also
        and see if after Valid move, there is en passant
        Castling also has to be updated
        Turn also has to be updated
    */
    pair<string, double> evalTree(string f, int d, bool isOpp = false); // First take the fen 'f' and send it to Dev
    double complete_eval(); // To be called when eval reaches its depth
    
};