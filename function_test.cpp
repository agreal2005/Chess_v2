#include "functions.h"
using namespace std;

int main()
{
    string fen = "rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1";
    string og_move = "dxe4";
    bool turn = 0;
    Board_FEN fiend(fen);
    string converted_move = stdToV2(fen, og_move);
    cout << og_move << " :: ";
    cout << converted_move << endl;
}