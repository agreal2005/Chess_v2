#include "search.h"

EvalBar::EvalBar(string f)
{
    pst.init_tables();
    fen.input_FEN(f);
    m.fetch_Moves(fen.board, fen.turn, fen.isEnPassant, fen.epSquare, fen.castle_options());
}

// Make functions for complete evaluation and Game Search Tree here

string EvalBar::playOneMove(string &move, vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms)
{
    string curr_position = move.substr(1, 2);
    string next_position = move.substr(move.length() - 2, 2);
    pair<int,int> curr_ij, next_ij;
    if (move[0] != 'o' && move[0] != 'O') curr_ij = sij(curr_position);
    if (move[0] != 'o' && move[0] != 'O') next_ij = sij(next_position);
    if (move == "O-O") // white castling king side
    {
        brd[7][4] = '.';
        brd[7][7] = '.';
        brd[7][5] = 'R';
        brd[7][6] = 'K';
        wck = false;
        wcq = false;
        isEnp = false;
    }
    else if (move == "O-O-O") // black castling queen side
    {
        brd[7][3] = 'R';
        brd[7][2] = 'K';
        brd[7][0] = '.';
        brd[7][1] = '.';
        brd[7][4] = '.';
        wck = false;
        wcq = false;
        isEnp = false;
    }
    else if (move == "o-o") // black castling king side
    {
        brd[0][5] = 'r';
        brd[0][6] = 'k';
        brd[0][4] = '.';
        brd[0][7] = '.';
        bck = false;
        bcq = false;
        isEnp = false;
    }
    else if (move == "o-o-o") // black castling queen side
    {
        brd[0][3] = 'r';
        brd[0][2] = 'k';
        brd[0][1] = '.';
        brd[0][0] = '.';
        brd[0][4] = '.';
        bck = false;
        bcq = false;
        isEnp = false;
    }
    else if (move[0] == 'P' && next_ij.first == 0)
    {
        brd[curr_ij.first][curr_ij.second] = '.';
        brd[0][next_ij.second] = 'Q';
    }
    else if (move[0] == 'p' && next_ij.first == 7)
    {
        brd[curr_ij.first][curr_ij.second] = '.';
        brd[7][next_ij.second] = 'q';
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
        if (move[0] == 'K')
        {
            wck = 0; wcq = 0;
        }
        else if (move[0] == 'k')
        {
            bck = 0; bcq = 0;
        }
        else if (move[0] == 'R' && curr_position == "h1") wck = 0;
        else if (move[0] == 'R' && curr_position == "a1") wcq = 0;
        else if (move[0] == 'r' && curr_position == "h8") bck = 0;
        else if (move[0] == 'r' && curr_position == "a8") bcq = 0;
    }
    t = !t;
    // display_board(brd); // for testing purposes
    return fen.get_FEN(brd, t, wck, wcq, bck, bcq, isEnp, epS, hfc, fms);
}

