#include "moves.h"
Moves::Moves()
{
    for (int i=0; i<8; ++i)
    {
        vector<char> initialize(8, '.');
        board.push_back(initialize);
    }
    for (int i=0; i<8; ++i)
    {
        vector<vector<Piece>> initialize(8);
        controlSquares.push_back(initialize);
    }
    oppControlSquares=controlSquares;
    turn = 0;
    wck = wcq = bck = bcq = false;
    king_square = sij("e1");
}
Moves::Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare, int castling)
{
    for (int i=0; i<8; ++i)
    {
        vector<char> initialize(8, '.');
        board.push_back(initialize);
    }
    for (int i=0; i<8; ++i)
    {
        vector<vector<Piece>> initialize(8);
        controlSquares.push_back(initialize);
    }
    oppControlSquares=controlSquares;
    fetch_Moves(board, turn, isEnPassant, epSquare, castling);
    return_controlSquares();
    return_oppControlSquares();
}
void Moves::fetch_Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare, int castling)
{
    this->board = board;
    this->turn = turn;
    this->isEnPassant = isEnPassant;
    this->epSquare = epSquare;
    if (castling&1) bcq = 1;
    if ((castling>>1)&1) bck = 1;
    if ((castling>>2)&1) wcq = 1;
    if ((castling>>3)&1) wck = 1;
    for (int i=0; i<8; ++i)
    {
        for (int j=0; j<8; ++j)
        {
            if (turn == 0 && board[i][j] == 'K')
            {
                king_square = {i, j};
                return;
            }
            if (turn == 1 && board[i][j] == 'k')
            {
                king_square = {i,j};
                return;
            }
        }
    }
}

vector<vector<vector<Piece>>> Moves::return_controlSquares()
{   
    int offset= turn? 32:0;
    for(int in_ver=0;in_ver<8;in_ver++){
       for(int in_hor=0;in_hor<8;in_hor++){
            controlSquares[in_ver][in_hor].clear();
       }
    }

    for(int in_ver=0;in_ver<8;in_ver++){
       for(int in_hor=0;in_hor<8;in_hor++){
            if (board[in_ver][in_hor] == '.') continue;
            char temp=board[in_ver][in_hor]-offset;

            Piece piece(board[in_ver][in_hor], in_ver, in_hor);
              if(temp=='Q'){

                    int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor--;
                    }
                    if(temp_hor>=0)controlSquares[in_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor++;
                    }
                    if(temp_hor<8)controlSquares[in_ver][temp_hor].push_back(piece);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver--;
                    }
                    if(temp_ver>=0)controlSquares[temp_ver][in_hor].push_back(piece);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver++;
                    }
                    if(temp_ver<8)controlSquares[temp_ver][in_hor].push_back(piece);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back(piece);
              }
      
              if(temp=='R'){
                int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor--;
                    }
                    if(temp_hor>=0)controlSquares[in_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor++;
                    }
                    if(temp_hor<8)controlSquares[in_ver][temp_hor].push_back(piece);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver--;
                    }
                    if(temp_ver>=0)controlSquares[temp_ver][in_hor].push_back(piece);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver++;
                    }
                    if(temp_ver<8)controlSquares[temp_ver][in_hor].push_back(piece);
              }
      
              if(temp=='B'){
                    int temp_hor=in_hor-1;
                    int temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back(piece);
              }
       
              if(temp=='N'){
                if(in_ver>=2&&in_hor>=1)controlSquares[in_ver-2][in_hor-1].push_back(piece);
                if(in_ver>=1&&in_hor>=2)controlSquares[in_ver-1][in_hor-2].push_back(piece);
                if(in_ver>=2&&in_hor<7)controlSquares[in_ver-2][in_hor+1].push_back(piece);
                if(in_ver>=1&&in_hor<6)controlSquares[in_ver-1][in_hor+2].push_back(piece);
                if(in_ver<6&&in_hor>=1)controlSquares[in_ver+2][in_hor-1].push_back(piece);
                if(in_ver<7&&in_hor>=2)controlSquares[in_ver+1][in_hor-2].push_back(piece);
                if(in_ver<6&&in_hor<7)controlSquares[in_ver+2][in_hor+1].push_back(piece);
                if(in_ver<7&&in_hor>=6)controlSquares[in_ver+1][in_hor+2].push_back(piece);
              }
        
              if(temp=='P'){
                if(turn==0&&in_ver>0){
                    if(in_hor)controlSquares[in_ver-1][in_hor-1].push_back(piece);
                    if(in_hor<7)controlSquares[in_ver-1][in_hor+1].push_back(piece);
                }
                if(turn==1&&in_ver<7){
                    if(in_hor)controlSquares[in_ver+1][in_hor-1].push_back(piece);
                    if(in_hor<7)controlSquares[in_ver+1][in_hor+1].push_back(piece);
                }
              }
      
              if(temp=='K'){
                if(in_ver>0){
                    if(in_hor>0)controlSquares[in_ver-1][in_hor-1].push_back(piece);
                    if(in_hor<7)controlSquares[in_ver-1][in_hor+1].push_back(piece);   
                    controlSquares[in_ver-1][in_hor].push_back(piece);  
                }
                if(in_ver<7){
                    if(in_hor>0)controlSquares[in_ver+1][in_hor-1].push_back(piece);
                    if(in_hor<7)controlSquares[in_ver+1][in_hor+1].push_back(piece);   
                    controlSquares[in_ver+1][in_hor].push_back(piece);  
                }
                if(in_hor>0)controlSquares[in_ver][in_hor-1].push_back(piece);
                if(in_hor<7)controlSquares[in_ver][in_hor+1].push_back(piece);
              }
      
       }
    }
    return controlSquares;
}

