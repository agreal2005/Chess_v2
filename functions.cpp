#include "functions.h"
void display_board(vector<vector<char>> &board)
{
    for (int i=0; i<40; ++i)
    {
        if (i%5 == 0) cout << "+";
        else cout << "-";
    }
    cout << endl;
    for (int i=0; i<8; ++i)
    {
        for (int j=0; j<8; ++j)
        {
            cout << '|' << setw(2) << board[i][j] << setw(2) << '|'; 
        }
        cout << endl;
        for (int i=0; i<40; ++i)
        {
            if (i%5 == 0) cout << "+";
            else cout << "-";
        }
        cout << endl;
    }
}
bool valid_rank(char ch)
{
    if (ch-'0' >= 1 && ch - '0' <= 8)
    {
        return true;
    }
    return false;
}
bool valid_file(char ch)
{
    switch(ch)
    {
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
        case 'g': case 'h':
        return true;
        default:
        return false;
    }
}
bool valid_piece_code(char ch)
{
    switch (ch)
    {
        case 'p': case 'P':
        case 'r': case 'R':
        case 'n': case 'N':
        case 'b': case 'B':
        case 'q': case 'Q':
        case 'k': case 'K':
        return true;
        default:
        return false;
    }
}

string ijs(int i, int j)
{
    string squares[8][8] = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    };
    return squares[i][j];
}

pair<int,int> sij(string s)
{
    int j = s[0] - 'a';
    int i = ('8' - s[1]);
    return {i,j};
}

int piece_type(char ch, bool turn)
{
    if (ch == '.') return 0;
    if (turn == 0)
    {
        if (ch >= 'a' && ch <= 'z') return -1;
        else return 1;
    }
    if (turn == 1 && ch>='a' && ch <= 'z') return 1;
    return -1;
}

vector<vector<char>> reverseBoard(const vector<vector<char>> &board)
{
    vector<vector<char>> rev(8, vector<char>(8));
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            rev[i][j] = board[7-i][j];
        }
    }
    return rev;
}

int get_material(const vector<vector<char>> &board)
{
    int white_score = 0;
    int black_score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            switch (board[i][j])
            {
            case 'P':
                white_score += 1;
                break;
            case 'N':
                white_score += 3;
                break;
            case 'R':
                white_score += 5;
                break;
            case 'B':
                white_score += 3;
                break;
            case 'Q':
                white_score += 9;
                break;

            case 'p':
                black_score += 1;
                break;
            case 'n':
                black_score += 3;
                break;
            case 'r':
                black_score += 5;
                break;
            case 'b':
                black_score += 3;
                break;
            case 'q':
                black_score += 9;
                break;

            default:
                break;
            }
        }
    }
    return white_score + black_score;
}

string get_FEN(vector<vector<char>> brd, bool t, bool wck, bool wcq, bool bck, bool bcq, bool isEnp, string epS, int hfc, int fms)
{
    string str = "";
    for (int i = 0; i < 8; ++i)
    {
        int gap = 0;
        for (int j = 0; j < 8; ++j)
        {
            if (brd[i][j] == '.')
            {
                gap++;
            }
            else
            {
                if (gap > 0)
                {
                    str.push_back('0' + gap);
                    gap = 0;
                }
                str.push_back(brd[i][j]);
            }
        }
        if (gap > 0)
        {
            str.push_back('0' + gap);
            gap = 0;
        }
        if (i < 7)
        {
            str.push_back('/');
        }
    }
    str.push_back(' ');
    if (t == 0)
        str.push_back('w');
    else
        str.push_back('b');
    str.push_back(' ');
    if (wck)
        str.push_back('K');
    if (wcq)
        str.push_back('Q');
    if (bck)
        str.push_back('k');
    if (bcq)
        str.push_back('q');
    if (!(bcq || bck || wcq || wck))
        str.push_back('-');
    str.push_back(' ');
    if (!isEnp)
        str.push_back('-');
    else
        str += epS;
    str.push_back(' ');
    str += to_string(hfc);
    str.push_back(' ');
    str += to_string(fms);
    return str;
}

