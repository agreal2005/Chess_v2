#include "eval.h"
#pragma once
#define DEPTH 3

class AllEvalScores{
    public :
    double TotalScore;
    double PawnStructScore;
    double MaterialScore;
    double OutpostScore;
    double HangingPiecePenalty;
    double WeakerAttackedPenalty;
    double MobilityScore;
    double PiecesEval;
    double KingSafetyScore;
    double TrappedScore;
    double PiecesquareScore;

    AllEvalScores(double n1=0, double n2=0, double n3=0, double n4=0, double n5=0, double n6=0, double n7=0, double n8=0, double n9=0, double n10=0){
        PawnStructScore = n1;
        MaterialScore = n2;
        OutpostScore = n3;
        HangingPiecePenalty = n4;
        WeakerAttackedPenalty = n5;
        MobilityScore = n6;
        PiecesEval = n7;
        KingSafetyScore = n8;
        TrappedScore = n9;
        PiecesquareScore = n10;
        TotalScore = n1+n2+n3+n4+n5+n6+n7+n8+n9+n10;
    }
};

struct evalWeights
{
    double matwt = 0, 
    pawnwt = 0, 
    outpostwt = 0, 
    hangingwt = 0, 
    weakerattacwt = 0, 
    pieceswt = 0, 
    pstwt = 0, 
    trappedwt = 0, 
    kingwt = 0, 
    mobilitywt = 0;
    void changeWeights(int material)
    {
        if (material <= 20) // Endgame
        {
            matwt = 2;
            pawnwt = 1/10;
            hangingwt = 1;
            weakerattacwt = 2;
            trappedwt = 1;
            pstwt = 0.01;
            mobilitywt = 0.04;
        }
        else if (material > 74) // Opening
        {
            matwt = 1;
            pawnwt = 1/10;
            outpostwt = 0;
            hangingwt = 1;
            weakerattacwt = 1.2;
            trappedwt = 1;
            pstwt = 0.02;
            kingwt = 0.006;
            mobilitywt = 0.015;
        }
        else // Middle game
        {
            matwt = 1.5;
            pawnwt = 1/30;
            outpostwt = 0.5;
            hangingwt = 1;
            weakerattacwt = 2;
            trappedwt = 1;
            pstwt = 0.1;
            kingwt = 0.01;
            mobilitywt = 0.02;
        }
    }
} wt;

struct EvalParams
{
    string f; // fen
    string tag; // important part of the fen
    vector<vector<char>> board; // from fen.h
    vector<Piece> pieces; // from moves.h
    vector<Piece> oppPieces; // from moves.h
    vector<vector<vector<Piece>>> controlSquares; // from moves.h
    vector<vector<vector<Piece>>> oppControlSquares; // from moves.h
    bool turn, isEnPassant; // from fen.h
    int castling; // from fen.h
    string epSquare; // from fen.h
    vector<Piece> trappedPieces; // from moves.h
    vector<Piece> trappedOppPieces; // from moves.h
    vector<string> validMoves; // from moves.h
    vector<string> validOppMoves; // from moves.h

    EvalParams( Moves &mv, Board_FEN &bf, string &fen){
       f=fen;
       tag = fen.substr(0, fen.length() - 4);
       board=bf.board;
       controlSquares=mv.return_controlSquares();
       oppControlSquares=mv.return_oppControlSquares();
       validMoves=mv.valid_Moves();
       validOppMoves=mv.valid_oppMoves();
       trappedPieces=mv.return_trappedPieces();
       trappedOppPieces=mv.return_trappedOppPieces();
       pieces=mv.pieces;
       oppPieces=mv.oppPieces;
       turn=bf.return_turn();
       isEnPassant=bf.return_ep();
       castling=bf.castle_options();
       epSquare=bf.return_eps();
    }
    // bool operator<(const EvalParams &o) const {
    //     return tag < o.tag;
    // }
    // bool operator==(const EvalParams &o) const {
    //     return tag == o.tag;
    // }
};

class EvalBar
{
    private:
    Board_FEN fen; // Contains info about the board, turn, ep Square, Castling, etc.
    Moves m; // Contains info about pieces controlling squares, valid Moves, etc.
    map<string, pair<bool, pair<string, double>>> vis;
    // map<string, double> memoFen;
    public:
    EvalBar(string f = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    string playOneMove(string &move, vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms); // Return FEN
    /*
        Assuming string move is always a valid legal move
        Update all the boolean params
        if valid Move is enPassant, then capture that pawn also
        and see if after Valid move, there is en passant
        Castling also has to be updated
        Turn also has to be updated
    */
    pair<string, double> evalTree(string f, int d = DEPTH, int c = 0);  // d = depth, f = fen, isOpp = is it Opponent's turn or not, c = count of depth
    // First take the fen 'f' and send it to playOneMove
    // Retrieve the parameters in the function
    // Make the vectors for pieces, oppPieces, controlSquares, oppControlSquares, validMoves, validOppMoves
    // See fen.h & moves.h for reference
    // pass everything to complete_eval including isOpp
    double complete_eval(EvalParams &pr); // To be called when eval reaches its depth
    AllEvalScores complete_TrainingEval(EvalParams &pr);
    pair<string, double> NewEvalTree(string BoardFen, int depth, int c, double alpha, double beta);
    pair<string, AllEvalScores> TrainingTree(string BoardFen, int depth, int c, double alpha, double beta);
    
};