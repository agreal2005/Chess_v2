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

void gameTesting(string fen)
{
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
        string changed_str = v.get_FEN();
        cout << changed_str << endl;
        auto start = high_resolution_clock::now();
        int depth = 5;
        // if (get_material(v.return_board()) <= 20) depth = 4; 
        // pair<string, double> p = lesgo.evalTree(changed_str, depth, 0);
        pair<string, double> p = lesgo.NewEvalTree(changed_str, depth, 0, -(inf+100), inf+100);
        auto stop = high_resolution_clock::now();
        if (p.first == "#")
        {
            cout << "CHECKMATE!" << endl;
            break;
        }
        else if (p.first == "-")
        {
            cout << "STALEMATE!" << endl;
            break;
        }
        cout << "Computer's move: " << p.first << endl;
        cout << "Eval: " << p.second << endl;
        cas_opt = v.castle_options();
        v.input_FEN(lesgo.playOneMove(p.first, v.return_board(),v.return_turn(),((cas_opt&8)!=0),((cas_opt&4)!=0),((cas_opt&2)!=0),((cas_opt&1)!=0),v.return_ep(),v.return_eps(),v.return_halfmoveclk(),v.return_fullmoves()));
        auto duration = duration_cast<seconds>(stop - start);
        cout << "Executed in " << duration.count() << " seconds." << endl;
        pair <string,double> breakmate = lesgo.evalTree(v.get_FEN(), 1);
        if (breakmate.first == "#")
        {
            cout << "CHECKMATE!" << endl;
            break;
        }
        else if (breakmate.first == "-")
        {
            cout << "STALEMATE!" << endl;
            break;
        }
    }
    v.display_board_FEN();
}

void positionTesting(string fen)
{
    Board_FEN v(fen);
    v.display_board_FEN();
    /* Position Testing*/
    Moves m;
    bool turn = v.return_turn();
    vector<vector<char>> brd = v.return_board();
    pst.init_tables();
    m.fetch_Moves(brd, turn, v.return_ep(), v.return_eps(), v.castle_options());
    cout << setw(60) << left << "Evaluate Checkmate: " << evaluate_checkmate(v.return_board(), m.return_oppControlSquares(), m.valid_Moves(), turn, fen) << endl;
    cout << setw(60) << left << "Evaluate Material: " << evaluate_material(v.return_board()) << endl;
    cout << setw(60) << left << "Pawn Structure: " << evaluate_pawn_structure(reverseBoard(v.return_board()), m.return_controlSquares(), m.return_oppControlSquares(), turn, fen) << endl;
    cout << setw(60) << left << "Evaluate Outposts: " << evaluate_outposts(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Hanging Piece Penalty: " << hanging_piece_penalty(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Weaker Attacked Penalty: " << weaker_attacked_penalty(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Mobility: " << mobility(brd, m.return_controlSquares(), m.return_oppControlSquares(), m.valid_Moves(), m.valid_oppMoves(), turn, v.return_ep(), v.return_eps(), v.castle_options()) << endl;
    cout << setw(60) << left << "Piece Square Evaluation: " << (1/625.0)*(double)pst.eval_sq_tables(v.return_board()) << endl;
    cout << setw(60) << left << "King safety" << eval_kingsafety(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Trapped Pieces Eval: " << trapped_eval(m.white_trapped_pieces, m.black_trapped_pieces) << endl;
    cout << setw(60) << left << "(Special) Pieces eval: " << pieces_eval(v.return_board(), m.return_pieces(), m.return_oppPieces(), turn) << endl;
    cout << "----> Valid Moves of Player: " << endl;
    for (auto str : m.valid_Moves()) cout << str << endl;
    cout << "----> Valid Moves of Opponent: " << endl;
    for (auto str : m.valid_oppMoves()) cout << str << endl;
    cout << "-----> Trapped pieces (if any):" << endl;
    for (auto str : m.return_trappedPieces()) cout << str.type << endl;
    cout << "-----> For opponent too, if any:" << endl;
    for (auto str : m.return_trappedOppPieces()) cout << str.type << endl;
    EvalBar bar;
    int cs = v.castle_options();
    pair<string, double> p = bar.NewEvalTree(fen, 5, 0, -(inf+100), inf+100);
    cout << p.second << " " << p.first << endl;
    // p = bar.evalTree(fen, 4, 0);
    // cout << "Old eval: " << p.second << " " << p.first << endl;
}

int main()
{
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; /* Starting position */
    // string fen = "8/1k6/8/8/8/8/1K6/2RR4 w - - 0 1"; /* Endgame Position */
    // string fen = "r1bqkbnr/pppppppp/8/8/3nP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 1"; /* Middlegame position*/
    // getline(cin, fen);
    // Board_FEN v(fen);
    // v.display_board_FEN();
    // gameTesting(fen);
    gameTesting("2k5/5P2/3K4/8/8/8/8/8 w - - 0 1");
    // positionTesting(fen);
    // positionTesting("2k5/5P2/3K4/8/8/8/8/8 w - - 0 1");
    // positionTesting("rnbqk1nr/p1pp1ppp/1p2p3/8/1b1PP3/2P2N2/PP3PPP/RNBQKB1R b KQkq - 0 1");
    // positionTesting("1rb2k1r/ppNpnpp1/3N1q2/7p/4P3/P3B3/1PPQ1PP1/R4K2 b - - 0 1");
}