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