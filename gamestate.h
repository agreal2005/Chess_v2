 #include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#pragma once
using namespace std;
/*
    Board Representation: Converting FEN to Board Matrix and vice-versa
*/
class GameState
{
    private:
    string fen;
    string pos; // Board encoded in string
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
    GameState();
    GameState(string fen_val);
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
    string getPos();
    string get_FEN();

    // Assumes string move is always a valid legal move
    // Does not update any parameter, only returns FEN
    // if move was played
    string simulateOneMove(string& move);

    /*
        Note: In future, need to make the Game Class
        (and other req classes) to be a friend class,
        to directly access the parameters.
    */

   friend class Moves;
   friend class EvalBar;
   friend struct EvalParams;
};