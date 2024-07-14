#include "eval.h"
#define pawn_score 1
#define bishop_score 3
#define knight_score 3
#define rook_score 5
#define queen_score 9

double evaluate_checkmate(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &oppcontrol_squares,
                          const vector<string> &legal_moves, bool turn, string FEN = "")
{
    if (turn == 0)
    {
        if (legal_moves.size() == 0)
        {
            int xking, yking;
            // check if stalemate
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (board[i][j] == 'K')
                    {
                        xking = i;
                        yking = j;
                    }
                }
            }

            if (oppcontrol_squares[xking][yking].size() == 0)
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
        if (legal_moves.size() == 0)
        {
            int xking, yking;
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (board[i][j] == 'k')
                    {
                        xking = i;
                        yking = j;
                    }
                }
            }

            if (oppcontrol_squares[xking][yking].size() == 0)
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
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
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

 int getStage(int total_material)
 {
     if (total_material >= 68) return 2; // opening
     else if (total_material <= 26) return 0; // endgame
     else return 1; // middlegame
 }

// This checks whether there is a pawn in the given column after or behind a given row for both white and black depending on dir
// dir 1 means ahead or equal to the row of whichever side you are playing
bool checkPawn(int col, int row, bool turn, const vector<vector<char>> &board, bool dir)
{
    if (col < 0 || col > 7)
    {
        return false;
    }

    if (turn == 0)
    {
        if (dir == 1)
        {
            for (int i = row; i < 8; i++)
            {
                if (board[i][col] == 'P')
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            for (int i = 0; i <= row; i++)
            {
                if (board[i][col] == 'P')
                {
                    return true;
                }
            }
            return false;
        }
    }
    else
    {
        if (dir == 1)
        {
            for (int i = 0; i <= row; i++)
            {
                if (board[i][col] == 'p')
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            for (int i = row; i < 8; i++)
            {
                if (board[i][col] == 'p')
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
    if (turn == 0)
    {
        bool check_black_pawn = checkPawn(col, row + 1, 1, board, 0) | checkPawn(col - 1, row + 1, 1, board, 0) | checkPawn(col + 1, row + 1, 1, board, 0);
        return 1 - check_black_pawn;
    }
    else
    {
        bool check_white_pawn = checkPawn(col, row - 1, 0, board, 0) | checkPawn(col - 1, row - 1, 0, board, 0) | checkPawn(col + 1, row - 1, 0, board, 0);
        return 1 - check_white_pawn;
    }
}

bool CheckDoublePawn(const vector<vector<char>> &board, int row, int col, int turn)
{
    if (turn == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            if (i != row && board[i][col] == 'P')
            {
                return true;
            }
        }
        return false;
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            if (i != row && board[i][col] == 'p')
            {
                return true;
            }
        }
        return false;
    }
}

double evaluate_pawn_structure(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn, string FEN)
{
    // doubled pawn penalty is made half of what it should be because it will be double counted, tripled pawn will be interpreted as a double doubled pawn
    double doubled_pawn_penalty = 0.3;  // checked 0.3
    double isolated_pawn_penalty = 0.3; // checked 0.3
    double pawn_chain_bonus = 0.2;      // checked 0.2
    double pawn_passer_bonus = 0.15;    // checked 0.15

    double white_score = 0;
    double black_score = 0;

    // while computing this it is calculated from white's POV

    if (turn == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j] == 'P')
                {
                    // accounting for pawn chains
                    for (auto piece : control_squares[7 - i][j])
                    {
                        //   cout << piece.type << ' ';
                        if (piece.type == 'P')
                        {
                            white_score += pawn_chain_bonus;
                        }
                    }

                    if ((checkPawn(j - 1, 0, 0, board, 1) | checkPawn(j + 1, 0, 0, board, 1)) == false)
                    {
                        white_score -= isolated_pawn_penalty;
                    }

                    if (CheckPasser(board, i, j, 0) == true)
                    {
                        white_score += pawn_passer_bonus * (i);
                    }

                    if (CheckDoublePawn(board, i, j, 0) == true)
                    {
                        white_score -= doubled_pawn_penalty;
                    }
                }

                else if (board[i][j] == 'p')
                {
                    for (auto piece : oppcontrol_squares[7 - i][j])
                    {
                        if (piece.type == 'p')
                        {
                            black_score += pawn_chain_bonus;
                        }
                    }

                    if ((checkPawn(j - 1, 0, 1, board, 0) | checkPawn(j + 1, 0, 1, board, 0)) == false)
                    {
                        black_score -= isolated_pawn_penalty;
                    }

                    if (CheckPasser(board, i, j, 1) == true)
                    {
                        black_score += pawn_passer_bonus * (7 - i);
                    }

                    if (CheckDoublePawn(board, i, j, 1) == true)
                    {
                        black_score -= doubled_pawn_penalty;
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (board[i][j] == 'P')
                {
                    // accounting for pawn chains
                    for (auto i : oppcontrol_squares[7 - i][j])
                    {
                        if (i.type == 'P')
                        {
                            white_score += pawn_chain_bonus;
                        }
                    }

                    if ((checkPawn(j - 1, 0, 0, board, 1) | checkPawn(j + 1, 0, 0, board, 1)) == false)
                    {
                        white_score -= isolated_pawn_penalty;
                    }

                    if (CheckPasser(board, i, j, 0) == true)
                    {
                        white_score += pawn_passer_bonus;
                    }

                    if (CheckDoublePawn(board, i, j, 0) == true)
                    {
                        white_score -= doubled_pawn_penalty;
                    }
                }

                else if (board[i][j] == 'p')
                {
                    for (auto i : control_squares[7 - i][j])
                    {
                        if (i.type == 'p')
                        {
                            black_score += pawn_chain_bonus;
                        }
                    }

                    if ((checkPawn(j - 1, 0, 1, board, 0) | checkPawn(j + 1, 0, 1, board, 0)) == false)
                    {
                        black_score -= isolated_pawn_penalty;
                    }

                    if (CheckPasser(board, i, j, 1) == true)
                    {
                        black_score += pawn_passer_bonus;
                    }

                    if (CheckDoublePawn(board, i, j, 1) == true)
                    {
                        black_score -= doubled_pawn_penalty;
                    }
                }
            }
        }
    }

    // cout << white_score << ' ' << black_score << ' ';
    return white_score - black_score;
}

double evaluate_outposts(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn)
{
    double white_score = 0;
    double black_score = 0;

    double knight_on_outpost = 0.3;
    double bishop_on_outpost = 0.2;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == 'N')
            {
                double outpost_score = 0;
                // evaluating for white outposts
                auto ptr = control_squares[7 - i][j];
                if (turn == 1)
                {
                    ptr = oppcontrol_squares[7 - i][j];
                }
                for (auto piece : ptr)
                {
                    if (piece.type == 'P')
                    {
                        outpost_score += knight_on_outpost;
                    }
                }

                if (checkPawn(j + 1, i + 1, 1, board, 0) == true)
                {
                    outpost_score -= knight_on_outpost;
                }

                if (checkPawn(j - 1, i + 1, 1, board, 0) == true)
                {
                    outpost_score -= knight_on_outpost;
                }

                white_score += max(0.0, outpost_score);
            }

            else if (board[i][j] == 'B')
            {
                double outpost_score = 0;
                auto ptr = control_squares[7 - i][j];
                if (turn == 1)
                {
                    ptr = oppcontrol_squares[7 - i][j];
                }
                // evaluating for white outposts
                for (auto piece : ptr)
                {
                    if (piece.type == 'P')
                    {
                        outpost_score += bishop_on_outpost;
                    }
                }

                if (checkPawn(j + 1, i + 1, 1, board, 0) == true)
                {
                    outpost_score -= bishop_on_outpost;
                }

                if (checkPawn(j - 1, i + 1, 1, board, 0) == true)
                {
                    outpost_score -= bishop_on_outpost;
                }

                white_score += max(0.0, outpost_score);
            }

            else if (board[i][j] == 'n')
            {
                double outpost_score = 0;
                auto ptr = oppcontrol_squares[7 - i][j];
                if (turn == 1)
                {
                    ptr = control_squares[7 - i][j];
                }
                for (auto piece : ptr)
                {
                    if (piece.type == 'p')
                    {
                        outpost_score += knight_on_outpost;
                    }
                }

                if (checkPawn(j + 1, i - 1, 0, board, 0) == true)
                {
                    outpost_score -= knight_on_outpost;
                }

                if (checkPawn(j - 1, i - 1, 0, board, 0) == true)
                {
                    outpost_score -= knight_on_outpost;
                }

                black_score += max(0.0, outpost_score);
            }

            else if (board[i][j] == 'b')
            {
                double outpost_score = 0;
                auto ptr = oppcontrol_squares[7 - i][j];
                if (turn == 1)
                {
                    ptr = control_squares[7 - i][j];
                }
                for (auto piece : ptr)
                {
                    if (piece.type == 'p')
                    {
                        outpost_score += bishop_on_outpost;
                    }
                }

                if (checkPawn(j + 1, i - 1, 0, board, 0) == true)
                {
                    outpost_score -= bishop_on_outpost;
                }

                if (checkPawn(j - 1, i - 1, 0, board, 0) == true)
                {
                    outpost_score -= bishop_on_outpost;
                }

                black_score += max(0.0, outpost_score);
            }
        }
    }
    return white_score - black_score;
}