string stdToV2(string fen, string std_move)
{
    GameState Board(fen);
    vector<vector<char>> board = Board.return_board();
    bool turn = Board.return_turn();
    string v2_move;
    if(std_move.substr(0,3)=="O-O" || std_move.substr(0,3)=="o-o")
    {
        return std_move.substr(0,3);
    }
    if(std_move.substr(0,5)=="O-O-O" || std_move.substr(0,5)=="o-o-o")
    {
        return std_move.substr(0,5 );
    }
    if(turn==0)     // white to move
    {
        char piece = std_move[0];
        string initial_position;
        string next_position;
        if(piece == 'Q' || piece=='B' 
            || piece=='R' || piece=='K' 
            || piece=='N')
        // non pawn piece
        {
            if(std_move[1]=='x') // capture
            {
                next_position = std_move.substr(2,2);
                pair<int,int> next_pos_ij = sij(next_position);
                if(piece=='Q' || piece=='K')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece)
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='R')
                {
                    for(int i=0;i<8;i++)
                    {
                        if(board[i][next_pos_ij.second]==piece)
                            initial_position = ijs(i, next_pos_ij.second);
                    }
                    for(int j=0;j<8;j++)
                    {
                        if(board[next_pos_ij.first][j]==piece)
                            initial_position = ijs(next_pos_ij.first, j);
                    }
                }
                else if(piece=='B')
                {

                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && abs(next_pos_ij.first-i)==abs(next_pos_ij.second-j))
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='N')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && (abs(next_pos_ij.first-i)==1 && (abs(next_pos_ij.second-j)==2)||(abs(next_pos_ij.first-i)==2 && abs(next_pos_ij.second-j)==1)))
                            {
                                initial_position = ijs(i,j);
                            }
                        }
                    }
                }
                v2_move = "";
                v2_move += piece;
                v2_move += initial_position;
                v2_move += 'x';
                v2_move += next_position;
                return v2_move;
            }
            else if(std_move[2]>='1' && std_move[2]<='8')   // normal move
            {
                next_position = std_move.substr(1,2);
                pair<int,int> next_pos_ij = sij(next_position);
                if(piece=='Q' || piece=='K')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece)
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='R')
                {
                    for(int i=0;i<8;i++)
                    {
                        if(board[i][next_pos_ij.second]==piece)
                            initial_position = ijs(i, next_pos_ij.second);
                    }
                    for(int j=0;j<8;j++)
                    {
                        if(board[next_pos_ij.first][j]==piece)
                            initial_position = ijs(next_pos_ij.first, j);
                    }
                }
                else if(piece=='B')
                {

                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && abs(next_pos_ij.first-i)==abs(next_pos_ij.second-j))
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='N')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && (abs(next_pos_ij.first-i)==1 && (abs(next_pos_ij.second-j)==2)||(abs(next_pos_ij.first-i)==2 && abs(next_pos_ij.second-j)==1)))
                            {
                                initial_position = ijs(i,j);
                            }
                        }
                    }
                }
                v2_move = "";
                v2_move += piece;
                v2_move += initial_position;
                v2_move += next_position;
                return v2_move;
            }
            else    // contention case where two pieces can move, happens with rook only ig
            {
                next_position = std_move.substr(2,2);
                pair<int,int> next_pos_ij = sij(next_position);
                char pos = std_move[1];
                if(piece=='R')
                {
                    if(pos>='1' && pos<='8')
                    {
                        initial_position = "";
                        initial_position += next_position[0];
                        initial_position += pos;
                    }
                    else
                    {
                        initial_position = "";
                        initial_position += pos;
                        initial_position += next_position[1];
                    }
                }
                v2_move = "";
                v2_move += piece;
                v2_move += initial_position;
                v2_move += next_position;
                return v2_move;
            }
        }
        else    // pawn piece
        {
            if(std_move[1]=='x') // capture
            {
                next_position = std_move.substr(2,2);
                pair<int,int> next_pos_ij = sij(next_position);
                char col_of_sq1 = std_move[0];
                if(std_move[4]=='=')    // promotion
                {

                }
                else
                {
                    initial_position = "";
                    initial_position += col_of_sq1;
                    initial_position += next_position[1];
                    v2_move = "";
                    v2_move += "P";
                    v2_move += initial_position;
                    v2_move += "x";
                    v2_move += next_position;
                    return v2_move;
                }
            }
            else
            {
                next_position = std_move.substr(0,2);
                pair<int,int> next_pos_ij = sij(next_position);
                if(std_move[2]=='=')    // promotion
                {

                }
                else
                {
                    if(board[next_pos_ij.first+1][next_pos_ij.second]=='P')     // single move
                    {
                        initial_position = ijs(next_pos_ij.first+1, next_pos_ij.second);
                        v2_move = "";
                        v2_move += "P";
                        v2_move += initial_position;
                        v2_move += next_position;
                        return v2_move;
                    }
                    else if(board[next_pos_ij.first+2][next_pos_ij.second]=='P')
                    {
                        initial_position = ijs(next_pos_ij.first+2, next_pos_ij.second);
                        v2_move = "";
                        v2_move += "P";
                        v2_move += initial_position;
                        v2_move += "Z";
                        v2_move += next_position;
                        return v2_move;
                    }
                }
            }
        }
    }
    else
    {
        char piece = std_move[0];
        piece = piece - 'A' + 'a';
        string initial_position;
        string next_position;
        if(piece == 'q' || piece=='b'
            || piece=='r' || piece=='k' 
            || piece=='n')
        // non pawn piece
        {
            if(std_move[1]=='x') // capture
            {
                next_position = std_move.substr(2,2);
                pair<int,int> next_pos_ij = sij(next_position);
                if(piece=='q' || piece=='k')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece)
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='r')
                {
                    for(int i=0;i<8;i++)
                    {
                        if(board[i][next_pos_ij.second]==piece)
                            initial_position = ijs(i, next_pos_ij.second);
                    }
                    for(int j=0;j<8;j++)
                    {
                        if(board[next_pos_ij.first][j]==piece)
                            initial_position = ijs(next_pos_ij.first, j);
                    }
                }
                else if(piece=='b')
                {

                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && abs(next_pos_ij.first-i)==abs(next_pos_ij.second-j))
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='n')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && (abs(next_pos_ij.first-i)==1 && (abs(next_pos_ij.second-j)==2)||(abs(next_pos_ij.first-i)==2 && abs(next_pos_ij.second-j)==1)))
                            {
                                initial_position = ijs(i,j);
                            }
                        }
                    }
                }
                v2_move = "";
                v2_move += piece;
                v2_move += initial_position;
                v2_move += 'x';
                v2_move += next_position;
                return v2_move;
            }
            else if(std_move[2]>='1' && std_move[2]<='8')   // normal move
            {
                next_position = std_move.substr(1,2);
                pair<int,int> next_pos_ij = sij(next_position);
                if(piece=='q' || piece=='k')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece)
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='r')
                {
                    for(int i=0;i<8;i++)
                    {
                        if(board[i][next_pos_ij.second]==piece)
                            initial_position = ijs(i, next_pos_ij.second);
                    }
                    for(int j=0;j<8;j++)
                    {
                        if(board[next_pos_ij.first][j]==piece)
                            initial_position = ijs(next_pos_ij.first, j);
                    }
                }
                else if(piece=='b')
                {

                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && abs(next_pos_ij.first-i)==abs(next_pos_ij.second-j))
                                initial_position = ijs(i,j);
                        }
                    }
                }
                else if(piece=='n')
                {
                    for(int i=0;i<8;i++)
                    {
                        for(int j=0;j<8;j++)
                        {
                            if(board[i][j]==piece && (abs(next_pos_ij.first-i)==1 && (abs(next_pos_ij.second-j)==2)||(abs(next_pos_ij.first-i)==2 && abs(next_pos_ij.second-j)==1)))
                            {
                                initial_position = ijs(i,j);
                            }
                        }
                    }
                }
                v2_move = "";
                v2_move += piece;
                v2_move += initial_position;
                v2_move += next_position;
                return v2_move;
            }
            else    // contention case where two pieces can move, happens with rook only ig
            {
                next_position = std_move.substr(2,2);
                pair<int,int> next_pos_ij = sij(next_position);
                char pos = std_move[1];
                if(piece=='R')
                {
                    if(pos>='1' && pos<='8')
                    {
                        initial_position = "";
                        initial_position += next_position[0];
                        initial_position += pos;
                    }
                    else
                    {
                        initial_position = "";
                        initial_position += pos;
                        initial_position += next_position[1];
                    }
                }
                v2_move = "";
                v2_move += piece;
                v2_move += initial_position;
                v2_move += next_position;
                return v2_move;
            }
        }
        else    // pawn piece
        {
            if(std_move[1]=='x') // capture
            {
                next_position = std_move.substr(2,2);
                pair<int,int> next_pos_ij = sij(next_position);
                char col_of_sq1 = std_move[0];
                if(std_move[4]=='=')    // promotion
                {

                }
                else
                {
                    initial_position = "";
                    initial_position += col_of_sq1;
                    initial_position += next_position[1];
                    v2_move = "";
                    v2_move += "p";
                    v2_move += initial_position;
                    v2_move += "x";
                    v2_move += next_position;
                    return v2_move;
                }
            }
            else
            {
                next_position = std_move.substr(0,2);
                pair<int,int> next_pos_ij = sij(next_position);
                if(std_move[2]=='=')    // promotion
                {

                }
                else
                {
                    if(board[next_pos_ij.first-1][next_pos_ij.second]=='p')     // single move
                    {
                        initial_position = ijs(next_pos_ij.first-1, next_pos_ij.second);
                        v2_move = "";
                        v2_move += "p";
                        v2_move += initial_position;
                        v2_move += next_position;
                        return v2_move;
                    }
                    else if(board[next_pos_ij.first-2][next_pos_ij.second]=='p')
                    {
                        initial_position = ijs(next_pos_ij.first-2, next_pos_ij.second);
                        v2_move = "";
                        v2_move += "p";
                        v2_move += initial_position;
                        v2_move += "z";
                        v2_move += next_position;
                        return v2_move;
                    }
                }
            }
        }
    }

    return "";
}

