#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#pragma once
using namespace std;
const string BLANC = "                                                                    ";
/*
    Board Representation: Converting FEN to Board Matrix and vice-versa
*/
class Board_FEN
{
    private:
    string fen;
    bool isEnPassant;
    bool white_castle_queenside, black_castle_queenside;
    bool white_castle_kingside, black_castle_kingside;
    bool turn; // 0 = white to move, 1 = black to move
    int halfmove_clock; // # of half moves counted, it is incremented on every move that is
    // not a pawn advance, nor a capture of a piece
    // If halfmoves == 100 then the game result must be a draw
    int fullmoves; // Incremented on every move of Black
    // Denotes the Move number
    string epSquare; // En Passant Square
    vector<vector<char>> board; // Board Matrix

    public:
    Board_FEN();
    Board_FEN(string fen_val);
    void input_FEN(string fen_val);
    void default_FEN();
    void display_board_FEN();
    vector<vector<char>> return_board();
    bool return_ep();
    string return_eps();
    int castle_options();
    int return_halfmoveclk();
    int return_fullmoves();
    bool return_turn();
    string get_FEN();
    string get_FEN(vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms);
    /*
        Note: In future, need to make the Game Class
        (and other req classes) to be a friend class,
        to directly access the parameters.
    */

   friend class Moves;
};