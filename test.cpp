#include "fen.h"
#include "moves.h"
#include "eval.h"
#include "functions.h"
using namespace std;
int gamePhase;
psTables pst;

int main()
{
    string fen;
    getline(cin, fen);
    Board_FEN v(fen);
    v.display_board_FEN();
    Moves m;
    bool turn = v.return_turn();
    pst.init_tables();
    m.fetch_Moves(v.return_board(), 0);
    cout << setw(60) << left << "Evaluate Checkmate: " << evaluate_checkmate(v.return_board(), m.return_oppControlSquares(), m.valid_Moves(), turn, fen) << endl;
    cout << setw(60) << left << "Evalute Material: " << evaluate_material(v.return_board()) << endl;
    cout << setw(60) << left << "Pawn Structure: " << evaluate_pawn_structure(reverseBoard(v.return_board()), m.return_controlSquares(), m.return_oppControlSquares(), turn, fen) << endl;
    cout << setw(60) << left << "Evaluate Outposts: " << evaluate_outposts(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Hanging Piece Penalty: " << hanging_piece_penalty(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Weaker Attacked Penalty: " << weaker_attacked_penalty(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn) << endl;
    cout << setw(60) << left << "Mobility: " << mobility(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn, v.return_ep(), v.return_eps(), v.castle_options()) << endl;
    cout << setw(60) << left << "Piece Square Evaluation: " << pst.eval_sq_tables(v.return_board()) << endl;
    cout << setw(60) << left << "King safey" << eval_kingsafety(v.return_board(), m.return_controlSquares(), m.return_oppControlSquares(), turn);
    cout << setw(60) << left << "(Special) Pieces eval: " << pieces_eval(v.return_board(), m.return_pieces(), m.return_oppPieces(), turn) << endl;
    cout << "----> Valid Moves of Player: " << endl;
    for (auto str : m.valid_Moves()) cout << str << endl;
    cout << "----> Valid Moves of Opponent: " << endl;
    for (auto str : m.valid_oppMoves()) cout << str << endl;
    cout << "-----> Trapped pieces (if any):" << endl;
    for (auto str : m.return_trappedPieces()) cout << str.type << endl;
    cout << "-----> For opponent too, if any:" << endl;
    for (auto str : m.return_trappedOppPieces()) cout << str.type << endl;
}