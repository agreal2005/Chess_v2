#include "fen.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"

int main()
{
    string fen = "8/1k6/2p1p3/3n1n2/4B3/3P1P2/8/2K5 w - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m(v.return_board(), 1);
    auto a = m.return_controlSquares();
    cout << evaluate_outposts(reverseBoard(v.return_board()), m.return_controlSquares(), m.return_oppControlSquares(), 1);
}