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