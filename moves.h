#include "functions.h"
#include "pieces.h"
#pragma once
class Moves
{
    private:
    pair<int,int> king_square;
    pair<int,int> oppKing_square;
    bool wck, wcq, bck, bcq;
    vector<vector<char>> board;
    vector<Piece> pieces; // own pieces
    vector<Piece> oppPieces; // opp pieces
    vector<vector<vector<Piece>>> controlSquares;
    vector<vector<vector<Piece>>> oppControlSquares;
    bool turn;
    bool isEnPassant; // If True, then store the EnPassant attack square in controlSquares
    string epSquare;
    public:
    Moves();
    Moves(vector<vector<char>> board, bool turn, bool isEnPassant = false, string epSquare = "", int castling = 0);
    void fetch_Moves(vector<vector<char>> board, bool turn, bool isEnPassant = false, string epSquare = "", int castling = 0); // Fetch the board and the turn of the player
    vector<string> valid_Moves(); // It is assumed that illegal positions + wrong player to move are not given
    vector<vector<vector<Piece>>> return_controlSquares();
    vector<vector<vector<Piece>>> return_oppControlSquares();
    void checkOppPinned();
    void checkPinned();
    vector<Piece> return_pieces() { return pieces;}
    vector<Piece> return_oppPieces() { return oppPieces;}
    /*
        Note: controlSquares and oppCS are computed in the
        following order: Q, R, B, N, P, K
    */
};