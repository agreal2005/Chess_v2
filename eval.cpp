#include "eval.h"
#define pawn_score 1
#define bishop_score 3
#define knight_score 3
#define rook_score 5
#define queen_score 9

double evaluate_checkmate(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &oppcontrol_squares, const vector<string> &legal_moves, bool turn, string FEN)
{
    if(turn == 0)
    {
        if(legal_moves.size() == 0)
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

            if(oppcontrol_squares[xking][yking].size() == 0)
            {
                return 0.00;
            }
            else
            {
                return -inf;
            }
        }
        else
        {
            return 0.00;
        }
    }
    else
    {
        if(legal_moves.size() == 0)
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

            if(oppcontrol_squares[xking][yking].size() == 0)
            {
                return 0.00;
            }
            else
            {
                return inf;
            }
        }
        else
        {
            return 0.00;
        }
    }
}

double evaluate_material(const vector<vector<char>> &board)
{
    double white_score = 0;
    double black_score = 0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            switch (board[i][j])
            {
            case 'P':
                white_score += pawn_score;
                break;
            case 'N':
                white_score += knight_score;
                break;
            case 'R':
                white_score += rook_score;
                break;
            case 'B':
                white_score += bishop_score;
                break;
            case 'Q':
                white_score += queen_score;
                break;
            
            case 'p':
                black_score += pawn_score;
                break;
            case 'n':
                black_score += knight_score;
                break;
            case 'r':
                black_score += rook_score;
                break;
            case 'b':
                black_score += bishop_score;
                break;
            case 'q':
                black_score += queen_score;
                break;
            
            default:
                break;
            }
        }
    }

    return white_score - black_score;
}

// This checks whether there is a pawn in the given column after or behind a given row for both white and black depending on dir
// dir 1 means ahead or equal to the row of whichever side you are playing
bool checkPawn(int col, int row, bool turn, const vector<vector<char>> &board, bool dir)
{
    if(col < 0 || col > 7)
    {
        return false;
    }

    if(turn == 0)
    {
        if(dir == 1)
       { 
        for(int i=row; i<8; i++)
        {
            if(board[i][col] == 'P')
            {
                return true;
            }
        }
        return false; 
        }
        else
        {
            for(int i=0; i<=row; i++)
            {
                if(board[i][col] == 'P')
                {
                    return true;
                }
            }
            return false;
        }
    }
    else
    {
       if(dir == 1)
       {
          for(int i=0; i<=row; i++)
          {
            if(board[i][col] == 'p')
            {
                return true;
            }
          }
          return false;
       }
       else
       {
          for(int i=row; i<8; i++)
          {
            if(board[i][col] == 'p')
            {
                return true;
            }
          }
          return false;
       }
    }
}

bool CheckPasser(const vector<vector<char>> &board, int row, int col, int turn)
{
    if(turn == 0)
    {
        bool check_black_pawn = checkPawn(col, row+1, 1, board, 0) | checkPawn(col-1, row+1, 1, board, 0) | checkPawn(col+1, row+1, 1, board, 0);
        return 1-check_black_pawn;
    }
    else
    {
        bool check_white_pawn = checkPawn(col, row-1, 0, board, 0) | checkPawn(col-1, row-1, 0, board, 0) | checkPawn(col+1, row-1, 0, board, 0);
        return 1-check_white_pawn;
    }
}

bool CheckDoublePawn(const vector<vector<char>> &board, int row, int col, int turn)
{
    if(turn == 0)
    {
        for(int i=0; i<8; i++)
        {
            if(i != row  && board[i][col] == 'P')
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            if(i != row && board[i][col] == 'p')
            {
                return true;
            }
        }
        return false;
    }
}

double evaluate_pawn_structure(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn, string FEN){
    // doubled pawn penalty is made half of what it should be because it will be double counted, tripled pawn will be interpreted as a double doubled pawn
    double doubled_pawn_penalty = 0.3; // checked
    double isolated_pawn_penalty = 0.3; //checked
    double pawn_chain_bonus = 0.2; // checked
    double pawn_passer_bonus = 0.15; // checked

    double white_score = 0;
    double black_score = 0;

    // while computing this it is calculated from white's POV

if(turn == 0){
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(board[i][j] == 'P')
            {
                // accounting for pawn chains
                for(auto i : control_squares[i][j])
                {
                    if(i.type == 'P')
                    {
                        white_score += pawn_chain_bonus;
                    }
                }

                if((checkPawn(j-1, 0, 0, board, 1) | checkPawn(j+1, 0, 0, board, 1)) == false)
                {
                    white_score -= isolated_pawn_penalty;
                }

                if(CheckPasser(board, i, j, 0) == true)
                {
                    white_score += pawn_passer_bonus*(i);
                }

                if(CheckDoublePawn(board, i, j, 0) == true)
                {
                    white_score -= doubled_pawn_penalty;
                }
            }

            else if(board[i][j] == 'p')
            {
                for(auto i : oppcontrol_squares[i][j])
                {
                    if(i.type == 'p')
                    {
                        black_score += pawn_chain_bonus;
                    }
                }

                if((checkPawn(j-1, 0, 1, board, 0) | checkPawn(j+1, 0, 1, board, 0)) == false)
                {
                    black_score -= isolated_pawn_penalty;
                }

                if(CheckPasser(board, i, j, 1) == true)
                {
                    black_score += pawn_passer_bonus*(7-i);
                }

                if(CheckDoublePawn(board, i, j, 1) == true)
                {
                    black_score -= doubled_pawn_penalty;
                }
                
            }
        }
    }
}
else
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(board[i][j] == 'P')
            {
                // accounting for pawn chains
                for(auto i : oppcontrol_squares[i][j])
                {
                    if(i.type == 'P')
                    {
                        white_score += pawn_chain_bonus;
                    }
                }

                if((checkPawn(j-1, 0, 0, board, 1) | checkPawn(j+1, 0, 0, board, 1)) == false)
                {
                    white_score -= isolated_pawn_penalty;
                }

                if(CheckPasser(board, i, j, 0) == true)
                {
                    white_score += pawn_passer_bonus;
                }

                if(CheckDoublePawn(board, i, j, 0) == true)
                {
                    white_score -= doubled_pawn_penalty;
                }
            }

            else if(board[i][j] == 'p')
            {
                for(auto i : control_squares[i][j])
                {
                    if(i.type == 'p')
                    {
                        black_score += pawn_chain_bonus;
                    }
                }

                if((checkPawn(j-1, 0, 1, board, 0) | checkPawn(j+1, 0, 1, board, 0)) == false)
                {
                    black_score -= isolated_pawn_penalty;
                }

                if(CheckPasser(board, i, j, 1) == true)
                {
                    black_score += pawn_passer_bonus;
                }

                if(CheckDoublePawn(board, i, j, 1) == true)
                {
                    black_score -= doubled_pawn_penalty;
                }
                
            }
        }
    }
}

//    cout << white_score << ' ' << black_score << ' ';
    return white_score-black_score;
}
