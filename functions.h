#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;
#pragma once
/*
    Commonly used functions to be declared here
*/
void display_board(vector<vector<char>> &board);

bool valid_rank(char ch);

bool valid_file(char ch);

bool valid_piece_code(char ch);

string ijs(int i, int j); // Convert (i,j) to square name

pair<int,int> sij(string s); // Convert square name to (i, j)

int piece_type(char ch, bool turn);