double hanging_piece_penalty(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn)
{
    double total_penalty = 0.0;
    vector<double> white_penalty;
    vector<double> black_penalty;

    for (int in_ver = 0; in_ver < 8; in_ver++)
    {
        for (int in_hor = 0; in_hor < 8; in_hor++)
        {
            if (board[in_ver][in_hor] >= 'A' && board[in_ver][in_hor] <= 'Z')
            {
                if ((turn ^ control_squares[in_ver][in_hor].empty()) && !(turn ^ oppcontrol_squares[in_ver][in_hor].empty()))
                {
                    switch (board[in_ver][in_hor])
                    {
                    case 'P':
                        white_penalty.push_back(0.8);
                        break;
                    case 'B':
                        white_penalty.push_back(2.4);
                        break;
                    case 'N':
                        white_penalty.push_back(2.4);
                        break;
                    case 'R':
                        white_penalty.push_back(4.0);
                        break;
                    case 'Q':
                        white_penalty.push_back(7.2);
                        break;
                    default:
                        break;
                    }
                }
            }
            if (board[in_ver][in_hor] >= 'a' && board[in_ver][in_hor] <= 'z')
            {
                if (!(turn ^ control_squares[in_ver][in_hor].empty()) && (turn ^ oppcontrol_squares[in_ver][in_hor].empty()))
                {
                    switch (board[in_ver][in_hor])
                    {
                    case 'p':
                        black_penalty.push_back(0.8);
                        break;
                    case 'b':
                        black_penalty.push_back(2.4);
                        break;
                    case 'n':
                        black_penalty.push_back(2.4);
                        break;
                    case 'r':
                        black_penalty.push_back(4.0);
                        break;
                    case 'q':
                        black_penalty.push_back(7.2);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    sort(white_penalty.begin(), white_penalty.end(), greater<double>());
    sort(black_penalty.begin(), black_penalty.end(), greater<double>());
    if (turn)
    {
        for (int i = 0; i < white_penalty.size(); i++) // i+=2
            total_penalty += white_penalty[i];
        for (int i = 0; i < black_penalty.size(); i++) // i=1, i+=2
            total_penalty -= black_penalty[i];
    }
    else
    {
        for (int i = 0; i < white_penalty.size(); i++)
            total_penalty += white_penalty[i];
        for (int i = 0; i < black_penalty.size(); i++)
            total_penalty -= black_penalty[i];
    }

    return -total_penalty;
}

double weaker_attacked_penalty(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn)
{
    double total_penalty = 0.0;
    vector<double> white_penalty;
    vector<double> black_penalty;

    for (int in_ver = 0; in_ver < 8; in_ver++)
    {
        for (int in_hor = 0; in_hor < 8; in_hor++)
        {
            if (board[in_ver][in_hor] >= 'A' && board[in_ver][in_hor] <= 'Z')
            {

                int this_piece_val = 0;
                switch (board[in_ver][in_hor])
                {
                case 'P':
                    this_piece_val = 1;
                    break;
                case 'B':
                    this_piece_val = 3;
                    break;
                case 'N':
                    this_piece_val = 3;
                    break;
                case 'R':
                    this_piece_val = 5;
                    break;
                case 'Q':
                    this_piece_val = 9;
                    break;
                default:
                    break;
                }

                int opp_piece_val = 9;
                vector<Piece> vec_of_opp_pieces;
                if (turn)
                    vec_of_opp_pieces = control_squares[in_ver][in_hor];
                else
                    vec_of_opp_pieces = oppcontrol_squares[in_ver][in_hor];

                for (auto it : vec_of_opp_pieces)
                {
                    if (turn == 0 && oppcontrol_squares[it.i][it.j].size() == 0) continue;
                    if (turn == 1 && control_squares[it.i][it.j].size() == 0) continue;
                    switch (it.type)
                    {
                    case 'p':
                        opp_piece_val = min(opp_piece_val, 1);
                        break;
                    case 'b':
                        opp_piece_val = min(opp_piece_val, 3);
                        break;
                    case 'n':
                        opp_piece_val = min(opp_piece_val, 3);
                        break;
                    case 'r':
                        opp_piece_val = min(opp_piece_val, 5);
                        break;
                    default:
                        break;
                    }
                }
                 if(this_piece_val>opp_piece_val)white_penalty.push_back(0.9 * (this_piece_val - opp_piece_val));
            }

            else if (board[in_ver][in_hor] >= 'a' && board[in_ver][in_hor] <= 'z')
            {

                int this_piece_val = 0;

                switch (board[in_ver][in_hor])
                {
                case 'p':
                    this_piece_val = 1;
                    break;
                case 'b':
                    this_piece_val = 3;
                    break;
                case 'n':
                    this_piece_val = 3;
                    break;
                case 'r':
                    this_piece_val = 5;
                    break;
                case 'q':
                    this_piece_val = 9;
                    break;
                default:
                    break;
                }

                int opp_piece_val = 9;
                vector<Piece> vec_of_opp_pieces;
                if (!turn)
                    vec_of_opp_pieces = control_squares[in_ver][in_hor];
                else
                    vec_of_opp_pieces = oppcontrol_squares[in_ver][in_hor];

                for (auto it : vec_of_opp_pieces)
                {
                    if (turn == 0 && control_squares[it.i][it.j].size() == 0) continue;
                    if (turn == 1 && oppcontrol_squares[it.i][it.j].size() == 0) continue;
                    switch (it.type)
                    {
                    case 'P':
                        opp_piece_val = min(opp_piece_val, 1);
                        break;
                    case 'B':
                        opp_piece_val = min(opp_piece_val, 3);
                        break;
                    case 'N':
                        opp_piece_val = min(opp_piece_val, 3);
                        break;
                    case 'R':
                        opp_piece_val = min(opp_piece_val, 5);
                        break;
                    default:
                        break;
                    }
                }
                if(this_piece_val>opp_piece_val)black_penalty.push_back(0.9 * (this_piece_val - opp_piece_val));
            }
        }
    }

    // sort(white_penalty.begin(), white_penalty.end(), greater<double>());
    // sort(black_penalty.begin(), black_penalty.end(), greater<double>());

    if (turn)
    {
        for (int i = 0; i < white_penalty.size(); i++) // i+=2
            total_penalty += white_penalty[i];
        for (int i = 0; i < black_penalty.size(); i++) // i=1, i+=2
            total_penalty -= black_penalty[i];
    }
    else
    {
        for (int i = 0; i < white_penalty.size(); i++)
            total_penalty += white_penalty[i];
        for (int i = 0; i < black_penalty.size(); i++)
            total_penalty -= black_penalty[i];
    }

    return -total_penalty;
}


double pieces_eval(const vector<vector<char>> &board, const vector<Piece> pieces, const vector<Piece> oppPieces, bool turn)
{
    double score = 0;
    for (auto piece: pieces)
    {
        if (piece.type == 'R' || piece.type == 'r')
        {
            double sq = 0;
            for (int j = piece.i-1; j>=0; --j)
            {
                if (piece_type(board[j][piece.j], turn) == 0)
                {
                    sq += 1;
                }
                else break;
            }
            for (int j = piece.i+1; j<8; ++j)
            {
                if (piece_type(board[j][piece.j], turn) == 0)
                {
                    sq += 1;
                }
                else break;
            }
            score += 0.0625*sq;
        }
    }
    for (auto piece: oppPieces)
    {
        if (piece.type == 'R' || piece.type == 'r')
        {
            double sq = 0;
            for (int j = piece.i-1; j>=0; --j)
            {
                if (piece_type(board[j][piece.j], !turn) == 0)
                {
                    sq += 1;
                }
                else break;
            }
            for (int j = piece.i+1; j<8; ++j)
            {
                if (piece_type(board[j][piece.j], !turn) == 0)
                {
                    sq += 1;
                }
                else break;
            }
            score -= 0.05*sq;
        }
    }
    if (turn) return -score;
    return score;
}

double mobility(vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, vector<string> my_moves, vector<string> opp_moves, bool turn, bool enpassant,string epsquare,int castling){
    double total_mobility=0.0;
    double white_mobility=0.0;
    double black_mobility=0.0;
    // Moves temp(board,turn,enpassant,epsquare,castling);
    
    // vector<string> my_moves= temp.valid_Moves();
    // vector<string> opp_moves=temp.valid_oppMoves();
    vector<string> all_moves;
    all_moves.reserve(my_moves.size()+opp_moves.size());
    all_moves.insert(all_moves.end(),my_moves.begin(),my_moves.end());
    all_moves.insert(all_moves.end(),opp_moves.begin(),opp_moves.end());
    vector<vector<vector<Piece>>> white_control_squares;
    vector<vector<vector<Piece>>> black_control_squares;

    if(turn){
        black_control_squares=control_squares;
        white_control_squares=oppcontrol_squares;
    }
    else{
        black_control_squares=oppcontrol_squares;
        white_control_squares=control_squares;
    }

    for(auto itr:all_moves){
        if(isupper(itr[0])&& itr[0]!='K' && itr[0]!='O'){
            string pos_in_char;
            pos_in_char.push_back(itr[itr.size()-2]);
            pos_in_char.push_back(itr[itr.size()-1]);
            if (pos_in_char == "pp" || pos_in_char == "PP") continue;
            // cout << pos_in_char << endl;
            pair<int,int> pos_in_num = sij(pos_in_char);
            bool res_in_hang = false , res_in_weak_att = false;
            if(white_control_squares[pos_in_num.first][pos_in_num.second].empty()&&black_control_squares[pos_in_num.first][pos_in_num.second].size()){
                res_in_hang=true;
            }

            int this_piece_val=0;

            switch( itr[0]){
                case 'P' : this_piece_val = 1; break;
                case 'B' : this_piece_val = 3; break;
                case 'N' : this_piece_val = 3; break;
                case 'R' : this_piece_val = 5; break;
                case 'Q' : this_piece_val = 9; break;
                default: break;
            }

            int min_val_attack=10;
            for(auto it: black_control_squares[pos_in_num.first][pos_in_num.second]){
                switch(it.type){
                        case 'p' : min_val_attack = min(min_val_attack,1); break;
                        case 'b' : min_val_attack = min(min_val_attack,3); break;
                        case 'n' : min_val_attack = min(min_val_attack,3); break;
                        case 'r' : min_val_attack = min(min_val_attack,5); break;
                        default: break;
                }
            }
            if(min_val_attack < this_piece_val ) res_in_weak_att=true;
            if(!res_in_hang && !res_in_weak_att)white_mobility++;
        }
        else if(islower(itr[0]) && itr[0]!='k' && itr[0]!='o'){
            string pos_in_char;
            pos_in_char.push_back(itr[itr.size()-2]);
            pos_in_char.push_back(itr[itr.size()-1]);
            pair<int,int> pos_in_num = sij(pos_in_char);
            bool res_in_hang = false , res_in_weak_att = false;
            if(white_control_squares[pos_in_num.first][pos_in_num.second].size()&&black_control_squares[pos_in_num.first][pos_in_num.second].empty()){
                res_in_hang=true;
            }

            int this_piece_val=0;

            switch( itr[0]){
                case 'p' : this_piece_val =1; break;
                case 'b' : this_piece_val =3; break;
                case 'n' : this_piece_val =3; break;
                case 'r' : this_piece_val =5; break;
                case 'q' : this_piece_val =9; break;
                default: break;
            }

            int min_val_attack=10;
            for(auto it: white_control_squares[pos_in_num.first][pos_in_num.second]){
                switch(it.type){
                        case 'P' : min_val_attack = min(min_val_attack,1); break;
                        case 'B' : min_val_attack = min(min_val_attack,3); break;
                        case 'N' : min_val_attack = min(min_val_attack,3); break;
                        case 'R' : min_val_attack = min(min_val_attack,5); break;
                        default: break;
                }
            }
            if(min_val_attack < this_piece_val ) res_in_weak_att=true;
            if(!res_in_hang && !res_in_weak_att)black_mobility++;
        }
    }
    total_mobility=white_mobility-black_mobility;
    total_mobility/=8.0;
    return total_mobility;
}

double eval_kingsafety(const vector<vector<char>> &board, const vector<vector<vector<Piece>>> &control_squares, const vector<vector<vector<Piece>>> &oppcontrol_squares, bool turn)
{
    double white_attack = 0;
    double black_attack = 0;

    double white_missing_pawn = 0;
    double black_missing_pawn = 0;

    double pawn_attack = 0.4;
    double bishop_attack = 0.3;
    double knight_attack = 0.3;
    double queen_attack = 0.6;
    double rook_attack = 0.5;
    double pawn_missing_ahead_penalty = 0.4;
    double side_pawn_missing_penalty = 0.3;

    vector<pair<int, int>> white_king_area;
    vector<pair<int, int>> black_king_area;

    int white_king_hor, white_king_ver;
    int black_king_hor, black_king_ver;

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(board[i][j] == 'K')
            {
                white_king_ver = i;
                white_king_hor = j;
            }
            if(board[i][j] == 'k')
            {
                black_king_ver = i;
                black_king_hor = j;
            }
        }
    }

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(abs(i-white_king_ver) + abs(j-white_king_hor) <= 3)
            {
                white_king_area.push_back({i,j});
            }
            if(abs(i-black_king_ver) + abs(j-black_king_hor) <= 3)
            {
                black_king_area.push_back({i,j});
            }
        }
    }

    for(auto square : white_king_area)
    {
        auto pieces = oppcontrol_squares[square.first][square.second];
        if(turn == 1)
        pieces = control_squares[square.first][square.second];

        for(auto piece : pieces)
        {
            switch(piece.type)
            {
            case 'p' : black_attack += pawn_attack; break;
            case 'b' : black_attack += bishop_attack; break;
            case 'n' : black_attack += knight_attack; break;
            case 'q' : black_attack += queen_attack; break;
            case 'r' : black_attack += rook_attack; break;

            default : break;
            }
        }
    }

    for(auto square : black_king_area)
    {
        auto pieces = control_squares[square.first][square.second];
        if(turn == 1)
        pieces = oppcontrol_squares[square.first][square.second];

        for(auto piece : pieces)
        {
            switch(piece.type)
            {
                case 'P' : white_attack += pawn_attack; break;
                case 'K' : white_attack += knight_attack; break;
                case 'B' : white_attack += bishop_attack; break;
                case 'Q' : white_attack += queen_attack; break;
                case 'R' : white_attack += rook_attack; break;

                default : break;
            }
        }
    }

    if((white_king_ver-1 >= 0 && board[white_king_ver-1][white_king_hor] != 'P') && (white_king_ver-2>=0 && board[white_king_ver-2][white_king_hor] != 'P'))
    {
        white_missing_pawn += pawn_missing_ahead_penalty;
    }
    if(white_king_ver-1 >= 0 && white_king_hor-1 >= 0 && board[white_king_ver-1][white_king_hor-1] != 'P')
    {
        white_missing_pawn += side_pawn_missing_penalty;
    }
    if(white_king_ver-1 >= 0 && white_king_hor+1 < 8 && board[white_king_ver-1][white_king_hor+1] != 'P')
    {
        white_missing_pawn += side_pawn_missing_penalty;
    }

    if((black_king_ver+1 < 8 && board[black_king_ver+1][black_king_hor] != 'p') && (black_king_ver+2) < 8 && board[black_king_ver+2][black_king_hor] != 'p')
    {
        black_missing_pawn += pawn_missing_ahead_penalty;
    }

    if(black_king_ver+1 < 8 && black_king_hor+1 < 8 && board[black_king_ver+1][black_king_hor+1] != 'p')
    {
        black_missing_pawn += side_pawn_missing_penalty;
    }

    if(black_king_ver+1 < 8 && black_king_hor-1 >= 0 && board[black_king_ver+1][black_king_hor-1] != 'p')
    {
        black_missing_pawn += side_pawn_missing_penalty;
    }

    return white_attack-black_attack-white_missing_pawn+black_missing_pawn;
}

double trapped_eval(const vector<Piece> whitetrapped, const vector<Piece> blacktrapped)
{
    double penaldo = 0.0;
    for (auto piece: whitetrapped)
    {
        switch (piece.type)
        {
            case 'Q':
            penaldo += 7.2;
            break;
            case 'R':
            penaldo += 4.0;
            break;
            case 'B':
            penaldo += 2.4;
            break;
            case 'N':
            penaldo += 2.4;
            break;
        }
    }
    for (auto piece: blacktrapped)
    {
        switch (piece.type)
        {
            case 'q':
            penaldo -= 7.2;
            break;
            case 'r':
            penaldo -= 4.0;
            break;
            case 'b':
            penaldo -= 2.4;
            break;
            case 'n':
            penaldo -= 2.4;
            break;
        }
    }
    return -penaldo;
}