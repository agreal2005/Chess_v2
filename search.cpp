#include "search.h"

EvalBar::EvalBar(string f)
{
    pst.init_tables();
    fen.input_FEN(f);
    m.setupPinAndControl(fen.board, fen.turn, fen.isEnPassant, fen.epSquare, fen.castle_options());
}

map<int, map<string, pair<string, double>*>> MasterMap;
map<int, vector<pair<string, double>*>> matMap;

double EvalBar::complete_eval(EvalParams &pr)
{
    int material = get_material(pr.board);
    evalWeights wt;
    wt.changeWeights(material);
    double eval = wt.matwt*evaluate_material(pr.board);
    eval += wt.pawnwt*evaluate_pawn_structure(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn, pr.f);
    eval += wt.outpostwt*evaluate_outposts(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += wt.hangingwt*hanging_piece_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += wt.weakerattacwt*weaker_attacked_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += wt.pieceswt*pieces_eval(pr.board, pr.pieces, pr.oppPieces, pr.turn);
    eval += ((double)pst.eval_sq_tables(pr.board)/625.0)*wt.pstwt;
    if (pr.turn == 0) eval += wt.trappedwt*trapped_eval(pr.trappedPieces, pr.trappedOppPieces);
    else eval += wt.trappedwt*trapped_eval(pr.trappedOppPieces, pr.trappedPieces);
    double king_score = wt.kingwt*eval_kingsafety(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    if(gamePhase > 18)
    {
        eval += king_score;
    }
    eval += wt.mobilitywt*mobility(pr.board, pr.controlSquares, pr.oppControlSquares, pr.validMoves, pr.validOppMoves, pr.turn, pr.isEnPassant, pr.epSquare, pr.castling);
    return eval/10;
}

AllEvalScores EvalBar::complete_TrainingEval(EvalParams &pr){
    int material = get_material(pr.board);
    evalWeights wt;
    wt.changeWeights(material);
    double eval = wt.matwt*evaluate_material(pr.board);
    eval += wt.pawnwt*evaluate_pawn_structure(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn, pr.f);
    eval += wt.outpostwt*evaluate_outposts(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += wt.hangingwt*hanging_piece_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += wt.weakerattacwt*weaker_attacked_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    eval += wt.pieceswt*pieces_eval(pr.board, pr.pieces, pr.oppPieces, pr.turn);
    eval += ((double)pst.eval_sq_tables(pr.board)/625.0)*wt.pstwt;
    if (pr.turn == 0) eval += wt.trappedwt*trapped_eval(pr.trappedPieces, pr.trappedOppPieces);
    else eval += wt.trappedwt*trapped_eval(pr.trappedOppPieces, pr.trappedPieces);
    double king_score = wt.kingwt*eval_kingsafety(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn);
    if(gamePhase > 18)
    {
        eval += king_score;
    }
    eval += wt.mobilitywt*mobility(pr.board, pr.controlSquares, pr.oppControlSquares, pr.validMoves, pr.validOppMoves, pr.turn, pr.isEnPassant, pr.epSquare, pr.castling);
    AllEvalScores Scores(evaluate_pawn_structure(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn, pr.f), (double)material, evaluate_outposts(reverseBoard(pr.board), pr.controlSquares, pr.oppControlSquares, pr.turn),
                         hanging_piece_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn) ,  weaker_attacked_penalty(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn), 
                         mobility(pr.board, pr.controlSquares, pr.oppControlSquares, pr.validMoves, pr.validOppMoves, pr.turn, pr.isEnPassant, pr.epSquare, pr.castling),  pieces_eval(pr.board, pr.pieces, pr.oppPieces, pr.turn), 
                         eval_kingsafety(pr.board, pr.controlSquares, pr.oppControlSquares, pr.turn), trapped_eval(pr.trappedOppPieces, pr.trappedPieces),  (double)pst.eval_sq_tables(pr.board)/625.0);

    return Scores;               
}

pair<string, double> EvalBar::evalTree(string f, int d, int c) {
    if (vis.size() == vis.max_size()/2) {
        while (vis.size()!=vis.max_size()/2 - vis.max_size()/10)
        {
            vis.erase(vis.begin());
        }
    }
    
    if(d<=0){
         cout<<"Invalid depth for evaluation\n";
         return {"_____",0.0};
    }
    
    GameState tempState(f);
    Moves temp_Moves(tempState.board,tempState.return_turn(),tempState.return_ep(),tempState.return_eps(),tempState.castle_options());
    vector<string> my_moves = temp_Moves.valid_Moves();
    
    // if (d!=1) {
    //     my_moves = temp_Moves.valid_Moves();
    // }
    // else {
    //     return {"#", inf};
    // }

    double check_for_end=evaluate_checkmate(tempState.return_board(),temp_Moves.return_oppControlSquares(),temp_Moves.valid_Moves(),tempState.return_turn(),f);
    
    if(check_for_end==inf||check_for_end==-inf){
        return {"#",check_for_end};
    }
    if ((check_for_end==0.0 && my_moves.size()==0))
    {
        return {"-", 0.0};
    }

    if(d==1){
            pair<string,double> result={"_",0.0};
            for(auto move: my_moves){
                string res = tempState.simulateOneMove(move);
                GameState final_fen(res);
                
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
                else if(tempState.return_turn()){
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

    pair<string,double> result={"_",0.0};
    for(auto move: my_moves){
                string res = tempState.simulateOneMove(move);
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
                // if (move == "Be3d4") cout << "****" << temp.first << " " << temp.second << endl;
                // if (d==2) cout << temp.first << " " << temp.second << endl;
                // if (move.substr(0,3) == "bb4") cout << move << " " << temp.first << " " << temp.second << endl;
                // if (f == "rnb1k1nr/pppp1ppp/4p3/8/1P1Pq3/8/PP3PPP/RNBQKBNR w KQkq - 0 1") cout << move << " : " <<  temp.first << " " << temp.second << endl;
                // if (f == "rnbk2nr/pppp1ppp/4p3/8/1P1PQ3/8/PP3PPP/RNB1KBNR b KQkq - 0 1") cout << move << " : " << temp.first << " " << temp.second << endl;
                //  getchar();
                if(result.first=="_"){
                    result=temp;
                    result.first=move;
                }
                else if(tempState.return_turn()){
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

pair<string, double> EvalBar :: NewEvalTree(string BoardFen, int depth, int c, double alpha, double beta){
    // If the first string is a ___ it means that the position is invalid
    // If the first string is a # it means that checkmate has happened
    // If the first string is a - it means that a stalemate has happened
    // If the first string is a _ it means that a leaf has been reached
    
    // check if depth is done
    if(depth < 0){
         cout<<"Invalid depth for evaluation\n";
         return {"___",0.0};
    }

    GameState CurrentState(BoardFen);
    int mat = get_material(CurrentState.board);
    
    if (!MasterMap[depth].empty() && MasterMap[depth].find(BoardFen) != MasterMap[depth].end()) {
        return *MasterMap[depth][BoardFen];
    }

    Moves CurrMoves(CurrentState.board,CurrentState.return_turn(),CurrentState.return_ep(),CurrentState.return_eps(),CurrentState.castle_options());
    vector<string> MyMoves = CurrMoves.valid_Moves();

    // check whether tapli has been received
    double CheckForEnd=evaluate_checkmate(CurrentState.return_board(), CurrMoves.return_oppControlSquares() , CurrMoves.valid_Moves(), CurrentState.return_turn(), BoardFen);
    if(CheckForEnd== inf || CheckForEnd ==-inf){
        MasterMap[depth][BoardFen] = new pair<string, double> {"#", CheckForEnd};
        matMap[mat].push_back(MasterMap[depth][BoardFen]);
        // insert_into__relevant_map(BoardFen);
        return {"#", CheckForEnd};
    }
    if ((CheckForEnd==0.0 && MyMoves.size()==0))
    {
        MasterMap[depth][BoardFen] = new pair<string, double> {"-", 0.0};
        matMap[mat].push_back(MasterMap[depth][BoardFen]);
        // insert_into__relevant_map(BoardFen);
        return {"-", 0.0};
    }

    if(depth == 0){
            EvalParams AllEvalParams(CurrMoves, CurrentState, BoardFen);
            double CurrentScore = complete_eval(AllEvalParams);
            return {"_", CurrentScore};
    }

    // check kiska move hai 
    if(CurrentState.return_turn() == 0){
        // White kheltoy atta
        string MoveToBePlayed = MyMoves[0];
        double MaxScore = -inf;
        for(auto move : MyMoves){
            string res = CurrentState.simulateOneMove(move);
            double PotentialScore;
            PotentialScore = NewEvalTree(res, depth-1, c, alpha, beta).second;
            if(PotentialScore > MaxScore){
                MaxScore = PotentialScore;
                MoveToBePlayed = move;
            }
            alpha = max(alpha, PotentialScore);
            if(beta <= alpha){
                break;
            }
        }
        MasterMap[depth][BoardFen] = new pair<string,double> (MoveToBePlayed, MaxScore);
        matMap[mat].push_back(MasterMap[depth][BoardFen]);
        // MasterMap[BoardFen] = score_entry(depth, MaxScore, MoveToBePlayed);
        // insert_into__relevant_map(BoardFen);
        return {MoveToBePlayed, MaxScore};
    }

    else{
        // Black kheltoy atta
        string MoveToBePlayed = MyMoves[0];
        double MinScore = inf;
        for(auto move : MyMoves){
            string res = CurrentState.simulateOneMove(move);
            double PotentialScore;
            PotentialScore = NewEvalTree(res, depth-1, c, alpha, beta).second;
            if(PotentialScore < MinScore){
                MinScore = PotentialScore;
                MoveToBePlayed = move;
            }
            beta = min(beta, PotentialScore);
            if(beta <= alpha){
                break;
            }
        }
        MasterMap[depth][BoardFen] = new pair<string,double> (MoveToBePlayed, MinScore);
        matMap[mat].push_back(MasterMap[depth][BoardFen]);
        // MasterMap[] = score_entry(depth, MinScore, MoveToBePlayed);
        // insert_into__relevant_map(BoardFen);
        return {MoveToBePlayed, MinScore};
    }
}

pair<string, AllEvalScores> EvalBar :: TrainingTree(string BoardFen, int depth, int c, double alpha, double beta){
    if(depth < 0){
         cout<<"Invalid depth for evaluation\n";
         AllEvalScores tapli;
         return {"___", tapli};
    }

    GameState CurrentState(BoardFen);
    Moves CurrMoves(CurrentState.board,CurrentState.return_turn(),CurrentState.return_ep(),CurrentState.return_eps(),CurrentState.castle_options());
    vector<string> MyMoves = CurrMoves.valid_Moves();
    double CheckForEnd=evaluate_checkmate(CurrentState.return_board(), CurrMoves.return_oppControlSquares() , CurrMoves.valid_Moves(), CurrentState.return_turn(), BoardFen);
    if(CheckForEnd== inf || CheckForEnd ==-inf){
        AllEvalScores tapli;
        tapli.TotalScore = CheckForEnd;
        return {"#", tapli};
    }
    if ((CheckForEnd==0.0 && MyMoves.size()==0))
    {   
        AllEvalScores tapli;
        return {"-", tapli};
    }

    if(depth == 0){
            EvalParams AllEvalParams(CurrMoves, CurrentState, BoardFen);
            AllEvalScores CurrentScore = complete_TrainingEval(AllEvalParams);
            return {"_", CurrentScore};
    }

    if(CurrentState.return_turn() == 0){
        // White kheltoy atta
        string MoveToBePlayed = MyMoves[0];
        AllEvalScores MaxScore;
        MaxScore.TotalScore = -inf;
        for(auto move : MyMoves){
            string res = CurrentState.simulateOneMove(move);
            AllEvalScores PotentialScore;
            PotentialScore = TrainingTree(res, depth-1, c, alpha, beta).second;
            if(PotentialScore.TotalScore > MaxScore.TotalScore){
                MaxScore = PotentialScore;
                MoveToBePlayed = move;
            }
            alpha = max(alpha, PotentialScore.TotalScore);
            if(beta <= alpha){
                break;
            }
        }
        return {MoveToBePlayed, MaxScore};
    }
    else{
        // Black kheltoy atta
        string MoveToBePlayed = MyMoves[0];
        AllEvalScores Minscore;
        Minscore.TotalScore = inf;
        for(auto move : MyMoves){
            string res = CurrentState.simulateOneMove(move);
            AllEvalScores PotentialScore;
            PotentialScore = TrainingTree(res, depth-1, c, alpha, beta).second;
            if(PotentialScore.TotalScore < Minscore.TotalScore){
                Minscore = PotentialScore;
                MoveToBePlayed = move;
            }
            beta = min(beta, PotentialScore.TotalScore);
            if(beta <= alpha){
                break;
            }
        }
        return {MoveToBePlayed, Minscore};
    }
} 
