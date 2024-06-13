#include "eval.h"
#pragma once
#define DEPTH 5
struct EvalParams
{
    string f; // fen
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
       board=bf.board;
       pieces=mv.pieces;
       oppPieces=mv.oppPieces;
       controlSquares=mv.return_controlSquares();
       oppControlSquares=mv.return_oppControlSquares();
       turn=bf.return_turn();
       isEnPassant=bf.return_ep();
       castling=bf.castle_options();
       epSquare=bf.return_eps();
       trappedPieces=mv.return_trappedPieces();
       trappedOppPieces=mv.return_trappedOppPieces();
       validMoves=mv.valid_Moves();
       validOppMoves=mv.valid_oppMoves();
    }
};

class EvalBar
{
    private:
    Board_FEN fen; // Contains info about the board, turn, ep Square, Castling, etc.
    Moves m; // Contains info about pieces controlling squares, valid Moves, etc.
    
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
    pair<string, double> evalTree(string &f, int d = DEPTH);  // d = depth, f = fen, isOpp = is it Opponent's turn or not
    // First take the fen 'f' and send it to playOneMove
    // Retrieve the parameters in the function
    // Make the vectors for pieces, oppPieces, controlSquares, oppControlSquares, validMoves, validOppMoves
    // See fen.h & moves.h for reference
    // pass everything to complete_eval including isOpp
    double complete_eval(EvalParams &pr); // To be called when eval reaches its depth
    
};