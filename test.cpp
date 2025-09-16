#include "gamestate.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"
#include "search.h"
#include <chrono>
#include <cassert>
using namespace std;
using namespace std::chrono;

int gamePhase;
psTables pst;
extern map<int, map<string, pair<string, double>*>> MasterMap;
extern map<int, vector<pair<string, double>*>> matMap;
int material = 78;

void gameTesting(string fen)
{
    GameState gs(fen);    
    EvalBar eb(fen);
    while (1)
    {
        gs.display_board_FEN();
        string str;
        cout << "Enter your move: " << endl;
        cin >> str;
        if (str == "exit") break;
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif


        auto board = gs.return_board();
        str = convertToMv(str, board, 0);
        // cout<<str<<endl;
        

        gs.input_FEN(gs.simulateOneMove(str));
        int n_mat = get_material(gs.return_board());
        if (n_mat < material && !matMap[material].empty())
        {
            for (auto it : matMap[material]) delete(it);
            matMap.erase(material);
        }
        material = n_mat;
        string changed_str = gs.get_FEN();
        cout << changed_str << endl;
        auto start = high_resolution_clock::now();
        int depth = 5;
        // if (get_material(v.return_board()) <= 20) depth = 4; 
        // pair<string, double> p = lesgo.evalTree(changed_str, depth, 0);
        pair<string, double> p = eb.NewEvalTree(changed_str, depth, 0, -(inf+100), inf+100);
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
        gs.input_FEN(gs.simulateOneMove(p.first));
        n_mat = get_material(gs.return_board());
        if (n_mat < material && !matMap[material].empty())
        {
            for (auto it : matMap[material]) delete(it);
            matMap.erase(material);
        }
        material = n_mat;
        auto duration = duration_cast<seconds>(stop - start);
        cout << "Executed in " << duration.count() << " seconds." << endl;
        pair <string,double> breakmate = eb.evalTree(gs.get_FEN(), 1);
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
    gs.display_board_FEN();
}

void positionTesting(string fen)
{
    GameState v(fen);
    v.display_board_FEN();
    /* Position Testing*/
    Moves m;
    bool turn = v.return_turn();
    vector<vector<char>> brd = v.return_board();
    pst.init_tables();
    m.setupPinAndControl(brd, turn, v.return_ep(), v.return_eps(), v.castle_options());
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

void convertMoveTesting(){
    vector<vector<char>> brd;
    brd = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','p','p','p','p','p'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','P','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'P','P','P','P','.','P','P','P'},
        {'R','N','B','Q','K','B','N','R'},
    };
    assert(convertToMv("Na3", brd, 0) == "Nb1a3");
    assert(convertToMv("Bc4", brd, 0) == "Bf1c4");
    
    brd = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','.','p','p','p','p'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','p','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','N','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','.','B','Q','K','B','N','R'},
    };
    assert(convertToMv("Nxd5", brd, 0) == "Nc3Xd5");

    brd = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','.','p','p','p','.'},
        {'.','.','.','.','.','.','.','p'},
        {'.','.','.','p','.','.','.','.'},
        {'.','.','.','.','.','N','.','.'},
        {'.','.','N','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','.','B','Q','K','B','.','R'},
    };
    assert(convertToMv("Ncxd5", brd, 0) == "Nc3Xd5");
    assert(convertToMv("Nfxd5", brd, 0) == "Nf4Xd5");
    assert(convertToMv("N3xd5", brd, 0) == "Nc3Xd5");
    assert(convertToMv("N4xd5", brd, 0) == "Nf4Xd5");

    brd = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','p','p','p','p','.'},
        {'.','.','.','.','.','.','.','p'},
        {'.','.','N','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','N','.','.','.','.','.'},
        {'P','P','P','P','P','P','P','P'},
        {'R','.','B','Q','K','B','.','R'},
    };
    assert(convertToMv("N3e4", brd, 0) == "Nc3e4");
    assert(convertToMv("N5e4", brd, 0) == "Nc5e4");

    brd = {
        {'r','n','b','q','k','b','n','r'},
        {'p','p','p','p','.','.','p','p'},
        {'.','.','.','.','p','p','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','B','.','P','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','P','P','P','.','P','P','P'},
        {'R','N','B','Q','K','.','N','R'},
    };
    assert(convertToMv("Bxe6", brd, 0) == "Bc4Xe6");
    assert(convertToMv("Ra6", brd, 0) == "Ra1a6");
    assert(convertToMv("Rxa7", brd, 0) == "Ra1Xa7");

    brd = {
        {'Q','.','Q','.','.','.','.','.'},
        {'.','p','.','.','.','.','.','.'},
        {'Q','.','Q','.','.','.','.','.'},
        {'.','.','.','p','.','R','.','.'},
        {'.','.','.','.','.','.','K','p'},
        {'.','B','.','.','.','.','.','.'},
        {'.','.','.','R','.','.','B','.'},
        {'.','.','.','.','.','.','.','.'},
    };
    assert(convertToMv("Rdxd5", brd, 0) == "Rd2Xd5");
    assert(convertToMv("R2xd5", brd, 0) == "Rd2Xd5");
    assert(convertToMv("Rfxd5", brd, 0) == "Rf5Xd5");
    assert(convertToMv("R5xd5", brd, 0) == "Rf5Xd5");
    assert(convertToMv("R5f2", brd, 0) == "Rf5f2");
    assert(convertToMv("B3xd5", brd, 0) == "Bb3Xd5");
    assert(convertToMv("Qa8xb7", brd, 0) == "Qa8Xb7");
    assert(convertToMv("Kxh4", brd, 0) == "Kg4Xh4");

    brd = {
        {'.','.','.','.','r','.','.','.'},
        {'.','.','.','P','.','P','.','.'},
        {'.','.','p','.','.','.','.','.'},
        {'.','P','p','P','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'P','.','P','.','P','.','P','P'},
        {'.','.','.','.','.','.','.','.'},
    };
    assert(convertToMv("e4", brd, 0) == "Pe2Ze4");
    assert(convertToMv("h3", brd, 0) == "Ph2h3");
    assert(convertToMv("f8=Q", brd, 0) == "Pf7f8Q");
    assert(convertToMv("f8=Q", brd, 0) == "Pf7f8Q");
    assert(convertToMv("bxc6 e.p.", brd, 0) == "Pb5Yc6");
    assert(convertToMv("dxc6", brd, 0) == "Pd5Xc6");
}