vector<vector<vector<Piece>>> Moves::return_oppControlSquares()
{
     int offset= turn? 0:32;
    for(int in_ver=0;in_ver<8;in_ver++){
       for(int in_hor=0;in_hor<8;in_hor++){
            oppControlSquares[in_ver][in_hor].clear();
       }
    }

    for(int in_ver=0;in_ver<8;in_ver++){
       for(int in_hor=0;in_hor<8;in_hor++){
            if (board[in_ver][in_hor] == '.') continue;
            char temp=board[in_ver][in_hor]-offset;
            Piece piece(board[in_ver][in_hor], in_ver, in_hor);

              if(temp=='Q'){
                    int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor--;
                    }
                    if(temp_hor>=0)oppControlSquares[in_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor++;
                    }
                    if(temp_hor<8)oppControlSquares[in_ver][temp_hor].push_back(piece);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver--;
                    }
                    if(temp_ver>=0)oppControlSquares[temp_ver][in_hor].push_back(piece);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver++;
                    }
                    if(temp_ver<8)oppControlSquares[temp_ver][in_hor].push_back(piece);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back(piece);
              }
      
              if(temp=='R'){
                int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor--;
                    }
                    if(temp_hor>=0)oppControlSquares[in_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back(piece);
                        temp_hor++;
                    }
                    if(temp_hor<8)oppControlSquares[in_ver][temp_hor].push_back(piece);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver--;
                    }
                    if(temp_ver>=0)oppControlSquares[temp_ver][in_hor].push_back(piece);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back(piece);
                        temp_ver++;
                    }
                    if(temp_ver<8)oppControlSquares[temp_ver][in_hor].push_back(piece);
              }
      
              if(temp=='B'){
                    int temp_hor=in_hor-1;
                    int temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back(piece);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back(piece);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back(piece);
              }
       
              if(temp=='N'){
                if(in_ver>=2&&in_hor>=1)oppControlSquares[in_ver-2][in_hor-1].push_back(piece);
                if(in_ver>=1&&in_hor>=2)oppControlSquares[in_ver-1][in_hor-2].push_back(piece);
                if(in_ver>=2&&in_hor<7)oppControlSquares[in_ver-2][in_hor+1].push_back(piece);
                if(in_ver>=1&&in_hor<6)oppControlSquares[in_ver-1][in_hor+2].push_back(piece);
                if(in_ver<6&&in_hor>=1)oppControlSquares[in_ver+2][in_hor-1].push_back(piece);
                if(in_ver<7&&in_hor>=2)oppControlSquares[in_ver+1][in_hor-2].push_back(piece);
                if(in_ver<6&&in_hor<7)oppControlSquares[in_ver+2][in_hor+1].push_back(piece);
                if(in_ver<7&&in_hor>=6)oppControlSquares[in_ver+1][in_hor+2].push_back(piece);
              }
        
              if(temp=='P'){
                if(turn==0&&in_ver>0){
                    if(in_hor)oppControlSquares[in_ver+1][in_hor-1].push_back(piece);
                    if(in_hor<7)oppControlSquares[in_ver+1][in_hor+1].push_back(piece);
                }
                if(turn==1&&in_ver<7){
                    if(in_hor)oppControlSquares[in_ver-1][in_hor-1].push_back(piece);
                    if(in_hor<7)oppControlSquares[in_ver-1][in_hor+1].push_back(piece);
                }
              }
       
              if(temp=='K'){
                if(in_ver>0){
                    if(in_hor>0)oppControlSquares[in_ver-1][in_hor-1].push_back(piece);
                    if(in_hor<7)oppControlSquares[in_ver-1][in_hor+1].push_back(piece);   
                    oppControlSquares[in_ver-1][in_hor].push_back(piece);  
                }
                if(in_ver<7){
                    if(in_hor>0)oppControlSquares[in_ver+1][in_hor-1].push_back(piece);
                    if(in_hor<7)oppControlSquares[in_ver+1][in_hor+1].push_back(piece);   
                    oppControlSquares[in_ver+1][in_hor].push_back(piece);  
                }
                if(in_hor>0)oppControlSquares[in_ver][in_hor-1].push_back(piece);
                if(in_hor<7)oppControlSquares[in_ver][in_hor+1].push_back(piece);
              }     
       }
    }
    return oppControlSquares;
}

