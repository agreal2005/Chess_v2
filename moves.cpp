#include "moves.h"
Moves::Moves()
{
    for (int i=0; i<8; ++i)
    {
        vector<char> initialize(8, '.');
        board.push_back(initialize);
    }
    for (int i=0; i<8; ++i)
    {
        vector<int> initialize(8, 0);
        attackSquares.push_back(initialize);
    }
    attackedSquares=attackSquares;
    turn = 0;
}
Moves::Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare)
{
    fetch_Moves(board, turn, isEnPassant, epSquare);
}
void Moves::fetch_Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare)
{
    this->board = board;
    this->turn = turn;
    this->isEnPassant = isEnPassant;
    this->epSquare = epSquare;
}
vector<vector<int>> Moves::return_attackSquares()
{   
    
    return attackSquares;
}
vector<vector<int>> Moves::return_attackedSquares()
{
    return attackedSquares;
}
void Moves::compute_Squares()
{
    /*
        First compute the attack Squares, and if isEnPassant is true, factor that into account.
        Then compute the attacked Squares (i.e. the attack squares of the opponent) in this position.
    */
}
