#include "fen.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"
#include "search.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

int gamePhase;
psTables pst;

int main()
{
    // string fen = "2r5/1P1pkp1p/b4p2/p3pB2/4P3/P3B1P1/2PK3P/1R6 b - - 0 1";
    string fen = "6r1/7P/1k1K4/8/8/8/8/8 w - - 0 1";
    // getline(cin, fen);
    Board_FEN v(fen);
    EvalBar lesgo(fen);
    while (1)
    {
        v.display_board_FEN();
        int cas_opt = v.castle_options();
        string str;
        cout << "Enter your move: " << endl;
        cin >> str;
        if (str == "exit") break;
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        v.input_FEN(lesgo.playOneMove(str, v.return_board(),v.return_turn(),((cas_opt&8)!=0),((cas_opt&4)!=0),((cas_opt&2)!=0),((cas_opt&1)!=0),v.return_ep(),v.return_eps(),v.return_halfmoveclk(),v.return_fullmoves()));
        cout << v.get_FEN() << endl;
        string changed_str = v.get_FEN();
        auto start = high_resolution_clock::now();
        pair<string, double> p = lesgo.evalTree(changed_str, 3);
        auto stop = high_resolution_clock::now();
        cout << "Computer's move: " << p.first << endl;
        cout << "Eval: " << p.second << endl;
        v.input_FEN(lesgo.playOneMove(p.first, v.return_board(),v.return_turn(),((cas_opt&8)!=0),((cas_opt&4)!=0),((cas_opt&2)!=0),((cas_opt&1)!=0),v.return_ep(),v.return_eps(),v.return_halfmoveclk(),v.return_fullmoves()));
        auto duration = duration_cast<seconds>(stop - start);
        cout << "Executed in " << duration.count() << " seconds." << endl;
        if (p.second == inf || p.second == -inf) {
            cout << "CHECKMATE!" << endl;
            break;
        }
        if (p.first == "-")
        {
            cout << "STALEMATE!" << endl;
            break;
        }
    }
    v.display_board_FEN();
    // Moves m;
    // bool turn = v.return_turn();
    // vector<vector<char>> brd = v.return_board();
    // m.fetch_Moves(brd, turn, v.return_ep(), v.return_eps(), v.castle_options());
    // cout << setw(60) << left << "Evaluate Checkmate: " << evaluate_checkmate(v.return_board(), m.return_oppControlSquares(), m.valid_Moves(), turn, fen) << endl;
    // cout << setw(60) << left << "Evaluate Material: " << evaluate_material(v.return_board()) << endl;
    // cout << setw(60) << left << "Pawn Structure: " << evaluate_pawn_structure(reverseBoard(v.return_board()), m.return_controlSquares(), m.return_oppControlSquares(), turn, fen) << endl;
    // cout << setw(60) << left << "Evaluate Outposts: " << evaluate_outposts(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    // cout << setw(60) << left << "Hanging Piece Penalty: " << hanging_piece_penalty(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    // cout << setw(60) << left << "Weaker Attacked Penalty: " << weaker_attacked_penalty(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    // // cout << setw(60) << left << "Mobility: " << mobility(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn, v.return_ep(), v.return_eps(), v.castle_options()) << endl;
    // cout << setw(60) << left << "Piece Square Evaluation: " << pst.eval_sq_tables(v.return_board()) << endl;
    // cout << setw(60) << left << "King safety" << eval_kingsafety(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    // cout << setw(60) << left << "(Special) Pieces eval: " << pieces_eval(v.return_board(), m.return_pieces(), m.return_oppPieces(), turn) << endl;
    // cout << "----> Valid Moves of Player: " << endl;
    // for (auto str : m.valid_Moves()) cout << str << endl;
    // cout << "----> Valid Moves of Opponent: " << endl;
    // for (auto str : m.valid_oppMoves()) cout << str << endl;
    // cout << "-----> Trapped pieces (if any):" << endl;
    // for (auto str : m.return_trappedPieces()) cout << str.type << endl;
    // cout << "-----> For opponent too, if any:" << endl;
    // for (auto str : m.return_trappedOppPieces()) cout << str.type << endl;
    // EvalBar bar;
    // int cs = v.castle_options();
    // for (auto mv : m.valid_Moves())
    // {
    //     getchar();
    //     cout << bar.playOneMove(mv, v.return_board(), v.return_turn(), ((cs>>3)&1), ((cs>>2)&1), ((cs>>1)&1), (cs&1), v.return_ep(), v.return_eps(), v.return_halfmoveclk(), v.return_fullmoves()) << endl;
    // }
}