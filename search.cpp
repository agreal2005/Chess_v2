#include "search.h"

EvalBar::EvalBar(string f)
{
    fen.input_FEN(f);
    m.fetch_Moves(fen.board, fen.turn, fen.isEnPassant, fen.epSquare, fen.castle_options());
}

// Make functions for complete evaluation and Game Search Tree here

string EvalBar::playOneMove(string move, vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms)
{
    string curr_position = move.substr(1, 2);
    string next_position = move.substr(move.length() - 2, 2);
    pair<int,int> curr_ij, next_ij;
    if (move[0] != 'o' && move[0] != 'O') curr_ij = sij(curr_position);
    if (move[0] != 'o' && move[0] != 'O') next_ij = sij(next_position);
    if (move == "O-O") // white castling king side
    {
        brd[7][4] = 'R';
        brd[7][7] = 'K';
        wck = false;
        wcq = false;
        isEnp = false;
    }
    else if (move == "O-O-O") // black castling queen side
    {
        brd[7][4] = 'R';
        brd[7][0] = 'K';
        wck = false;
        wcq = false;
        isEnp = false;
    }
    else if (move == "o-o") // black castling king side
    {
        brd[0][4] = 'r';
        brd[0][7] = 'k';
        bck = false;
        bcq = false;
        isEnp = false;
    }
    else if (move == "o-o-o") // black castling queen side
    {
        brd[0][4] = 'r';
        brd[0][0] = 'k';
        bck = false;
        bcq = false;
        isEnp = false;
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
    }
    t = !t;
    display_board(brd); // for testing purposes
    return fen.get_FEN(brd, t, wck, wcq, bck, bcq, isEnp, epS, hfc, fms);
}

double complete_eval(EvalParams pr, bool isOpp = false)
{
    double eval = evaluate_material(pr.board) + evaluate_pawn_structure(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn, pr.f) + evaluate_outposts(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn)+
           hanging_piece_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn)+weaker_attacked_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn) + mobility(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    double king_score = eval_kingsafety(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    
    if(gamePhase > 24)
    {
        eval += king_score;
    }
    return eval;
}
