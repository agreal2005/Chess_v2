#include "fen.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"

int main()
{
    string fen = "3r4/2k1p3/8/8/2K5/8/P7/1R6 w - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m;
    m.fetch_Moves(v.return_board(), 0);
    cout << pieces_eval(v.return_board(), m.return_pieces(), m.return_oppPieces(), 0);
}