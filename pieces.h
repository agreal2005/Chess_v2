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
    int resi, resj; // (i,j) of restricting piece
    Piece(char ch = '.', int i = 0, int j = 0, bool pinned = 0, int pini = -1, int pinj = -1, int resi = -1, int resj = -1)
    {
        this->i = i;
        this->j = j;
        type = ch;
        this->pinned = pinned;
        this->pini = pini;
        this->pinj = pinj;
        this->resi = resi;
        this->resj = resj;
    }
    void setPin(int x, int y)
    {
        pinned = true;
        pini = x;
        pinj = y;
        resi = -1;
        resj = -1;
    }
    void setRes(int x, int y)
    {
        pinned = true;
        pini = -1;
        pinj = -1;
        resi = x;
        resj = y;
    }
};