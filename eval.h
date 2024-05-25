#include "moves.h"
#include "fen.h"
#include "pieces.h"
#define inf 20000

#pragma once

double evaluate_checkmate(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &oppcontrol_squares, const vector<string> &valid_moves, bool turn, string FEN);
double evaluate_material(const vector<vector<char>> &board);
double evaluate_pawn_structure(vector<vector<char>> &board, vector<vector<vector<char>>> &control_squares, vector<vector<vector<char>>> &oppcontrol_squares, bool turn, string FEN);
