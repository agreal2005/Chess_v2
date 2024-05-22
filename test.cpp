#include "functions.h"
#include "fen.h"
#include "moves.h"
int main()
{
    string fen = "r1bqkbnr/pppp1ppp/8/8/2PpP3/8/PP3PPP/RNBQKB1R b KQkq c3 0 5";
    Board_FEN v(fen);
    cout << fen << endl;
    v.display_board_FEN();
    Moves m(v.return_board(), 0);
    cout << v.return_ep() << ' ' << v.return_eps() << ' ' << v.castle_options() << ' ' << v.return_halfmoveclk() << ' ' << v.return_fullmoves() << endl;
    cout << v.get_FEN() << endl;
    cout << (v.get_FEN() == fen) << endl;
    return 0;
}