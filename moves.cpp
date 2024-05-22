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
        vector<vector<char>> initialize(8);
        controlSquares.push_back(initialize);
    }
    oppControlSquares=controlSquares;
    turn = 0;
}
Moves::Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare)
{
    for (int i=0; i<8; ++i)
    {
        vector<char> initialize(8, '.');
        board.push_back(initialize);
    }
    for (int i=0; i<8; ++i)
    {
        vector<vector<char>> initialize(8);
        controlSquares.push_back(initialize);
    }
    fetch_Moves(board, turn, isEnPassant, epSquare);
}
void Moves::fetch_Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare)
{
    this->board = board;
    this->turn = turn;
    this->isEnPassant = isEnPassant;
    this->epSquare = epSquare;
}

vector<vector<vector<char>>> Moves::return_controlSquares()
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
           
              if(temp=='Q'){
                    int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back('Q'+offset);
                        temp_hor--;
                    }
                    if(temp_hor>=0)controlSquares[in_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back('Q'+offset);
                        temp_hor++;
                    }
                    if(temp_hor<8)controlSquares[in_ver][temp_hor].push_back('Q'+offset);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back('Q'+offset);
                        temp_ver--;
                    }
                    if(temp_ver>=0)controlSquares[temp_ver][in_hor].push_back('Q'+offset);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back('Q'+offset);
                        temp_ver++;
                    }
                    if(temp_ver<8)controlSquares[temp_ver][in_hor].push_back('Q'+offset);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back('Q'+offset);
              }
      
              if(temp=='R'){
                int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back('R'+offset);
                        temp_hor--;
                    }
                    if(temp_hor>=0)controlSquares[in_ver][temp_hor].push_back('R'+offset);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        controlSquares[in_ver][temp_hor].push_back('R'+offset);
                        temp_hor++;
                    }
                    if(temp_hor<8)controlSquares[in_ver][temp_hor].push_back('R'+offset);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back('R'+offset);
                        temp_ver--;
                    }
                    if(temp_ver>=0)controlSquares[temp_ver][in_hor].push_back('R'+offset);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        controlSquares[temp_ver][in_hor].push_back('R'+offset);
                        temp_ver++;
                    }
                    if(temp_ver<8)controlSquares[temp_ver][in_hor].push_back('R'+offset);
              }
      
              if(temp=='B'){
                    int temp_hor=in_hor-1;
                    int temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back('B'+offset);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back('B'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) controlSquares[temp_ver][temp_hor].push_back('B'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        controlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) controlSquares[temp_ver][temp_hor].push_back('B'+offset);
              }
       
              if(temp=='N'){
                if(in_ver>=2&&in_hor>=1)controlSquares[in_ver-2][in_hor-1].push_back('N'+offset);
                if(in_ver>=1&&in_hor>=2)controlSquares[in_ver-1][in_hor-2].push_back('N'+offset);
                if(in_ver>=2&&in_hor<7)controlSquares[in_ver-2][in_hor+1].push_back('N'+offset);
                if(in_ver>=1&&in_hor<6)controlSquares[in_ver-1][in_hor+2].push_back('N'+offset);
                if(in_ver<6&&in_hor>=1)controlSquares[in_ver+2][in_hor-1].push_back('N'+offset);
                if(in_ver<7&&in_hor>=2)controlSquares[in_ver+1][in_hor-2].push_back('N'+offset);
                if(in_ver<6&&in_hor<7)controlSquares[in_ver+2][in_hor+1].push_back('N'+offset);
                if(in_ver<7&&in_hor>=6)controlSquares[in_ver+1][in_hor+2].push_back('N'+offset);
              }
        
              if(temp=='P'){
                if(turn==0&&in_ver>0){
                    if(in_hor)controlSquares[in_ver-1][in_hor-1].push_back('P'+offset);
                    if(in_hor<7)controlSquares[in_ver-1][in_hor+1].push_back('P'+offset);
                }
                if(turn==1&&in_ver<7){
                    if(in_hor)controlSquares[in_ver+1][in_hor-1].push_back('P'+offset);
                    if(in_hor<7)controlSquares[in_ver+1][in_hor+1].push_back('P'+offset);
                }
              }
       }
    }
    return controlSquares;
}

