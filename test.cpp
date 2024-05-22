#include "functions.h"
#include "fen.h"
#include "moves.h"
int main()
{
    Board_FEN v("r1bqkbnr/pppp1ppp/8/8/2PpP3/8/PP3PPP/RNBQKB1R b KQkq c3 0 5");
    v.display_board_FEN();
    Moves m(v.return_board(), 0);
    vector<vector<vector<char>>> csq = m.return_controlSquares();
    for (int i=0; i<8; ++i)
    {
        for (int j=0; j<8; ++j)
        {
            if (!csq[i][j].empty()) {
                cout << ijs(i,j) << ": ";
                for (auto piece : csq[i][j]) cout << piece << ' ';
                cout << endl;
                getchar();
            }
        }
    }
    return 0;
}