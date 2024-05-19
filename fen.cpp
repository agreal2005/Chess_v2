#include "fen.h"
#include "functions.h"
Board_FEN::Board_FEN()
{
    default_FEN();
}
Board_FEN::Board_FEN(string fen_val)
{
    int cursor = 0;
    vector<vector<char>> inp_board(8, vector<char> (8, '.'));
    for (int i=0; i<8; ++i)
    {
        int row = 0; // j value to move along the row
        while (fen_val[cursor]!='/' || fen_val[cursor]!=' ')
        {
            if (fen_val[cursor]-'0' >= 1 && fen_val[cursor]-'0' <= 7)
            {
                int gap = fen_val[cursor] - '0';
                row += gap;
                cursor++;
            }
            if (row >= 8)
            {
                // Invalid FEN
                default_FEN();
                return;
            }
            if (valid_piece_code(fen_val[cursor]))
            {
                board[i][row] = fen_val[cursor];
                cursor++;
                row++;
            }
            else
            {
                // Invalid FEN
                default_FEN();
                return;
            }
        }
    }
}
void Board_FEN::default_FEN()
{
    isEnPassant = false;
    white_castle_kingside = true;
    white_castle_queenside = true;
    black_castle_kingside = true;
    black_castle_queenside = true;
    turn = 0;
    halfmove_clock = 0;
    fullmoves = 1;
    // lowercase letters for black pieces
    // uppercase for white pieces
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.clear();
    for (int i=0; i<8; ++i)
    {
        vector<char> rank; // row vector
        for (int j=0; j<8; ++j)
        {
            if (i == 0 || i == 1 || i == 6 || i == 7)
            {
                rank.push_back(fen[j]);
            }
            else rank.push_back('.'); // '.' will denote unoccupied square
        }
        board.push_back(rank);
    }
}