vector<vector<vector<char>>> Moves::return_oppControlSquares()
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
           
              if(temp=='Q'){
                    int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back('Q'+offset);
                        temp_hor--;
                    }
                    if(temp_hor>=0)oppControlSquares[in_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back('Q'+offset);
                        temp_hor++;
                    }
                    if(temp_hor<8)oppControlSquares[in_ver][temp_hor].push_back('Q'+offset);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back('Q'+offset);
                        temp_ver--;
                    }
                    if(temp_ver>=0)oppControlSquares[temp_ver][in_hor].push_back('Q'+offset);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back('Q'+offset);
                        temp_ver++;
                    }
                    if(temp_ver<8)oppControlSquares[temp_ver][in_hor].push_back('Q'+offset);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back('Q'+offset);
              }
      
              if(temp=='R'){
                int temp_hor=in_hor-1;
                    while(in_hor>=0&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back('R'+offset);
                        temp_hor--;
                    }
                    if(temp_hor>=0)oppControlSquares[in_ver][temp_hor].push_back('R'+offset);

                    temp_hor=in_hor+1;
                    while(in_hor<8&&board[in_ver][temp_hor]=='.'){
                        oppControlSquares[in_ver][temp_hor].push_back('R'+offset);
                        temp_hor++;
                    }
                    if(temp_hor<8)oppControlSquares[in_ver][temp_hor].push_back('R'+offset);

                    int temp_ver=in_ver-1;
                     while(temp_ver>=0&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back('R'+offset);
                        temp_ver--;
                    }
                    if(temp_ver>=0)oppControlSquares[temp_ver][in_hor].push_back('R'+offset);

                    temp_ver=in_ver+1;
                     while(temp_ver<8&&board[temp_ver][in_hor]=='.'){
                        oppControlSquares[temp_ver][in_hor].push_back('R'+offset);
                        temp_ver++;
                    }
                    if(temp_ver<8)oppControlSquares[temp_ver][in_hor].push_back('R'+offset);
              }
      
              if(temp=='B'){
                    int temp_hor=in_hor-1;
                    int temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor--;
                        temp_ver--;
                    }
                    if(temp_hor>=0&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);

                    temp_hor=in_hor-1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor>=0&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor--;
                        temp_ver++;
                    }
                    if(temp_hor>=0&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver+1;
                    while(temp_ver<8&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor++;
                        temp_ver++;
                    }
                    if(temp_hor<8&&temp_ver<8) oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);

                    temp_hor=in_hor+1;
                    temp_ver=in_ver-1;
                    while(temp_ver>=0&&temp_hor<8&&board[temp_ver][temp_hor]=='.'){
                        oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);
                        temp_hor++;
                        temp_ver--;
                    }
                    if(temp_hor<8&&temp_ver>=0) oppControlSquares[temp_ver][temp_hor].push_back('B'+offset);
              }
       
              if(temp=='N'){
                if(in_ver>=2&&in_hor>=1)oppControlSquares[in_ver-2][in_hor-1].push_back('N'+offset);
                if(in_ver>=1&&in_hor>=2)oppControlSquares[in_ver-1][in_hor-2].push_back('N'+offset);
                if(in_ver>=2&&in_hor<7)oppControlSquares[in_ver-2][in_hor+1].push_back('N'+offset);
                if(in_ver>=1&&in_hor<6)oppControlSquares[in_ver-1][in_hor+2].push_back('N'+offset);
                if(in_ver<6&&in_hor>=1)oppControlSquares[in_ver+2][in_hor-1].push_back('N'+offset);
                if(in_ver<7&&in_hor>=2)oppControlSquares[in_ver+1][in_hor-2].push_back('N'+offset);
                if(in_ver<6&&in_hor<7)oppControlSquares[in_ver+2][in_hor+1].push_back('N'+offset);
                if(in_ver<7&&in_hor>=6)oppControlSquares[in_ver+1][in_hor+2].push_back('N'+offset);
              }
        
              if(temp=='P'){
                if(turn==0&&in_ver>0){
                    if(in_hor)oppControlSquares[in_ver-1][in_hor-1].push_back('P'+offset);
                    if(in_hor<7)oppControlSquares[in_ver-1][in_hor+1].push_back('P'+offset);
                }
                if(turn==1&&in_ver<7){
                    if(in_hor)oppControlSquares[in_ver+1][in_hor-1].push_back('P'+offset);
                    if(in_hor<7)oppControlSquares[in_ver+1][in_hor+1].push_back('P'+offset);
                }
              }
       }
    }
    return oppControlSquares;
}

void Moves::compute_Squares()
{
    /*
        First compute the attack Squares, and if isEnPassant is true, factor that into account.
        Then compute the attacked Squares (i.e. the attack squares of the opponent) in this position.
    */
}
