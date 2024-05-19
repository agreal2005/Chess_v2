#include "fen.h"
#include "functions.h"
Board_FEN::Board_FEN()
{
    default_FEN();
}
Board_FEN::Board_FEN(string fen_val)
{
    input_FEN(fen_val);
}
void Board_FEN::input_FEN(string fen_val)
{
    fen_val += BLANC; // Need this for (seamless) fen validation
    int cursor = 0;
    vector<vector<char>> inp_board(8, vector<char> (8, '.'));
    for (int i=0; i<8; ++i)
    {
        int row = 0; // j value to move along the row
        while (fen_val[cursor]!='/' && fen_val[cursor]!=' ')
        {
            if (fen_val[cursor]-'0' >= 1 && fen_val[cursor]-'0' <= 8)
            {
                int gap = fen_val[cursor] - '0';
                row += gap;
                cursor++;
                continue;
            }
            if (row >= 8)
            {
                // Invalid FEN
                default_FEN();
                return;
            }
            if (valid_piece_code(fen_val[cursor]))
            {
                inp_board[i][row] = fen_val[cursor];
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
        cursor++;
    }
    board = inp_board;
    if (fen_val[cursor] == 'w')
    {
        turn = 0;
    }
    else if (fen_val[cursor] == 'b')
    {
        turn = 1;
    }
    else
    {
        // Invalid FEN
        default_FEN();
        return;
    }
    white_castle_kingside = true;
    white_castle_queenside = true;
    black_castle_kingside = true;
    black_castle_queenside = true;
    cursor++;
    if (fen_val[cursor]!=' ')
    {
        // Invalid FEN
        default_FEN();
        return;
    }
    cursor++;
    if (fen_val[cursor] == '-')
    {
        cursor++;
    }
    else
    {
        for (int i=0; i<4; ++i)
        {
            if (fen_val[cursor] == ' ')
            {
                break;
            }
            switch(fen_val[cursor])
            {
                case 'K':
                white_castle_kingside = true;
                break;
                case 'Q':
                white_castle_queenside = true;
                break;
                case 'k':
                black_castle_kingside = false;
                break;
                case 'q':
                black_castle_queenside = false;
                break;
                default:
                // Invalid FEN
                default_FEN();
                return;
            }
            cursor++;
        }
    }
    if (fen_val[cursor]!=' ')
    {
        // Invalid FEN
        default_FEN();
        return;
    }
    cursor++;
    if (fen_val[cursor] == '-')
    {
        isEnPassant = false;
    }
    else
    {
        if (valid_file(fen_val[cursor]) && (fen_val[cursor+1] == '3' || fen_val[cursor+1] == '6'))
        {
            isEnPassant = true;
            epSquare=fen_val[cursor];
            epSquare.push_back(fen_val[++cursor]);
        }
        else
        {
            // Invalid FEN
            default_FEN();
            return;
        }
    }
    cursor++;
    if (fen_val[cursor]!=' ')
    {
        // Invalid FEN
        default_FEN();
        return;
    }
    cursor++;
    int hfm = 0; // half move clock
    while (fen_val[cursor]!=' ')
    {
        if (fen_val[cursor]-'0'>=0 && fen_val[cursor]-'0'<=9)
        {
            hfm = 10*hfm + (fen_val[cursor] - '0');
        }
        cursor++;
    }
    halfmove_clock = hfm;
    cursor++;
    hfm = 0; // Using this for full moves
    while (fen_val[cursor]!=' ')
    {
        if (fen_val[cursor]-'0'>=0 && fen_val[cursor]-'0'<=9)
        {
            hfm = 10*hfm + (fen_val[cursor] - '0');
        }
        cursor++;
    }
    fullmoves = hfm;
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
vector<vector<char>> Board_FEN::return_board()
{
    return board;
}
void Board_FEN::display_board_FEN()
{
    display_board(board);
}