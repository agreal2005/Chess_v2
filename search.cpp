#include "search.h"

EvalBar::EvalBar(string f)
{
    fen.input_FEN(f);
    m.fetch_Moves(fen.board, fen.turn, fen.isEnPassant, fen.epSquare, fen.castle_options());
}

double EvalBar::complete_eval()
{
    /*
        eval = sum of (weightage of an eval param*eval of the param)
    */
    double eval = 0.0;
    //---------------To be completed------------------------------/
    /*-----------------------------------------------------------*/
    return eval;
}

// Make functions for complete evaluation and Game Search Tree here