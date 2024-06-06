#include "fen.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"
#include "sq_tables_eval.h"

int main()
{
    string fen = "3r4/1Pk5/8/8/2K5/8/3p4/1R6 w - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    init_tables();
    cout << eval_sq_tables(v.return_board());
}