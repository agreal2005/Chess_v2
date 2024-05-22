#include <iostream>
using namespace std;
#pragma once

class Piece
{
    public:
    char type;
    int i, j;
    bool pinned;
    int pini, pinj; // (i,j) of pinning piece
    Piece(char ch = '.', int i = 0, int j = 0, bool pinned = 0, int pini = 0, int pinj = 0)
    {
        this->i = i;
        this->j = j;
        type = ch;
        this->pinned = pinned;
        this->pini = pini;
        this->pinj = pinj;
    }
};