// impossible moves are returned as empty string
// still need to check if it is valid 
string convertToMv(string move, vector<vector<char>>& brd, bool turn){
    string mv = ""; // according to our convention

    move.erase(
        std::remove_if(move.begin(), move.end(),
            [](char c){ return c == '!' || c == '?'; }),
        move.end()
    );


    // castling
    if(move == "O-O"){
        mv = move;
        if(turn == 1){
            mv = "o-o";
        }
    }
    else if(move == "O-O-O"){
        mv = move;
        if(turn == 1){
            mv = "o-o-o";
        }
    }
    
    string suffix = "";
    if(move.back() == '+' || move.back() == '#'){
        suffix += move.back();
        move.pop_back();
    }

    auto check_for_piece = [&brd](vector<pair<int, int>>& to_check, char piece_symbol) -> vector<string>{
        vector<string> ans;
        for(auto p : to_check){
            if(p.first < 0 || p.first > 7 || p.second < 0 || p.second > 7)
                continue;
            if(brd[p.first][p.second] == piece_symbol){
                string pos;
                pos += p.second + 'a';
                pos += 7 - p.first + '1';
                ans.push_back(pos);
            }
        }
        return ans;
    };

    // expects move to have +, #, = (promotions) removed
    auto get_move = [&move, turn](std::function<vector<string>(int, int)> get_possible_origins, char symbol) -> string{
        string mv;
        mv.push_back(symbol);

        int ind = move.length()-1;
        int x = 7 - (move[ind--] - '1');
        int y = move[ind--] - 'a';

        string captureMarker = "";
        if(move[ind] == 'x'){
            if(turn == 0)
                captureMarker.push_back('X');
            else
                captureMarker.push_back('x');
            ind--;
        }

        string disambiguator = "";
        while(ind >= 0 && move[ind] != symbol){
            disambiguator.push_back(move[ind]);
            ind--;
        }
        reverse(disambiguator.begin(), disambiguator.end());

        vector<string> possible_origins = get_possible_origins(x, y);
        string origin = "";
        int num_possible_origins = 0;
        for(auto str : possible_origins){
            if(str.find(disambiguator) != string::npos){
                origin = str;
                num_possible_origins++;
            }
        }
        if(num_possible_origins != 1){
            cout<<"Multiple origins detected"<<endl;
            abort();
        }

        mv += origin + captureMarker + move.substr(move.length()-2, 2);

        return mv;
    };

    // Knight move
    if(move[0] == 'N'){
        char symbol = turn==0 ? 'N' : 'n';

        auto possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
            vector<pair<int, int>> to_check = {
                {x+2, y+1},
                {x+2, y-1},
                {x-2, y+1},
                {x-2, y-1},
                {x+1, y+2},
                {x+1, y-2},
                {x-1, y+2},
                {x-1, y-2},
            };
            return check_for_piece(to_check, symbol);
        };

        mv = get_move(possible_origin, symbol);
    }

    // Bishop move
    else if(move[0] == 'B'){
        char symbol = turn==0 ? 'B' : 'b';
        
        auto possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
            vector<pair<int, int>> to_check;
            int i, j;
            if(x+y < 8){
                i = x+y;
                j = 0;
            }
            else{
                i = 7;
                j = x+y - 7;
            }
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                i--;
                j++;
            }

            if(x-y < 0){
                i = 0;
                j = y-x;
            }
            else{
                i = x-y;
                j = 0;
            }
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                i++;
                j++;
            }
            return check_for_piece(to_check, symbol);
        };

        mv = get_move(possible_origin, symbol);
    }

    // Rook move
    else if(move[0] == 'R'){
        char symbol = turn==0 ? 'R' : 'r';
        
        auto possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
            vector<pair<int, int>> to_check;
            int i, j;
            i=x;
            j=0;
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                j++;
            }

            i=0;
            j=y;
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                i++;
            }
            return check_for_piece(to_check, symbol);
        };

        mv = get_move(possible_origin, symbol);
    }

    // Queen move
    else if(move[0] == 'Q'){
        char symbol = turn==0 ? 'Q' : 'q';
        
        auto possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
            vector<pair<int, int>> to_check;
            int i, j;
            i=x;
            j=0;
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                j++;
            }

            i=0;
            j=y;
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                i++;
            }

            if(x+y < 8){
                i = x+y;
                j = 0;
            }
            else{
                i = 7;
                j = x+y - 7;
            }
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                i--;
                j++;
            }

            if(x-y < 0){
                i = 0;
                j = y-x;
            }
            else{
                i = x-y;
                j = 0;
            }
            while(i>=0 && i<8 && j>=0 && j<8){
                to_check.push_back({i, j});
                i++;
                j++;
            }

            vector<string> possibilities = check_for_piece(to_check, symbol);
            return possibilities;
        };

        mv = get_move(possible_origin, symbol);
    }

    // King move
    else if(move[0] == 'K'){
        char symbol = turn==0 ? 'K' : 'k';

        auto possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
            vector<pair<int, int>> to_check = {
                {x+1, y+1},
                {x+1, y-1},
                {x-1, y+1},
                {x-1, y-1},
                {x+1,  y },
                {x-1,  y },
                { x , y+1},
                { x , y-1},
            };
            return check_for_piece(to_check, symbol);
        };

        mv = get_move(possible_origin, symbol);
    }

    // Pawn move
    else {
        char symbol = turn==0 ? 'P' : 'p';

        // En Passant handling
        if(move.find(" e.p.") != string::npos){
            auto possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
                int dir = (turn == 0 ? 1 : -1);
                vector<pair<int, int>> to_check = {
                    {x+dir, y-1},
                    {x+dir, y+1},
                };
                return check_for_piece(to_check, symbol);
            };
            move = move.substr(0, move.find(" e.p."));
            mv = get_move(possible_origin, symbol);
            replace(mv.begin(), mv.end(), 'X', 'Y');
            replace(mv.begin(), mv.end(), 'x', 'y');
        }
        else if(move.find("=") != string::npos){ // promotion
            function<vector<string>(int, int)> possible_origin;
            
            if(move.find("x") != string::npos){ // kill promotion
                possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
                    int dir = (turn == 0 ? 1 : -1);
                    vector<pair<int, int>> to_check = {
                        {x+dir, y-1},
                        {x+dir, y+1},
                    };
                    return check_for_piece(to_check, symbol);
                };
            } 
            else {
                possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
                    int dir = (turn == 0 ? 1 : -1);
                    vector<pair<int, int>> to_check = {
                        {x+dir, y},
                    };
                    return check_for_piece(to_check, symbol);
                };
            }
            char promoted_to = move.back();
            move = move.substr(0, move.find("="));
            mv = get_move(possible_origin, symbol);
            mv.push_back(promoted_to);
        }
        else { // normal pawn move
            function<vector<string>(int, int)> possible_origin;
            if(move.find("x") != string::npos){
                possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
                    int dir = (turn == 0 ? 1 : -1);
                    vector<pair<int, int>> to_check = {
                        {x+dir, y-1},
                        {x+dir, y+1},
                    };
                    return check_for_piece(to_check, symbol);
                };
            }
            else {
                possible_origin = [&brd, turn, symbol, &check_for_piece](int x, int y) -> vector<string>{
                    int dir = (turn == 0 ? 1 : -1);
                    vector<pair<int, int>> to_check = {
                        {x+dir, y},
                    };
                    if(turn == 0 && x == 4)
                        to_check.push_back({x+2*dir, y});
                    if(turn == 1 && x == 3)
                        to_check.push_back({x+2*dir, y});
                    return check_for_piece(to_check, symbol);
                };
            }
            mv = get_move(possible_origin, symbol);
            if(mv[2] == '2' && mv[4] == '4')
                mv.insert(mv.begin()+3, 'Z');
            if(mv[2] == '7' && mv[4] == '5')
                mv.insert(mv.begin()+3, 'z');
        }
    }


    mv += suffix;
    return mv;
}