vector<string> Moves::valid_Moves()
{
    vector<string> validMoves;
    // First writing function assuming pov of white

    // First check if the King is in Check
    // If yes, then see if anyone can defend the king
    // If no one can defend the king, see if the king has to move

    if (oppControlSquares[king_square.first][king_square.second].size() > 0)
    {
        // King is in Check
        // First we'll see if any piece can protect it
        for (auto piece : oppControlSquares[king_square.first][king_square.second])
        {
            // Cycle over all opponent pieces that are putting king in check
            if (piece.type == 'Q' || piece.type == 'q' || piece.type == 'B' || piece.type == 'b' || piece.type == 'R' || piece.type == 'r' || piece.type == 'P' || piece.type == 'p')
            {
                // Find if any of our piece(s) can come on the straight line
                // from the K to Q including Q's square
                int i_inc = 0, j_inc = 0;
                if (piece.i > king_square.first)
                    i_inc = -1;
                else if (piece.i < king_square.first)
                    i_inc = 1;
                if (piece.j > king_square.second)
                    j_inc = -1;
                else if (piece.j < king_square.second)
                    j_inc = 1;
                int x = piece.i, y = piece.j;
                while (x!=king_square.first || y!=king_square.second)
                {
                    int sd = piece_type(board[x][y], turn);
                    if (sd <= 0)
                    {
                        for (auto p : controlSquares[x][y])
                        {
                            if (p.type == 'K' || p.type == 'k' || p.pinned) continue;
                            string str;
                            str.push_back(p.type);
                            if (sd == 0) str += ijs(x,y);
                            else str += "x" + ijs(x,y);
                            validMoves.push_back(str);
                        }
                    }
                    x+=i_inc;
                    y+=j_inc;
                }
            }
            else if (piece.type == 'H' || piece.type == 'h') // Horsey
            {
                int x = piece.i, y = piece.j;
                for (auto p : controlSquares[x][y])
                {
                    if (p.type == 'K' || p.type == 'k' || p.pinned) continue;
                    string str;
                    str.push_back(p.type);
                    str += "x" + ijs(x,y);
                    validMoves.push_back(str);
                }
            }
        }
        // Now we'll see if the King can move
        int in_ver = king_square.first, in_hor = king_square.second;
        string king = "K";
        if (turn) king = "k";
        if(in_ver>0){
            if(in_hor>0 && oppControlSquares[in_ver-1][in_hor-1].size() == 0)
            {
                if (piece_type(board[in_ver-1][in_hor-1], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver-1,in_hor-1));
                else if (piece_type(board[in_ver-1][in_hor-1], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver-1,in_hor-1));
            }
            if(in_hor<7 && oppControlSquares[in_ver-1][in_hor+1].size() == 0)
            {
                if (piece_type(board[in_ver-1][in_hor+1], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver-1,in_hor+1));
                else if (piece_type(board[in_ver-1][in_hor+1], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver-1,in_hor+1));
            }
            if (oppControlSquares[in_ver-1][in_hor].size() == 0)
            {
                if (piece_type(board[in_ver-1][in_hor], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver-1,in_hor));
                else if (piece_type(board[in_ver-1][in_hor], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver-1,in_hor));
            }
        }
        if(in_ver<7){
            if(in_hor>0 && oppControlSquares[in_ver+1][in_hor-1].size() == 0)
            {
                if (piece_type(board[in_ver+1][in_hor-1], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver+1,in_hor-1));
                else if (piece_type(board[in_ver+1][in_hor-1], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver+1,in_hor-1));
            }
            if(in_hor<7 && oppControlSquares[in_ver+1][in_hor+1].size() == 0)
            {
                if (piece_type(board[in_ver+1][in_hor+1], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver+1,in_hor+1));
                else if (piece_type(board[in_ver+1][in_hor+1], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver+1,in_hor+1));
            }  
            if (oppControlSquares[in_ver+1][in_hor].size() == 0)
            {
                if (piece_type(board[in_ver+1][in_hor], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver+1,in_hor));
                else if (piece_type(board[in_ver+1][in_hor], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver+1,in_hor));
            }
        }
        if(in_hor>0 && oppControlSquares[in_ver][in_hor-1].size() == 0)
        {
            if (piece_type(board[in_ver][in_hor-1], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver,in_hor-1));
                else if (piece_type(board[in_ver][in_hor-1], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver,in_hor-1));
        }
        if(in_hor<7 && oppControlSquares[in_ver][in_hor+1].size() == 0)
        {
            if (piece_type(board[in_ver][in_hor+1], turn) == 0)
                    validMoves.push_back(king+ijs(in_ver,in_hor+1));
                else if (piece_type(board[in_ver][in_hor+1], turn) == -1)
                    validMoves.push_back(king+"x"+ijs(in_ver,in_hor+1));
        }
        return validMoves;
    }

    // If no check, then compute valid moves of King that could include Castling (see for checks)

    // For all pieces, attack Squares except the squares in which own piece is there
    // ^^ except the King
    // Make sure the piece is not pinned
    // If EnPassant is possible then include that

    return validMoves;
}
