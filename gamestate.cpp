#include "gamestate.h"
#include "functions.h"

void destroy_fen(string fen_val)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    cout << "INVALID FEN:\n";
    cout << fen_val << endl;
    abort();
}

GameState::GameState()
{
    default_FEN();
}

GameState::GameState(string fen_val)
{
    input_FEN(fen_val);
}

void GameState::input_FEN(string fen_val)
{
    fen_val += string(68, ' '); // End padding to prevent seg fault for small invalid FENs
    int cursor = 0;
    vector<vector<char>> inp_board(8, vector<char>(8, '.'));
    for (auto ch:fen_val)
    {
        if (ch == ' ') break;
        pos.push_back(ch);
    }
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
                destroy_fen(fen_val);
                // default_FEN();
                // return;
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
                destroy_fen(fen_val);
                // default_FEN();
                // return;
            }
        }
        if (row!=8)
        {
            // Invalid FEN
            destroy_fen(fen_val);
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
        destroy_fen(fen_val);
        // default_FEN();
        // return;
    }
    white_castle_kingside = false;
    white_castle_queenside = false;
    black_castle_kingside = false;
    black_castle_queenside = false;
    cursor++;
    if (fen_val[cursor] != ' ')
    {
        // Invalid FEN
        destroy_fen(fen_val);
        // default_FEN();
        // return;
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
                destroy_fen(fen_val);
                // default_FEN();
                // return;
            }
            cursor++;
        }
    }
    if (fen_val[cursor] != ' ')
    {
        // Invalid FEN
        destroy_fen(fen_val);
        // default_FEN();
        // return;
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
            destroy_fen(fen_val);
            // default_FEN();
            // return;
        }
    }
    cursor++;
    if (fen_val[cursor] != ' ')
    {
        // Invalid FEN
        destroy_fen(fen_val);
        // default_FEN();
        // return;
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
void GameState::default_FEN()
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
    pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
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
vector<vector<char>> GameState::return_board()
{
    return board;
}
bool GameState::return_turn()
{
    return turn;
}
void GameState::display_board_FEN()
{
    display_board(board);
}
bool GameState::return_ep()
{
    return isEnPassant;
}
string GameState::return_eps()
{
    return epSquare;
}
int GameState::castle_options()
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
int GameState::return_halfmoveclk()
{
    return halfmove_clock;
}
int GameState::return_fullmoves()
{
    return fullmoves;
}
string GameState::getPos()
{
    return pos;
}
string GameState::get_FEN()
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

string GameState::simulateOneMove(string& move)
{
    vector<vector<char>> brd = return_board();
    bool t = return_turn();
    int cas_opt = castle_options();
    bool wck = (cas_opt&8)!=0;
    bool wcq = (cas_opt&4)!=0;
    bool bck = (cas_opt&2)!=0;
    bool bcq = (cas_opt&1)!=0;
    bool isEnp = return_ep();
    string epS = return_eps();
    int hfc = return_halfmoveclk();
    int fms = return_fullmoves();


    string curr_position = move.substr(1, 2);
    string next_position = move.substr(move.length() - 2, 2);
    pair<int,int> curr_ij, next_ij;
    if (move[0] != 'o' && move[0] != 'O') curr_ij = sij(curr_position);
    if (move[0] != 'o' && move[0] != 'O') next_ij = sij(next_position);
    if (move == "O-O") // white castling king side
    {
        brd[7][4] = '.';
        brd[7][7] = '.';
        brd[7][5] = 'R';
        brd[7][6] = 'K';
        wck = false;
        wcq = false;
        isEnp = false;
    }
    else if (move == "O-O-O") // white castling queen side
    {
        brd[7][3] = 'R';
        brd[7][2] = 'K';
        brd[7][0] = '.';
        brd[7][1] = '.';
        brd[7][4] = '.';
        wck = false;
        wcq = false;
        isEnp = false;
    }
    else if (move == "o-o") // black castling king side
    {
        brd[0][5] = 'r';
        brd[0][6] = 'k';
        brd[0][4] = '.';
        brd[0][7] = '.';
        bck = false;
        bcq = false;
        isEnp = false;
    }
    else if (move == "o-o-o") // black castling queen side
    {
        brd[0][3] = 'r';
        brd[0][2] = 'k';
        brd[0][1] = '.';
        brd[0][0] = '.';
        brd[0][4] = '.';
        bck = false;
        bcq = false;
        isEnp = false;
    }
    else if (move[0] == 'P' && (move.back() == 'Q' || move.back() == 'R' || move.back() == 'B' || move.back() == 'N')) // Promotion
    {
        brd[curr_ij.first][curr_ij.second] = '.';
        next_ij = sij(move.substr(move.length() - 3, 2));
        brd[0][next_ij.second] = move.back();
    }
    else if (move[0] == 'p' && (move.back() == 'q' || move.back() == 'r' || move.back() == 'b' || move.back() == 'n')) // Promotion
    {
        brd[curr_ij.first][curr_ij.second] = '.';
        next_ij = sij(move.substr(move.length() - 3, 2));
        brd[7][next_ij.second] = move.back();
    }
    else if (move[0] == 'P' || move[0] == 'p')
    {
        if (isEnp && (move[3] == 'y' || move[3] == 'Y'))
        {
            string passed_sq;
            if (move[0] == 'P')
            {
                passed_sq = ijs(sij(epS).first + 1, sij(epS).second);
            }
            else
            {
                passed_sq = ijs(sij(epS).first - 1, sij(epS).second);
            }
            brd[sij(passed_sq).first][sij(passed_sq).second] = '.';
            brd[curr_ij.first][curr_ij.second] = '.';
            brd[next_ij.first][next_ij.second] = move[0];
            isEnp = false;
        }
        else
        {
            brd[curr_ij.first][curr_ij.second] = '.';
            brd[next_ij.first][next_ij.second] = move[0];
            isEnp = false;
            if (move[3] == 'Z')
            {
                if ((next_ij.second - 1 >= 0 && brd[next_ij.first][next_ij.second - 1] == 'p') ||
                    (next_ij.second + 1 < 8 && brd[next_ij.first][next_ij.second + 1] == 'p'))
                {
                    isEnp = true;
                    epS = ijs(next_ij.first + 1, next_ij.second);
                }
            }
            else if(move[3]=='z')
            {
                if ((next_ij.second - 1 >= 0 && brd[next_ij.first][next_ij.second - 1] == 'P') ||
                    (next_ij.second + 1 < 8 && brd[next_ij.first][next_ij.second + 1] == 'P'))
                {
                    isEnp = true;
                    epS = ijs(next_ij.first - 1, next_ij.second);
                }
            }
        }
    }
    else
    {
        brd[curr_ij.first][curr_ij.second] = '.';
        brd[next_ij.first][next_ij.second] = move[0];
        isEnp = false;
        if (move[0] == 'K')
        {
            wck = 0; wcq = 0;
        }
        else if (move[0] == 'k')
        {
            bck = 0; bcq = 0;
        }
        else if (move[0] == 'R' && curr_position == "h1") wck = 0;
        else if (move[0] == 'R' && curr_position == "a1") wcq = 0;
        else if (move[0] == 'r' && curr_position == "h8") bck = 0;
        else if (move[0] == 'r' && curr_position == "a8") bcq = 0;
    }
    t = !t;

    // call the global get_FEN
    return ::get_FEN(brd, t, wck, wcq, bck, bcq, isEnp, epS, hfc, fms);
}