double EvalBar::complete_eval(EvalParams &pr)
{
    int material = get_material(pr.board);
    double matwt = 0, pawnwt = 0, outpostwt = 0, hangingwt = 0, weakerattacwt = 0, pieceswt = 0, pstwt = 0, trappedwt = 0, kingwt = 0, mobilitywt = 0;
    if (material <= 20) // Endgame
    {
        matwt = 7;
        pawnwt = 1/10;
        hangingwt = 1.5;
        weakerattacwt = 2;
        trappedwt = 1;
        pstwt = 0.8;
        mobilitywt = 0.007;
    }
    else if (material > 68) // Opening
    {
        matwt = 7;
        pawnwt = 1/20;
        outpostwt = 0;
        hangingwt = 1.5;
        weakerattacwt = 1.2;
        trappedwt = 1;
        pstwt = 0.04;
        kingwt = 0.01;
        mobilitywt = 0.001;
    }
    else // Middle game
    {
        matwt = 7;
        pawnwt = 1/30;
        outpostwt = 0;
        hangingwt = 1.5;
        weakerattacwt = 2;
        trappedwt = 1;
        pstwt = 0.08;
        kingwt = 0.01;
        mobilitywt = 0.001;
    }
    double eval = matwt*evaluate_material(pr.board);
    eval += pawnwt*evaluate_pawn_structure(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn, pr.f);
    eval += outpostwt*evaluate_outposts(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += hangingwt*hanging_piece_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += weakerattacwt*weaker_attacked_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += pieceswt*pieces_eval(pr.board, pr.pieces, pr.oppPieces, pr.turn);
    eval += ((double)pst.eval_sq_tables(pr.board)/625.0)*pstwt;
    if (pr.turn == 0) eval += trappedwt*trapped_eval(pr.trappedPieces, pr.trappedOppPieces);
    else eval += trappedwt*trapped_eval(pr.trappedOppPieces, pr.trappedPieces);
    double king_score = kingwt*eval_kingsafety(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    if(gamePhase > 18)
    {
        eval += king_score;
    }
    eval += mobilitywt*mobility(pr.board, pr.controlSquares, pr.oppControlSquares, pr.validMoves, pr.validOppMoves, pr.turn, pr.isEnPassant, pr.epSquare, pr.castling);
    return eval;
}

pair<string, double> EvalBar::evalTree(string f, int d, int c) {
    if (vis.size() == vis.max_size()/2) {
        while (vis.size()!=vis.max_size()/2 - vis.max_size()/10)
        {
            vis.erase(vis.begin());
        }
    }
    // if (d == 3 && c == 0) vis.clear();
    if(d<=0){
         cout<<"Invalid depth for evaluation\n";
         return {"_____",0.0};
    }
    // cout << f << " at depth: " << d << endl;
    Board_FEN temp_fen(f);
    Moves temp_Moves(temp_fen.board,temp_fen.return_turn(),temp_fen.return_ep(),temp_fen.return_eps(),temp_fen.castle_options());
    vector<string> my_moves = temp_Moves.valid_Moves();
    
    // if (d!=1) {
    //     my_moves = temp_Moves.valid_Moves();
    // }
    // else {
    //     return {"#", inf};
    // }

    double check_for_end=evaluate_checkmate(temp_fen.return_board(),temp_Moves.return_oppControlSquares(),temp_Moves.valid_Moves(),temp_fen.return_turn(),f);
    
    if(check_for_end==inf||check_for_end==-inf){
        return {"#",check_for_end};
    }
    if ((check_for_end==0.0 && my_moves.size()==0))
    {
        return {"-", 0.0};
    }

    if(d==1){
            int cas_opt=temp_fen.castle_options();
            pair<string,double> result={"_",0.0};
            // cout << my_moves.size() << "\n";
            for(auto move: my_moves){
                //  cout << move << ' ';
                string res=playOneMove(move,temp_fen.return_board(),temp_fen.return_turn(),((cas_opt&8)!=0),((cas_opt&4)!=0),((cas_opt&2)!=0),((cas_opt&1)!=0),temp_fen.return_ep(),temp_fen.return_eps(),temp_fen.return_halfmoveclk(),temp_fen.return_fullmoves());
                //  cout << res << endl;
                Board_FEN final_fen(res);
                //  cout << final_fen.get_FEN() << " ; ";
                //  cout << final_fen.return_turn() << " " << final_fen.return_ep() << " " << final_fen.return_eps() << " " << final_fen.castle_options() << endl;
                string tag = res.substr(0, res.length() - 4);
                pair<string,double> temp;
                temp.first=move;
                if (vis[tag].first == 0)
                {
                    Moves final_Moves(final_fen.board,final_fen.return_turn(),final_fen.return_ep(),final_fen.return_eps(),final_fen.castle_options());
                    // return {move, 0.0};
                    EvalParams lmao_mujhe_ni_pata_kya_hai_ye(final_Moves,final_fen,f);
                    temp.second=complete_eval(lmao_mujhe_ni_pata_kya_hai_ye);
                }
                else temp.second = vis[tag].second.second;
                // temp.second = 0.01;
                
                if(result.first=="_"){
                    result=temp;
                    result.first=move;
                }
                else if(temp_fen.return_turn()){
                    if(result.second>temp.second){
                        result=temp;
                    }
                }
                else if(result.second<temp.second){
                    result=temp;
                }
                // cout << move << " " << temp.second << endl;
            }
            vis[f.substr(0, f.length()-4)] = {1, result};
            return result;
    }

    int cas_opt=temp_fen.castle_options();

    pair<string,double> result={"_",0.0};
    for(auto move: my_moves){
                string res=playOneMove(move,temp_fen.return_board(),temp_fen.return_turn(),((cas_opt&8)!=0),((cas_opt&4)!=0),((cas_opt&2)!=0),((cas_opt&1)!=0),temp_fen.return_ep(),temp_fen.return_eps(),temp_fen.return_halfmoveclk(),temp_fen.return_fullmoves());
                string tag = res.substr(0, res.length() - 4);
                pair<string,double> temp;
                if (vis[tag].first == 0)
                {
                    temp = evalTree(res,d-1, 1+c);
                }
                else
                {
                    temp = vis[tag].second;
                }
                // if (move == "Pc3xb4") cout << "****" << temp.first << " " << temp.second << endl;
                // if (d==2) cout << temp.first << " " << temp.second << endl;
                // if (move.substr(0,3) == "bb4") cout << move << " " << temp.first << " " << temp.second << endl;
                // if (f == "rnb1k1nr/pppp1ppp/4p3/8/1P1Pq3/8/PP3PPP/RNBQKBNR w KQkq - 0 1") cout << move << " : " <<  temp.first << " " << temp.second << endl;
                // if (f == "rnbk2nr/pppp1ppp/4p3/8/1P1PQ3/8/PP3PPP/RNB1KBNR b KQkq - 0 1") cout << move << " : " << temp.first << " " << temp.second << endl;
                //  getchar();
                if(result.first=="_"){
                    result=temp;
                    result.first=move;
                }
                else if(temp_fen.return_turn()){
                        if(result.second>temp.second){
                            result=temp;
                            result.first=move;
                        }
                }
                else if(result.second<temp.second){
                        result=temp;
                        result.first=move;
                    }
                vis[f.substr(0, f.length()-4)] = {1, result};
    }
    return result;
}
    