#include "functions.h"
#include "fen.h"
#include "moves.h"
int main()
{
    string fen = "3r4/1k6/7B/8/6K1/4n1b1/8/8 w - - 0 1";
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m(v.return_board(), 0);
    vector<vector<vector<Piece>>> osq = m.return_oppControlSquares();
    vector<string> vms = m.valid_Moves();
    for (auto str : vms) cout << str << endl;
    // cout << "TEST" << endl;
    // getchar();
    // for (int i=0; i<8; ++i)
    // {
    //     for (int j=0; j<8; ++j)
    //     {
    //         if (osq[i][j].size() > 0)
    //         {
    //             cout << ijs(i,j) << ": ";
    //             for (auto piece : osq[i][j])
    //             {
    //                 cout << piece.type << " at " << ijs(piece.i, piece.j) << "; ";
    //             }
    //             cout << endl;
    //         }
    //     }
    // }
    return 0;
}