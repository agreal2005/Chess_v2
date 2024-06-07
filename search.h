#include "eval.h"
#pragma once

class EvalBar
{
    private:
    Board_FEN fen; // Contains info about the board, turn, ep Square, Castling, etc.
    Moves m; // Contains info about pieces controlling squares, valid Moves, etc.
    const int depth = 10;
    
    public:
    EvalBar(string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    double complete_eval(); // To be called when eval reaches its depth
    //
    // make a minimax function
    //
};