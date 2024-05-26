#include "fen.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"

int main()
{
    string fen = "2k5/8/1pp1p3/8/8/3PPPPP/8/3K4 w - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m(v.return_board(), 0);
    cout << evaluate_pawn_structure(reverseBoard(v.return_board()), m.return_controlSquares(), m.return_oppControlSquares(), 0, fen);
}