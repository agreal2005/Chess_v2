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
    vector<vector<char>> inp_board(8, vector<char>(8, '.'));
    for (int i = 0; i < 8; ++i)
    {
        int row = 0; // j value to move along the row
        while (fen_val[cursor] != '/' && fen_val[cursor] != ' ')
        {
            if (fen_val[cursor] - '0' >= 1 && fen_val[cursor] - '0' <= 8)
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
    if (fen_val[cursor] != ' ')
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
        for (int i = 0; i < 4; ++i)
        {
            if (fen_val[cursor] == ' ')
            {
                break;
            }
            switch (fen_val[cursor])
            {
            case 'K':
                white_castle_kingside = true;
                break;
            case 'Q':
                white_castle_queenside = true;
                break;
            case 'k':
                black_castle_kingside = true;
                break;
            case 'q':
                black_castle_queenside = true;
                break;
            default:
                // Invalid FEN
                default_FEN();
                return;
            }
            cursor++;
        }
    }
    if (fen_val[cursor] != ' ')
    {
        // Invalid FEN
        default_FEN();
        return;
    }
    cursor++;
    if (fen_val[cursor] == '-')
    {
        isEnPassant = false;
        epSquare = "";
    }
    else
    {
        if (valid_file(fen_val[cursor]) && (fen_val[cursor + 1] == '3' || fen_val[cursor + 1] == '6'))
        {
            isEnPassant = true;
            epSquare = fen_val[cursor];
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
    if (fen_val[cursor] != ' ')
    {
        // Invalid FEN
        default_FEN();
        return;
    }
    cursor++;
    int hfm = 0; // half move clock
    while (fen_val[cursor] != ' ')
    {
        if (fen_val[cursor] - '0' >= 0 && fen_val[cursor] - '0' <= 9)
        {
            hfm = 10 * hfm + (fen_val[cursor] - '0');
        }
        cursor++;
    }
    halfmove_clock = hfm;
    cursor++;
    hfm = 0; // Using this for full moves
    while (fen_val[cursor] != ' ')
    {
        if (fen_val[cursor] - '0' >= 0 && fen_val[cursor] - '0' <= 9)
        {
            hfm = 10 * hfm + (fen_val[cursor] - '0');
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
    epSquare = "";
    halfmove_clock = 0;
    fullmoves = 1;
    // lowercase letters for black pieces
    // uppercase for white pieces
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    board.clear();
    for (int i = 0; i < 8; ++i)
    {
        vector<char> rank; // row vector
        for (int j = 0; j < 8; ++j)
        {
            if (i == 0 || i == 1 || i == 6 || i == 7)
            {
                rank.push_back(fen[j]);
            }
            else
                rank.push_back('.'); // '.' will denote unoccupied square
        }
        board.push_back(rank);
    }
}
vector<vector<char>> Board_FEN::return_board()
{
    return board;
}
bool Board_FEN::return_turn()
{
    return turn;
}
void Board_FEN::display_board_FEN()
{
    display_board(board);
}
bool Board_FEN::return_ep()
{
    return isEnPassant;
}
string Board_FEN::return_eps()
{
    return epSquare;
}
int Board_FEN::castle_options()
{
    /*
        ret has 4 bits
        MSB to LSB represent if the following castling
        options are true (possible) or not:
        -> WCK, WCQ, BCK, BCQ
    */
    int ret = 0;
    if (white_castle_kingside)
        ret |= 8;
    if (white_castle_queenside)
        ret |= 4;
    if (black_castle_kingside)
        ret |= 2;
    if (black_castle_queenside)
        ret |= 1;
    return ret;
}
int Board_FEN::return_halfmoveclk()
{
    return halfmove_clock;
}
int Board_FEN::return_fullmoves()
{
    return fullmoves;
}
string Board_FEN::get_FEN()
{
    string str = "";
    for (int i = 0; i < 8; ++i)
    {
        int gap = 0;
        for (int j = 0; j < 8; ++j)
        {
            if (board[i][j] == '.')
            {
                gap++;
            }
            else
            {
                if (gap > 0)
                {
                    str.push_back('0' + gap);
                    gap = 0;
                }
                str.push_back(board[i][j]);
            }
        }
        if (gap > 0)
        {
            str.push_back('0' + gap);
            gap = 0;
        }
        if (i < 7)
        {
            str.push_back('/');
        }
    }
    str.push_back(' ');
    if (turn == 0)
        str.push_back('w');
    else
        str.push_back('b');
    str.push_back(' ');
    if (white_castle_kingside)
        str.push_back('K');
    if (white_castle_queenside)
        str.push_back('Q');
    if (black_castle_kingside)
        str.push_back('k');
    if (black_castle_queenside)
        str.push_back('q');
    if (!(white_castle_kingside || black_castle_kingside || white_castle_queenside || black_castle_queenside))
        str.push_back('-');
    str.push_back(' ');
    if (!isEnPassant)
        str.push_back('-');
    else
        str += epSquare;
    str.push_back(' ');
    str += to_string(halfmove_clock);
    str.push_back(' ');
    str += to_string(fullmoves);
    return str;
}
string Board_FEN::get_FEN(vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms)
{
    string str = "";
    for (int i = 0; i < 8; ++i)
    {
        int gap = 0;
        for (int j = 0; j < 8; ++j)
        {
            if (brd[i][j] == '.')
            {
                gap++;
            }
            else
            {
                if (gap > 0)
                {
                    str.push_back('0' + gap);
                    gap = 0;
                }
                str.push_back(brd[i][j]);
            }
        }
        if (gap > 0)
        {
            str.push_back('0' + gap);
            gap = 0;
        }
        if (i < 7)
        {
            str.push_back('/');
        }
    }
    str.push_back(' ');
    if (t == 0)
        str.push_back('w');
    else
        str.push_back('b');
    str.push_back(' ');
    if (wck)
        str.push_back('K');
    if (wcq)
        str.push_back('Q');
    if (bck)
        str.push_back('k');
    if (bcq)
        str.push_back('q');
    if (!(bcq || bck || wcq || wck))
        str.push_back('-');
    str.push_back(' ');
    if (!isEnp)
        str.push_back('-');
    else
        str += epS;
    str.push_back(' ');
    str += to_string(hfc);
    str.push_back(' ');
    str += to_string(fms);
    return str;
}