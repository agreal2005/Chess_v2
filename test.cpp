#include "functions.h"
#include "fen.h"
#include "moves.h"
int main()
{
    string fen = "1R6/8/8/2p5/1b6/1k6/8/1K6 b - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m(v.return_board(), 1);
    vector<string> vms = m.valid_Moves();
    for (auto str : vms) cout << str << endl;
    return 0;
}