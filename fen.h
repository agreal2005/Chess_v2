#include <iostream>
#include <vector>
#include <set>
#include <map>
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
    void default_FEN();
};