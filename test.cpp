#include "functions.h"
#include "fen.h"
#include "moves.h"
#include "eval.h"

int main()
{
    string fen = "1k5R/6R1/8/8/8/8/8/K7 b - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m(v.return_board(), 1);
    cout << evaluate_checkmate(v.return_board(), m.return_oppControlSquares(), m.valid_Moves(), 1, fen);
}