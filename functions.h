#include <iostream>
#include <vector>
using namespace std;
#pragma once
/*
    Commonly used functions to be declared here
*/
void display_board(vector<vector<char>> &board)
{
    
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
        break;
        default:
        return false;
    }
}