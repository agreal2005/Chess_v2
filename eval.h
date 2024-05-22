#include "moves.h"
#include "fen.h"
#define inf 20000

double evaluate(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_checkmate(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_material(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_pawn_structure(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_pieces_eval(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_outposts(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_mobility(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn,string FEN);
double evaluate_centre_control(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_connectivity(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_king_safety(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);
double evaluate_tempo(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN);