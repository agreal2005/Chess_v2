#include "eval.h"

double evaluate_checkmate(vector<vector<char>> &board, vector<vector<vector<char>>> &attacked_squares, vector<vector<vector<char>>> &attack_squares, bool turn, string FEN)
{
    if(turn == 0)
    {
        if(attack_squares.size() == 0)
        {
            int xking, yking;
            // check if stalemate
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(board[i][j] == 'K')
                    {
                        xking = i;
                        yking = j;
                    }
                }
            }

            if(attacked_squares[xking][yking].size() == 0)
            {
                return 0.00;
            }
            else
            {
                return -inf;
            }
        }
    }
    else
    {
        if(attack_squares.size() == 0)
        {
            int xking, yking;
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(board[i][j] == 'k')
                    {
                        xking = i;
                        yking = j;
                    }
                }
            }

            if(attacked_squares[xking][yking].size() == 0)
            {
                return 0.00;
            }
            else
            {
                return inf;
            }
        }
    }
}
