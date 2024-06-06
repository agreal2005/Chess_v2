#include "moves.h"
#include "fen.h"
#include "pieces.h"
#include "sq_tables_eval.h"
#define inf 20000

#pragma once

double evaluate_checkmate(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &oppcontrol_squares,
                          const vector<string> &valid_moves, bool turn, string FEN);

double evaluate_material(const vector<vector<char>> &board);

double evaluate_pawn_structure(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares,
                               const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn, string FEN);

bool checkPawn(int col, int row, bool turn, const vector<vector<char>> &board, bool dir);


double evaluate_outposts(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn);
double hanging_piece_penalty(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn);
double weaker_attacked_penalty(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn);

// short getStage(int material_count);
double mobility(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn, bool enpassant,string epsquare,int castling);
double pieces_eval(const vector<vector<char>> &board, const vector<Piece> pieces, const vector<Piece> oppPieces, bool turn = 0);

