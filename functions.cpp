#include "functions.h"
void display_board(vector<vector<char>> &board)
{
    for (int i=0; i<40; ++i)
    {
        if (i%5 == 0) cout << "+";
        else cout << "-";
    }
    cout << endl;
    for (int i=0; i<8; ++i)
    {
        for (int j=0; j<8; ++j)
        {
            cout << '|' << setw(2) << board[i][j] << setw(2) << '|'; 
        }
        cout << endl;
        for (int i=0; i<40; ++i)
        {
            if (i%5 == 0) cout << "+";
            else cout << "-";
        }
        cout << endl;
    }
}
bool valid_rank(char ch)
{
    if (ch-'0' >= 1 && ch - '0' <= 8)
    {
        return true;
    }
    return false;
}
bool valid_file(char ch)
{
    switch(ch)
    {
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
        case 'g': case 'h':
        return true;
        default:
        return false;
    }
}
bool valid_piece_code(char ch)
{
    switch (ch)
    {
        case 'p': case 'P':
        case 'r': case 'R':
        case 'n': case 'N':
        case 'b': case 'B':
        case 'q': case 'Q':
        case 'k': case 'K':
        return true;
        default:
        return false;
    }
}

string ijs(int i, int j)
{
    string squares[8][8] = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    };
    return squares[i][j];
}

pair<int,int> sij(string s)
{
    int j = s[0] - 'a';
    int i = ('8' - s[1]);
    return {i,j};
}

int piece_type(char ch, bool turn)
{
    if (ch == '.') return 0;
    if (turn == 0)
    {
        if (ch >= 'a' && ch <= 'z') return -1;
        else return 1;
    }
    if (turn == 1 && ch>='a' && ch <= 'z') return 1;
    return -1;
}

vector<vector<char>> reverseBoard(const vector<vector<char>> &board)
{
    vector<vector<char>> rev(8, vector<char>(8));
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            rev[i][j] = board[7-i][j];
        }
    }
    return rev;
}

int get_material(const vector<vector<char>> &board)
{
    int white_score = 0;
    int black_score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (board[i][j])
            {
            case 'P':
                white_score += 1;
                break;
            case 'N':
                white_score += 3;
                break;
            case 'R':
                white_score += 5;
                break;
            case 'B':
                white_score += 3;
                break;
            case 'Q':
                white_score += 9;
                break;

            case 'p':
                black_score += 1;
                break;
            case 'n':
                black_score += 3;
                break;
            case 'r':
                black_score += 5;
                break;
            case 'b':
                black_score += 3;
                break;
            case 'q':
                black_score += 9;
                break;

            default:
                break;
            }
        }
    }
    return white_score + black_score;
}