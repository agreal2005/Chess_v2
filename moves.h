#include "functions.h"
class Moves
{
    private:
    vector<vector<char>> board;
    vector<vector<char>> attackSquares;
    vector<vector<char>> attackedSquares;
    bool turn;
    bool isEnPassant; // If True, then store the EnPassant attack square in attackSquares
    string epSquare;
    public:
    Moves();
    Moves(vector<vector<char>> board, bool turn, bool isEnPassant = false, string epSquare = "");
    void fetch_Moves(vector<vector<char>> board, bool turn, bool isEnPassant = false, string epSquare = ""); // Fetch the board and the turn of the player
    void compute_Squares(); // Compute attack and attacked Squares
    vector<vector<char>> return_attackSquares();
    vector<vector<char>> return_attackedSquares();
};