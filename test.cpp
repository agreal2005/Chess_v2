#include "functions.h"
#include "fen.h"
int main()
{
    Board_FEN v("r1bqkbnr/pppp1ppp/8/8/2PpP3/8/PP3PPP/RNBQKB1R b KQkq c3 0 5");
    v.display_board_FEN();
    return 0;
}