void playOneNewMoveTesting() {
    string fen;
    getline(cin, fen);
    while (fen != "exit") {
        GameState gs(fen); 
        gs.display_board_FEN();
        cout << "Your move: ";
        string mv;
        cin >> mv;
        auto board = gs.return_board();
        mv = convertToMv(mv, board, 0);
        cout << mv << endl;
        getchar();
        // cout<<mv<<endl;
        gs.input_FEN(gs.simulateOneMove(mv));
        string changed_str = gs.get_FEN();
        cout << changed_str << endl;
        gs.display_board_FEN();
        getline(cin, fen);
    }
}

int main()
{
    convertMoveTesting();

    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; /* Starting position */
    // string fen = "8/1k6/8/8/8/8/1K6/2RR4 w - - 0 1"; /* Endgame Position */
    // string fen = "r1bqkbnr/pppppppp/8/8/3nP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 1"; /* Middlegame position*/
    // getline(cin, fen);
    // Board_FEN v(fen);
    // v.display_board_FEN();
    // gameTesting(fen);
    playOneNewMoveTesting();
    // gameTesting("2k5/5P2/3K4/8/8/8/8/8 w - - 0 1");
    // positionTesting(fen);
    // positionTesting("2k5/5P2/3K4/8/8/8/8/8 w - - 0 1");
    // positionTesting("rnbqk1nr/p1pp1ppp/1p2p3/8/1b1PP3/2P2N2/PP3PPP/RNBQKB1R b KQkq - 0 1");
    // positionTesting("1rb2k1r/ppNpnpp1/3N1q2/7p/4P3/P3B3/1PPQ1PP1/R4K2 b - - 0 1");
}