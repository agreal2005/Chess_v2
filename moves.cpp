#include "moves.h"
Moves::Moves()
{
    for (int i = 0; i < 8; ++i)
    {
        vector<char> initialize(8, '.');
        board.push_back(initialize);
    }
    for (int i = 0; i < 8; ++i)
    {
        vector<vector<Piece>> initialize(8);
        controlSquares.push_back(initialize);
    }
    oppControlSquares = controlSquares;
    turn = 0;
    wck = wcq = bck = bcq = false;
    king_square = sij("e1");
    oppKing_square = sij("e8");
}

Moves::Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare, int castling)
{
    fetch_Moves(board, turn, isEnPassant, epSquare, castling);
}

void Moves::fetch_Moves(vector<vector<char>> board, bool turn, bool isEnPassant, string epSquare, int castling)
{
    for (int i = 0; i < 8; ++i)
    {
        vector<vector<Piece>> initialize(8);
        controlSquares.push_back(initialize);
    }
    oppControlSquares = controlSquares;
    this->board = board;
    this->turn = turn;
    this->isEnPassant = isEnPassant;
    this->epSquare = epSquare;
    if (castling & 1)
        bcq = 1;
    if ((castling >> 1) & 1)
        bck = 1;
    if ((castling >> 2) & 1)
        wcq = 1;
    if ((castling >> 3) & 1)
        wck = 1;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (piece_type(board[i][j], turn) > 0)
            {
                pieces.push_back(Piece(board[i][j], i, j));
            }
            else if (piece_type(board[i][j], turn) < 0)
            {
                oppPieces.push_back(Piece(board[i][j], i, j));
            }
            if (board[i][j] == 'K')
            {
                if (turn == 0)
                    king_square = {i, j};
                else
                    oppKing_square = {i, j};
            }
            if (board[i][j] == 'k')
            {
                if (turn == 1)
                    king_square = {i, j};
                else
                    oppKing_square = {i, j};
            }
        }
    }
    checkPinned();
    checkOppPinned();
    return_controlSquares();
    return_oppControlSquares();
}

vector<vector<vector<Piece>>> Moves::return_controlSquares()
{
    int offset = turn ? 32 : 0;
    for (int in_ver = 0; in_ver < 8; in_ver++)
    {
        for (int in_hor = 0; in_hor < 8; in_hor++)
        {
            controlSquares[in_ver][in_hor].clear();
        }
    }

    for (int in_ver = 0; in_ver < 8; in_ver++)
    {
        for (int in_hor = 0; in_hor < 8; in_hor++)
        {
            if (board[in_ver][in_hor] == '.')
                continue;
            char temp = board[in_ver][in_hor] - offset;

            Piece piece(board[in_ver][in_hor], in_ver, in_hor);
            for (auto p : pieces)
            {
                if (piece.i == p.i && piece.j == p.j)
                {
                    piece = p;
                    break;
                }
            }
            if (piece.pinned)
            {
                if (piece.pini == -1)
                {
                    int pinx = piece.resi, piny = piece.resj; // pinning piece's coords
                    int i_inc = 0, j_inc = 0;
                    if (pinx > king_square.first)
                        i_inc = -1;
                    else if (pinx < king_square.first)
                        i_inc = 1;
                    if (piny > king_square.second)
                        j_inc = -1;
                    else if (piny < king_square.second)
                        j_inc = 1;
                    if (piece.type == 'P' || piece.type == 'p')
                    {
                        if (j_inc != 0)
                        {
                            controlSquares[pinx][piny].push_back(piece);
                        }
                    }
                    else
                    {
                        int x = pinx, y = piny;
                        while (x != king_square.first && y != king_square.second)
                        {
                            if (x != piece.i || y != piece.j)
                            {
                                controlSquares[x][y].push_back(piece);
                            }
                            x += i_inc;
                            y += j_inc;
                        }
                    }
                }
                continue;
            }

            if (temp == 'Q')
            {

                int temp_hor = in_hor - 1;
                while (in_hor >= 0 && board[in_ver][temp_hor] == '.')
                {
                    controlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor--;
                }
                if (temp_hor >= 0)
                    controlSquares[in_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                while (in_hor < 8 && board[in_ver][temp_hor] == '.')
                {
                    controlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor++;
                }
                if (temp_hor < 8)
                    controlSquares[in_ver][temp_hor].push_back(piece);

                int temp_ver = in_ver - 1;
                while (temp_ver >= 0 && board[temp_ver][in_hor] == '.')
                {
                    controlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver--;
                }
                if (temp_ver >= 0)
                    controlSquares[temp_ver][in_hor].push_back(piece);

                temp_ver = in_ver + 1;
                while (temp_ver < 8 && board[temp_ver][in_hor] == '.')
                {
                    controlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver++;
                }
                if (temp_ver < 8)
                    controlSquares[temp_ver][in_hor].push_back(piece);

                temp_hor = in_hor - 1;
                temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver--;
                }
                if (temp_hor >= 0 && temp_ver >= 0)
                    controlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor - 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver++;
                }
                if (temp_hor >= 0 && temp_ver < 8)
                    controlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver++;
                }
                if (temp_hor < 8 && temp_ver < 8)
                    controlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver--;
                }
                if (temp_hor < 8 && temp_ver >= 0)
                    controlSquares[temp_ver][temp_hor].push_back(piece);
            }

            if (temp == 'R')
            {
                int temp_hor = in_hor - 1;
                while (in_hor >= 0 && board[in_ver][temp_hor] == '.')
                {
                    controlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor--;
                }
                if (temp_hor >= 0)
                    controlSquares[in_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                while (in_hor < 8 && board[in_ver][temp_hor] == '.')
                {
                    controlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor++;
                }
                if (temp_hor < 8)
                    controlSquares[in_ver][temp_hor].push_back(piece);

                int temp_ver = in_ver - 1;
                while (temp_ver >= 0 && board[temp_ver][in_hor] == '.')
                {
                    controlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver--;
                }
                if (temp_ver >= 0)
                    controlSquares[temp_ver][in_hor].push_back(piece);

                temp_ver = in_ver + 1;
                while (temp_ver < 8 && board[temp_ver][in_hor] == '.')
                {
                    controlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver++;
                }
                if (temp_ver < 8)
                    controlSquares[temp_ver][in_hor].push_back(piece);
            }

            if (temp == 'B')
            {
                int temp_hor = in_hor - 1;
                int temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver--;
                }
                if (temp_hor >= 0 && temp_ver >= 0)
                    controlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor - 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver++;
                }
                if (temp_hor >= 0 && temp_ver < 8)
                    controlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver++;
                }
                if (temp_hor < 8 && temp_ver < 8)
                    controlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    controlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver--;
                }
                if (temp_hor < 8 && temp_ver >= 0)
                    controlSquares[temp_ver][temp_hor].push_back(piece);
            }

            if (temp == 'N')
            {
                if (in_ver >= 2 && in_hor >= 1)
                    controlSquares[in_ver - 2][in_hor - 1].push_back(piece);
                if (in_ver >= 1 && in_hor >= 2)
                    controlSquares[in_ver - 1][in_hor - 2].push_back(piece);
                if (in_ver >= 2 && in_hor < 7)
                    controlSquares[in_ver - 2][in_hor + 1].push_back(piece);
                if (in_ver >= 1 && in_hor < 6)
                    controlSquares[in_ver - 1][in_hor + 2].push_back(piece);
                if (in_ver < 6 && in_hor >= 1)
                    controlSquares[in_ver + 2][in_hor - 1].push_back(piece);
                if (in_ver < 7 && in_hor >= 2)
                    controlSquares[in_ver + 1][in_hor - 2].push_back(piece);
                if (in_ver < 6 && in_hor < 7)
                    controlSquares[in_ver + 2][in_hor + 1].push_back(piece);
                if (in_ver < 7 && in_hor < 6)
                {
                    controlSquares[in_ver + 1][in_hor + 2].push_back(piece);
                }
            }

            if (temp == 'P')
            {
                if (turn == 0 && in_ver > 0)
                {
                    if (in_hor)
                        controlSquares[in_ver - 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        controlSquares[in_ver - 1][in_hor + 1].push_back(piece);
                }
                if (turn == 1 && in_ver < 7)
                {
                    if (in_hor)
                        controlSquares[in_ver + 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        controlSquares[in_ver + 1][in_hor + 1].push_back(piece);
                }
            }

            if (temp == 'K')
            {
                if (in_ver > 0)
                {
                    if (in_hor > 0)
                        controlSquares[in_ver - 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        controlSquares[in_ver - 1][in_hor + 1].push_back(piece);
                    controlSquares[in_ver - 1][in_hor].push_back(piece);
                }
                if (in_ver < 7)
                {
                    if (in_hor > 0)
                        controlSquares[in_ver + 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        controlSquares[in_ver + 1][in_hor + 1].push_back(piece);
                    controlSquares[in_ver + 1][in_hor].push_back(piece);
                }
                if (in_hor > 0)
                    controlSquares[in_ver][in_hor - 1].push_back(piece);
                if (in_hor < 7)
                    controlSquares[in_ver][in_hor + 1].push_back(piece);
            }
        }
    }
    controlSquares[king_square.first][king_square.second].clear();
    // for (int i=0; i<8; ++i)
    // {
    //     for (int j=0; j<8; ++j)
    //     {
    //         for (auto p : controlSquares[i][j])
    //         {
    //             cout << ijs(i,j) << " " << p.type << " at " << ijs(p.i, p.j) << endl;
    //         }
    //     }
    // }
    return controlSquares;
}

vector<vector<vector<Piece>>> Moves::return_oppControlSquares()
{
    int offset = turn ? 0 : 32;
    for (int in_ver = 0; in_ver < 8; in_ver++)
    {
        for (int in_hor = 0; in_hor < 8; in_hor++)
        {
            oppControlSquares[in_ver][in_hor].clear();
        }
    }

    for (int in_ver = 0; in_ver < 8; in_ver++)
    {
        for (int in_hor = 0; in_hor < 8; in_hor++)
        {
            if (board[in_ver][in_hor] == '.')
                continue;
            char temp = board[in_ver][in_hor] - offset;
            Piece piece(board[in_ver][in_hor], in_ver, in_hor);
            for (auto p : oppPieces)
            {
                if (piece.i == p.i && piece.j == p.j)
                {
                    piece = p;
                    break;
                }
            }
            if (piece.pinned)
            {
                if (piece.pini == -1)
                {
                    int pinx = piece.resi, piny = piece.resj; // pinning piece's coords
                    int i_inc = 0, j_inc = 0;
                    if (pinx > king_square.first)
                        i_inc = -1;
                    else if (pinx < king_square.first)
                        i_inc = 1;
                    if (piny > king_square.second)
                        j_inc = -1;
                    else if (piny < king_square.second)
                        j_inc = 1;
                    if (piece.type == 'P' || piece.type == 'p')
                    {
                        if (j_inc != 0)
                        {
                            oppControlSquares[pinx][piny].push_back(piece);
                        }
                    }
                    else
                    {
                        int x = pinx, y = piny;
                        while (x != king_square.first && y != king_square.second)
                        {
                            if (x != piece.i || y != piece.j)
                            {
                                oppControlSquares[x][y].push_back(piece);
                            }
                            x += i_inc;
                            y += j_inc;
                        }
                    }
                }
                continue;
            }
            if (temp == 'Q')
            {
                int temp_hor = in_hor - 1;
                while (in_hor >= 0 && board[in_ver][temp_hor] == '.')
                {
                    oppControlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor--;
                }
                if (temp_hor >= 0)
                    oppControlSquares[in_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                while (in_hor < 8 && board[in_ver][temp_hor] == '.')
                {
                    oppControlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor++;
                }
                if (temp_hor < 8)
                    oppControlSquares[in_ver][temp_hor].push_back(piece);

                int temp_ver = in_ver - 1;
                while (temp_ver >= 0 && board[temp_ver][in_hor] == '.')
                {
                    oppControlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver--;
                }
                if (temp_ver >= 0)
                    oppControlSquares[temp_ver][in_hor].push_back(piece);

                temp_ver = in_ver + 1;
                while (temp_ver < 8 && board[temp_ver][in_hor] == '.')
                {
                    oppControlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver++;
                }
                if (temp_ver < 8)
                    oppControlSquares[temp_ver][in_hor].push_back(piece);

                temp_hor = in_hor - 1;
                temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver--;
                }
                if (temp_hor >= 0 && temp_ver >= 0)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor - 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver++;
                }
                if (temp_hor >= 0 && temp_ver < 8)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver++;
                }
                if (temp_hor < 8 && temp_ver < 8)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver--;
                }
                if (temp_hor < 8 && temp_ver >= 0)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
            }

            if (temp == 'R')
            {
                int temp_hor = in_hor - 1;
                while (in_hor >= 0 && board[in_ver][temp_hor] == '.')
                {
                    oppControlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor--;
                }
                if (temp_hor >= 0)
                    oppControlSquares[in_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                while (in_hor < 8 && board[in_ver][temp_hor] == '.')
                {
                    oppControlSquares[in_ver][temp_hor].push_back(piece);
                    temp_hor++;
                }
                if (temp_hor < 8)
                    oppControlSquares[in_ver][temp_hor].push_back(piece);

                int temp_ver = in_ver - 1;
                while (temp_ver >= 0 && board[temp_ver][in_hor] == '.')
                {
                    oppControlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver--;
                }
                if (temp_ver >= 0)
                    oppControlSquares[temp_ver][in_hor].push_back(piece);

                temp_ver = in_ver + 1;
                while (temp_ver < 8 && board[temp_ver][in_hor] == '.')
                {
                    oppControlSquares[temp_ver][in_hor].push_back(piece);
                    temp_ver++;
                }
                if (temp_ver < 8)
                    oppControlSquares[temp_ver][in_hor].push_back(piece);
            }

            if (temp == 'B')
            {
                int temp_hor = in_hor - 1;
                int temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver--;
                }
                if (temp_hor >= 0 && temp_ver >= 0)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor - 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor >= 0 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor--;
                    temp_ver++;
                }
                if (temp_hor >= 0 && temp_ver < 8)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver + 1;
                while (temp_ver < 8 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver++;
                }
                if (temp_hor < 8 && temp_ver < 8)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);

                temp_hor = in_hor + 1;
                temp_ver = in_ver - 1;
                while (temp_ver >= 0 && temp_hor < 8 && board[temp_ver][temp_hor] == '.')
                {
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
                    temp_hor++;
                    temp_ver--;
                }
                if (temp_hor < 8 && temp_ver >= 0)
                    oppControlSquares[temp_ver][temp_hor].push_back(piece);
            }

            if (temp == 'N')
            {
                if (in_ver >= 2 && in_hor >= 1)
                    oppControlSquares[in_ver - 2][in_hor - 1].push_back(piece);
                if (in_ver >= 1 && in_hor >= 2)
                    oppControlSquares[in_ver - 1][in_hor - 2].push_back(piece);
                if (in_ver >= 2 && in_hor < 7)
                    oppControlSquares[in_ver - 2][in_hor + 1].push_back(piece);
                if (in_ver >= 1 && in_hor < 6)
                    oppControlSquares[in_ver - 1][in_hor + 2].push_back(piece);
                if (in_ver < 6 && in_hor >= 1)
                    oppControlSquares[in_ver + 2][in_hor - 1].push_back(piece);
                if (in_ver < 7 && in_hor >= 2)
                    oppControlSquares[in_ver + 1][in_hor - 2].push_back(piece);
                if (in_ver < 6 && in_hor < 7)
                    oppControlSquares[in_ver + 2][in_hor + 1].push_back(piece);
                if (in_ver < 7 && in_hor < 6)
                    oppControlSquares[in_ver + 1][in_hor + 2].push_back(piece);
            }

            if (temp == 'P')
            {
                if (turn == 0 && in_ver > 0)
                {
                    if (in_hor)
                        oppControlSquares[in_ver + 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        oppControlSquares[in_ver + 1][in_hor + 1].push_back(piece);
                }
                if (turn == 1 && in_ver < 7)
                {
                    if (in_hor)
                        oppControlSquares[in_ver - 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        oppControlSquares[in_ver - 1][in_hor + 1].push_back(piece);
                }
            }

            if (temp == 'K')
            {
                if (in_ver > 0)
                {
                    if (in_hor > 0)
                        oppControlSquares[in_ver - 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        oppControlSquares[in_ver - 1][in_hor + 1].push_back(piece);
                    oppControlSquares[in_ver - 1][in_hor].push_back(piece);
                }
                if (in_ver < 7)
                {
                    if (in_hor > 0)
                        oppControlSquares[in_ver + 1][in_hor - 1].push_back(piece);
                    if (in_hor < 7)
                        oppControlSquares[in_ver + 1][in_hor + 1].push_back(piece);
                    oppControlSquares[in_ver + 1][in_hor].push_back(piece);
                }
                if (in_hor > 0)
                    oppControlSquares[in_ver][in_hor - 1].push_back(piece);
                if (in_hor < 7)
                    oppControlSquares[in_ver][in_hor + 1].push_back(piece);
            }
        }
    }
    return oppControlSquares;
}

void Moves::checkPinned()
{
    for (auto &piece : oppPieces)
    {
        // Cycle over all opponent pieces that are in line of sight of the king
        if (piece.type == 'Q' || piece.type == 'q' || piece.type == 'B' || piece.type == 'b' || piece.type == 'R' || piece.type == 'r')
        {
            // Find if any of our piece(s) are on the straight line
            if (!(piece.i - piece.j == king_square.first - king_square.second || piece.i + piece.j == king_square.first + king_square.second || piece.i == king_square.first || piece.j == king_square.second))
            {
                continue;
            }
            int i_inc = 0, j_inc = 0;
            if (piece.i > king_square.first)
                i_inc = -1;
            else if (piece.i < king_square.first)
                i_inc = 1;
            if (piece.j > king_square.second)
                j_inc = -1;
            else if (piece.j < king_square.second)
                j_inc = 1;
            int x = piece.i + i_inc, y = piece.j + j_inc;
            int pinx = -1, piny = -1, resx = -1, resy = -1;
            while (x != king_square.first || y != king_square.second)
            {
                int sd = piece_type(board[x][y], turn);
                if (sd == -1)
                {
                    pinx = -1, piny = -1;
                    break;
                }
                if (sd == 1)
                {
                    if (pinx != -1)
                    {
                        pinx = -1;
                        piny = -1;
                        break;
                    }
                    else
                    {
                        if (j_inc == 0)
                        {
                            if (board[x][y] == 'b' || board[x][y] == 'B' || board[x][y] == 'p' || board[x][y] == 'P' || board[x][y] == 'n' || board[x][y] == 'N')
                            {
                                pinx = x, piny = y;
                            }
                            else
                            {
                                resx = x, resy = y;
                                break;
                            }
                        }
                        else if (i_inc == 0)
                        {
                            if (board[x][y] == 'b' || board[x][y] == 'B' || board[x][y] == 'n' || board[x][y] == 'N')
                            {
                                pinx = x, piny = y;
                            }
                            else
                            {
                                resx = x, resy = y;
                                break;
                            }
                        }
                        else
                        {
                            if (board[x][y] == 'r' || board[x][y] == 'R' || board[x][y] == 'n' || board[x][y] == 'N')
                            {
                                pinx = x, piny = y;
                            }
                            else if (board[x][y] == 'P' || board[x][y] == 'p')
                            {
                                if (x == piece.i + i_inc && y == piece.j + j_inc)
                                {
                                    resx = x, resy = y;
                                    break;
                                }
                                else
                                {
                                    pinx = x, piny = y;
                                }
                            }
                            else
                            {
                                resx = x, resy = y;
                                break;
                            }
                        }
                    }
                }
                x += i_inc;
                y += j_inc;
            }
            if (pinx != -1)
            {
                for (auto &ownpiece : pieces)
                {
                    if (ownpiece.i == pinx && ownpiece.j == piny)
                    {
                        ownpiece.setPin(piece.i, piece.j);
                    }
                }
            }
            if (resx != -1)
            {
                for (auto &ownpiece : pieces)
                {
                    if (ownpiece.i == resx && ownpiece.j == resy)
                    {
                        ownpiece.setRes(piece.i, piece.j);
                    }
                }
            }
        }
    }
}

void Moves::checkOppPinned()
{
    for (auto &piece : pieces)
    {
        // Cycle over all opponent pieces that are in line of sight of the king
        if (piece.type == 'Q' || piece.type == 'q' || piece.type == 'B' || piece.type == 'b' || piece.type == 'R' || piece.type == 'r')
        {
            // Find if any of our piece(s) are on the straight line
            if (!(piece.i - piece.j == oppKing_square.first - oppKing_square.second || piece.i + piece.j == oppKing_square.first + oppKing_square.second || piece.i == oppKing_square.first || piece.j == oppKing_square.second))
            {
                continue;
            }
            int i_inc = 0, j_inc = 0;
            if (piece.i > oppKing_square.first)
                i_inc = -1;
            else if (piece.i < oppKing_square.first)
                i_inc = 1;
            if (piece.j > oppKing_square.second)
                j_inc = -1;
            else if (piece.j < oppKing_square.second)
                j_inc = 1;
            int x = piece.i + i_inc, y = piece.j + j_inc;
            int pinx = -1, piny = -1, resx = -1, resy = -1;
            while (x != oppKing_square.first || y != oppKing_square.second)
            {
                int sd = piece_type(board[x][y], turn);
                if (sd == -1)
                {
                    pinx = -1, piny = -1;
                    break;
                }
                if (sd == 1)
                {
                    if (pinx != -1)
                    {
                        pinx = -1;
                        piny = -1;
                        break;
                    }
                    else
                    {
                        if (j_inc == 0)
                        {
                            if (board[x][y] == 'b' || board[x][y] == 'B' || board[x][y] == 'p' || board[x][y] == 'P' || board[x][y] == 'n' || board[x][y] == 'N')
                            {
                                pinx = x, piny = y;
                            }
                            else
                            {
                                resx = x, resy = y;
                                break;
                            }
                        }
                        else if (i_inc == 0)
                        {
                            if (board[x][y] == 'b' || board[x][y] == 'B' || board[x][y] == 'n' || board[x][y] == 'N')
                            {
                                pinx = x, piny = y;
                            }
                            else
                            {
                                resx = x, resy = y;
                                break;
                            }
                        }
                        else
                        {
                            if (board[x][y] == 'r' || board[x][y] == 'R' || board[x][y] == 'n' || board[x][y] == 'N')
                            {
                                pinx = x, piny = y;
                            }
                            else if (board[x][y] == 'P' || board[x][y] == 'p')
                            {
                                if (x == piece.i + i_inc && y == piece.j + j_inc)
                                {
                                    resx = x, resy = y;
                                    break;
                                }
                                else
                                {
                                    pinx = x, piny = y;
                                }
                            }
                            else
                            {
                                resx = x, resy = y;
                                break;
                            }
                        }
                    }
                }
                x += i_inc;
                y += j_inc;
            }
            if (pinx != -1)
            {
                for (auto &ownpiece : oppPieces)
                {
                    if (ownpiece.i == pinx && ownpiece.j == piny)
                    {
                        ownpiece.setPin(piece.i, piece.j);
                    }
                }
            }
            if (resx != -1)
            {
                for (auto &ownpiece : oppPieces)
                {
                    if (ownpiece.i == resx && ownpiece.j == resy)
                    {
                        ownpiece.setRes(piece.i, piece.j);
                    }
                }
            }
        }
    }
}

vector<string> Moves::valid_Moves(bool flag)
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
        vector<string> illegalSquares;
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
                while (x != king_square.first || y != king_square.second)
                {
                    int sd = piece_type(board[x][y], turn);
                    if (sd <= 0)
                    {
                        for (auto p : controlSquares[x][y])
                        {
                            if (p.pinned || p.type == 'K' || p.type == 'k' || p.pinned)
                                continue;
                            string str;
                            str.push_back(p.type);
                            str += ijs(piece.i, piece.j);
                            if (sd == 0)
                                str += ijs(x, y);
                            else
                                str += "x" + ijs(x, y);
                            validMoves.push_back(str);
                        }
                    }
                    x += i_inc;
                    y += j_inc;
                }
                if (x + i_inc >= 0 && x + i_inc <= 7 && y + j_inc >= 0 && y + j_inc <= 7)
                {
                    illegalSquares.push_back(ijs(x + i_inc, y + j_inc));
                }
            }
            else if (piece.type == 'N' || piece.type == 'n') // Horsey
            {
                int x = piece.i, y = piece.j;
                for (auto p : controlSquares[x][y])
                {
                    if (p.type == 'K' || p.type == 'k' || p.pinned)
                        continue;
                    string str;
                    str.push_back(p.type);
                    str += ijs(piece.i, piece.j);
                    str += "x" + ijs(x, y);
                    validMoves.push_back(str);
                }
            }
        }
        // Now we'll see if the King can move
        int in_ver = king_square.first, in_hor = king_square.second;
        string king = "K";
        if (turn)
            king = "k";
        if (in_ver > 0)
        {
            if (in_hor > 0 && oppControlSquares[in_ver - 1][in_hor - 1].size() == 0)
            {
                if (piece_type(board[in_ver - 1][in_hor - 1], turn) == 0)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver - 1, in_hor - 1));
                else if (piece_type(board[in_ver - 1][in_hor - 1], turn) == -1)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver - 1, in_hor - 1));
            }
            if (in_hor < 7 && oppControlSquares[in_ver - 1][in_hor + 1].size() == 0)
            {
                if (piece_type(board[in_ver - 1][in_hor + 1], turn) == 0)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver - 1, in_hor + 1));
                else if (piece_type(board[in_ver - 1][in_hor + 1], turn) == -1)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver - 1, in_hor + 1));
            }
            if (oppControlSquares[in_ver - 1][in_hor].size() == 0)
            {
                if (piece_type(board[in_ver - 1][in_hor], turn) == 0)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver - 1, in_hor));
                else if (piece_type(board[in_ver - 1][in_hor], turn) == -1)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver - 1, in_hor));
            }
        }
        if (in_ver < 7)
        {
            if (in_hor > 0 && oppControlSquares[in_ver + 1][in_hor - 1].size() == 0)
            {
                if (piece_type(board[in_ver + 1][in_hor - 1], turn) == 0)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver + 1, in_hor - 1));
                else if (piece_type(board[in_ver + 1][in_hor - 1], turn) == -1)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver + 1, in_hor - 1));
            }
            if (in_hor < 7 && oppControlSquares[in_ver + 1][in_hor + 1].size() == 0)
            {
                if (piece_type(board[in_ver + 1][in_hor + 1], turn) == 0)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver + 1, in_hor + 1));
                else if (piece_type(board[in_ver + 1][in_hor + 1], turn) == -1)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver + 1, in_hor + 1));
            }
            if (oppControlSquares[in_ver + 1][in_hor].size() == 0)
            {
                if (piece_type(board[in_ver + 1][in_hor], turn) == 0)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver + 1, in_hor));
                else if (piece_type(board[in_ver + 1][in_hor], turn) == -1)
                    validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver + 1, in_hor));
            }
        }
        if (in_hor > 0 && oppControlSquares[in_ver][in_hor - 1].size() == 0)
        {
            if (piece_type(board[in_ver][in_hor - 1], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver, in_hor - 1));
            else if (piece_type(board[in_ver][in_hor - 1], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver, in_hor - 1));
        }
        if (in_hor < 7 && oppControlSquares[in_ver][in_hor + 1].size() == 0)
        {
            if (piece_type(board[in_ver][in_hor + 1], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver, in_hor + 1));
            else if (piece_type(board[in_ver][in_hor + 1], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver, in_hor + 1));
        }
        // Illegal King Moves to be checked
        vector<string> vmc;
        for (auto mv : validMoves)
        {
                string square;
                square.push_back(mv[mv.length() - 2]);
                square.push_back(mv[mv.length() - 1]);
                bool f = 0;
                for (auto sq : illegalSquares)
                {
                    if (sq == square)
                    {
                        f = 1;
                        break;
                    }
                }
                if (!f) vmc.push_back(mv);
        }
        return vmc;
    }

    // If no check, then compute valid moves of King that could include Castling (see for checks)
    // King Moves
    int in_ver = king_square.first, in_hor = king_square.second;
    string king = "K";
    if (turn)
        king = "k";
    if (in_ver > 0)
    {
        if (in_hor > 0 && oppControlSquares[in_ver - 1][in_hor - 1].size() == 0)
        {
            if (piece_type(board[in_ver - 1][in_hor - 1], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver - 1, in_hor - 1));
            else if (piece_type(board[in_ver - 1][in_hor - 1], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver - 1, in_hor - 1));
        }
        if (in_hor < 7 && oppControlSquares[in_ver - 1][in_hor + 1].size() == 0)
        {
            if (piece_type(board[in_ver - 1][in_hor + 1], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver - 1, in_hor + 1));
            else if (piece_type(board[in_ver - 1][in_hor + 1], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver - 1, in_hor + 1));
        }
        if (oppControlSquares[in_ver - 1][in_hor].size() == 0)
        {
            if (piece_type(board[in_ver - 1][in_hor], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver - 1, in_hor));
            else if (piece_type(board[in_ver - 1][in_hor], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver - 1, in_hor));
        }
    }
    if (in_ver < 7)
    {
        if (in_hor > 0 && oppControlSquares[in_ver + 1][in_hor - 1].size() == 0)
        {
            if (piece_type(board[in_ver + 1][in_hor - 1], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver + 1, in_hor - 1));
            else if (piece_type(board[in_ver + 1][in_hor - 1], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver + 1, in_hor - 1));
        }
        if (in_hor < 7 && oppControlSquares[in_ver + 1][in_hor + 1].size() == 0)
        {
            if (piece_type(board[in_ver + 1][in_hor + 1], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver + 1, in_hor + 1));
            else if (piece_type(board[in_ver + 1][in_hor + 1], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver + 1, in_hor + 1));
        }
        if (oppControlSquares[in_ver + 1][in_hor].size() == 0)
        {
            if (piece_type(board[in_ver + 1][in_hor], turn) == 0)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver + 1, in_hor));
            else if (piece_type(board[in_ver + 1][in_hor], turn) == -1)
                validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver + 1, in_hor));
        }
    }
    if (in_hor > 0 && oppControlSquares[in_ver][in_hor - 1].size() == 0)
    {
        if (piece_type(board[in_ver][in_hor - 1], turn) == 0)
            validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver, in_hor - 1));
        else if (piece_type(board[in_ver][in_hor - 1], turn) == -1)
            validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver, in_hor - 1));
    }
    if (in_hor < 7 && oppControlSquares[in_ver][in_hor + 1].size() == 0)
    {
        if (piece_type(board[in_ver][in_hor + 1], turn) == 0)
            validMoves.push_back(king + ijs(king_square.first, king_square.second) + ijs(in_ver, in_hor + 1));
        else if (piece_type(board[in_ver][in_hor + 1], turn) == -1)
            validMoves.push_back(king + ijs(king_square.first, king_square.second) + "x" + ijs(in_ver, in_hor + 1));
    }
    if (turn)
    {
        if (bck)
        {
            if (board[0][5] == '.' && board[0][6] == '.' && board[0][4] == 'k' && board[0][7] == 'r')
            {
                if (oppControlSquares[0][5].size() == 0 && oppControlSquares[0][6].size() == 0)
                {
                    validMoves.push_back("o-o");
                }
            }
        }
        else if (bcq)
        {
            if (board[0][2] == '.' && board[0][3] == '.' && board[0][1] == '.' && board[0][4] == 'k' && board[0][0] == 'r')
            {
                if (oppControlSquares[0][1].size() == 0 && oppControlSquares[0][2].size() == 0 && oppControlSquares[0][3].size() == 0)
                {
                    validMoves.push_back("o-o-o");
                }
            }
        }
    }
    else
    {
        if (wck)
        {
            if (board[7][5] == '.' && board[7][6] == '.' && board[7][4] == 'K' && board[7][7] == 'R')
            {
                if (oppControlSquares[7][5].size() == 0 && oppControlSquares[7][6].size() == 0)
                {
                    validMoves.push_back("O-O");
                }
            }
        }
        else if (wcq)
        {
            if (board[7][2] == '.' && board[7][3] == '.' && board[7][1] == '.' && board[7][4] == 'K' && board[7][0] == 'R')
            {
                if (oppControlSquares[7][1].size() == 0 && oppControlSquares[7][2].size() == 0 && oppControlSquares[7][3].size() == 0)
                {
                    validMoves.push_back("O-O-O");
                }
            }
        }
    }
    // For all pieces, attack Squares except the squares in which own piece is there
    // ^^ except the King
    // Make sure the piece is not pinned
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int sd = piece_type(board[i][j], turn);
            if (sd == 1)
                continue;
            if (controlSquares[i][j].size() > 0)
            {
                for (auto piece : controlSquares[i][j])
                {
                    if (piece.type == 'K' || piece.type == 'k')
                    {
                        continue;
                    }
                    string str;
                    str.push_back(piece.type);
                    str += ijs(piece.i, piece.j);
                    if (sd == -1)
                    {
                        str += "x" + ijs(i, j);
                    }
                    else
                    {
                        if (piece.type == 'P' || piece.type == 'p')
                            continue;
                        str += ijs(i, j);
                    }
                    validMoves.push_back(str);
                }
            }
        }
    }
    // Pawn Moves
    for (auto piece : pieces)
    {
        if (piece.type == 'P' || piece.type == 'p')
        {
            if (turn == 0)
            {
                if (piece.i == 1 && board[0][piece.j] == '.')
                {
                    validMoves.push_back("P" + ijs(1, piece.j) + "PP"); // pp = pawn promotion
                }
                else if (piece.i > 1)
                {
                    if (board[piece.i - 1][piece.j] == '.')
                    {
                        validMoves.push_back("P" + ijs(piece.i, piece.j) + ijs(piece.i - 1, piece.j));
                    }
                }
                if (piece.i == 6)
                {
                    if (board[piece.i - 2][piece.j] == '.' && board[piece.i - 1][piece.j] == '.')
                    {
                        validMoves.push_back("P" + ijs(piece.i, piece.j) + "Z" + ijs(piece.i - 2, piece.j));
                    }
                }
            }
            if (turn == 1)
            {
                if (piece.i == 6 && board[7][piece.j] == '.')
                {
                    validMoves.push_back("p" + ijs(6, piece.j) + "pp"); // pp = pawn promotion
                }
                else if (piece.i < 6)
                {
                    if (board[piece.i + 1][piece.j] == '.')
                    {
                        validMoves.push_back("p" + ijs(piece.i, piece.j) + ijs(piece.i + 1, piece.j));
                    }
                }
                if (piece.i == 1)
                {
                    if (board[piece.i + 2][piece.j] == '.' && board[piece.i + 1][piece.j] == '.')
                    {
                        validMoves.push_back("p" + ijs(piece.i, piece.j) + "z"+ ijs(piece.i + 2, piece.j));
                    }
                }
            }
        }
    }
    // If EnPassant is possible then include that
    if (isEnPassant)
    {
        if (turn == 0)
        {
            int x1 = sij(epSquare).first, y1 = sij(epSquare).second;
            if (y1 > 0 && board[x1+1][y1-1] == 'P') {
                validMoves.push_back("P" + ijs(x1+1, y1-1) + "Y" + epSquare);
            }
            if (y1 < 7 && board[x1+1][y1+1] == 'P') validMoves.push_back("P" + ijs(x1+1, y1+1) + "Y" + epSquare);
        }
        else if (turn == 1)
        {
            int x1 = sij(epSquare).first, y1 = sij(epSquare).second;
            if (y1 > 0 && board[x1-1][y1-1] == 'p') validMoves.push_back("p" + ijs(x1-1, y1-1) + "y" + epSquare);
            if (y1 < 7 && board[x1-1][y1+1] == 'p') validMoves.push_back("p" + ijs(x1-1, y1+1) + "y" + epSquare);
        }
    }
    return validMoves;
}

vector<string> Moves::valid_oppMoves()
{
    vector<string> validMoves;
    // First writing function assuming pov of white

    // First check if the King is in Check
    // If yes, then see if anyone can defend the king
    // If no one can defend the king, see if the king has to move

    if (controlSquares[oppKing_square.first][oppKing_square.second].size() > 0)
    {
        // King is in Check
        // First we'll see if any piece can protect it
        vector<string> illegalSquares;
        for (auto piece : controlSquares[oppKing_square.first][oppKing_square.second])
        {
            // Cycle over all opponent pieces that are putting king in check
            if (piece.type == 'Q' || piece.type == 'q' || piece.type == 'B' || piece.type == 'b' || piece.type == 'R' || piece.type == 'r' || piece.type == 'P' || piece.type == 'p')
            {
                // Find if any of our piece(s) can come on the straight line
                // from the K to Q including Q's square
                int i_inc = 0, j_inc = 0;
                if (piece.i > oppKing_square.first)
                    i_inc = -1;
                else if (piece.i < oppKing_square.first)
                    i_inc = 1;
                if (piece.j > oppKing_square.second)
                    j_inc = -1;
                else if (piece.j < oppKing_square.second)
                    j_inc = 1;
                int x = piece.i, y = piece.j;
                while (x != oppKing_square.first || y != oppKing_square.second)
                {
                    int sd = piece_type(board[x][y], !turn);
                    if (sd <= 0)
                    {
                        for (auto p : oppControlSquares[x][y])
                        {
                            if (p.pinned || p.type == 'K' || p.type == 'k' || p.pinned)
                                continue;
                            string str;
                            str.push_back(p.type);
                            str += ijs(piece.i, piece.j);
                            if (sd == 0)
                                str += ijs(x, y);
                            else
                                str += "x" + ijs(x, y);
                            validMoves.push_back(str);
                        }
                    }
                    x += i_inc;
                    y += j_inc;
                }
                if (x + i_inc >= 0 && x + i_inc <= 7 && y + j_inc >= 0 && y + j_inc <= 7)
                {
                    illegalSquares.push_back(ijs(x + i_inc, y + j_inc));
                }
            }
            else if (piece.type == 'N' || piece.type == 'n') // Horsey
            {
                int x = piece.i, y = piece.j;
                for (auto p : oppControlSquares[x][y])
                {
                    if (p.type == 'K' || p.type == 'k' || p.pinned)
                        continue;
                    string str;
                    str.push_back(p.type);
                    str += ijs(piece.i, piece.j);
                    str += "x" + ijs(x, y);
                    validMoves.push_back(str);
                }
            }
        }
        // Now we'll see if the King can move
        int in_ver = oppKing_square.first, in_hor = oppKing_square.second;
        string king = "k";
        if (turn)
            king = "K";
        if (in_ver > 0)
        {
            if (in_hor > 0 && controlSquares[in_ver - 1][in_hor - 1].size() == 0)
            {
                if (piece_type(board[in_ver - 1][in_hor - 1], !turn) == 0)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver - 1, in_hor - 1));
                else if (piece_type(board[in_ver - 1][in_hor - 1], !turn) == -1)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver - 1, in_hor - 1));
            }
            if (in_hor < 7 && controlSquares[in_ver - 1][in_hor + 1].size() == 0)
            {
                if (piece_type(board[in_ver - 1][in_hor + 1], !turn) == 0)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver - 1, in_hor + 1));
                else if (piece_type(board[in_ver - 1][in_hor + 1], !turn) == -1)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver - 1, in_hor + 1));
            }
            if (controlSquares[in_ver - 1][in_hor].size() == 0)
            {
                if (piece_type(board[in_ver - 1][in_hor], !turn) == 0)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver - 1, in_hor));
                else if (piece_type(board[in_ver - 1][in_hor], !turn) == -1)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver - 1, in_hor));
            }
        }
        if (in_ver < 7)
        {
            if (in_hor > 0 && controlSquares[in_ver + 1][in_hor - 1].size() == 0)
            {
                if (piece_type(board[in_ver + 1][in_hor - 1], !turn) == 0)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver + 1, in_hor - 1));
                else if (piece_type(board[in_ver + 1][in_hor - 1], !turn) == -1)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver + 1, in_hor - 1));
            }
            if (in_hor < 7 && controlSquares[in_ver + 1][in_hor + 1].size() == 0)
            {
                if (piece_type(board[in_ver + 1][in_hor + 1], !turn) == 0)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver + 1, in_hor + 1));
                else if (piece_type(board[in_ver + 1][in_hor + 1], !turn) == -1)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver + 1, in_hor + 1));
            }
            if (controlSquares[in_ver + 1][in_hor].size() == 0)
            {
                if (piece_type(board[in_ver + 1][in_hor], !turn) == 0)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver + 1, in_hor));
                else if (piece_type(board[in_ver + 1][in_hor], !turn) == -1)
                    validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver + 1, in_hor));
            }
        }
        if (in_hor > 0 && controlSquares[in_ver][in_hor - 1].size() == 0)
        {
            if (piece_type(board[in_ver][in_hor - 1], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver, in_hor - 1));
            else if (piece_type(board[in_ver][in_hor - 1], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver, in_hor - 1));
        }
        if (in_hor < 7 && controlSquares[in_ver][in_hor + 1].size() == 0)
        {
            if (piece_type(board[in_ver][in_hor + 1], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver, in_hor + 1));
            else if (piece_type(board[in_ver][in_hor + 1], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver, in_hor + 1));
        }
        // Illegal King Moves to be checked
        if (validMoves.size() != 0)
        {
            auto it = validMoves.begin();
            while (it != validMoves.end())
            {
                string mv = *it;
                int index = it - validMoves.begin();
                it++;
                string square;
                square.push_back(mv[mv.length() - 2]);
                square.push_back(mv[mv.length() - 1]);
                for (auto sq : illegalSquares)
                {
                    if (sq == square)
                    {
                        validMoves.erase(validMoves.begin() + index);
                        break;
                    }
                }
            }
        }
        return validMoves;
    }

    // If no check, then compute valid moves of King that could include Castling (see for checks)
    // King Moves
    int in_ver = oppKing_square.first, in_hor = oppKing_square.second;
    string king = "k";
    if (turn)
        king = "K";
    if (in_ver > 0)
    {
        if (in_hor > 0 && controlSquares[in_ver - 1][in_hor - 1].size() == 0)
        {
            if (piece_type(board[in_ver - 1][in_hor - 1], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver - 1, in_hor - 1));
            else if (piece_type(board[in_ver - 1][in_hor - 1], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver - 1, in_hor - 1));
        }
        if (in_hor < 7 && controlSquares[in_ver - 1][in_hor + 1].size() == 0)
        {
            if (piece_type(board[in_ver - 1][in_hor + 1], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver - 1, in_hor + 1));
            else if (piece_type(board[in_ver - 1][in_hor + 1], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver - 1, in_hor + 1));
        }
        if (controlSquares[in_ver - 1][in_hor].size() == 0)
        {
            if (piece_type(board[in_ver - 1][in_hor], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver - 1, in_hor));
            else if (piece_type(board[in_ver - 1][in_hor], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver - 1, in_hor));
        }
    }
    if (in_ver < 7)
    {
        if (in_hor > 0 && controlSquares[in_ver + 1][in_hor - 1].size() == 0)
        {
            if (piece_type(board[in_ver + 1][in_hor - 1], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver + 1, in_hor - 1));
            else if (piece_type(board[in_ver + 1][in_hor - 1], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver + 1, in_hor - 1));
        }
        if (in_hor < 7 && controlSquares[in_ver + 1][in_hor + 1].size() == 0)
        {
            if (piece_type(board[in_ver + 1][in_hor + 1], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver + 1, in_hor + 1));
            else if (piece_type(board[in_ver + 1][in_hor + 1], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver + 1, in_hor + 1));
        }
        if (controlSquares[in_ver + 1][in_hor].size() == 0)
        {
            if (piece_type(board[in_ver + 1][in_hor], !turn) == 0)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver + 1, in_hor));
            else if (piece_type(board[in_ver + 1][in_hor], !turn) == -1)
                validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver + 1, in_hor));
        }
    }
    if (in_hor > 0 && controlSquares[in_ver][in_hor - 1].size() == 0)
    {
        if (piece_type(board[in_ver][in_hor - 1], !turn) == 0)
            validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver, in_hor - 1));
        else if (piece_type(board[in_ver][in_hor - 1], !turn) == -1)
            validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver, in_hor - 1));
    }
    if (in_hor < 7 && controlSquares[in_ver][in_hor + 1].size() == 0)
    {
        if (piece_type(board[in_ver][in_hor + 1], !turn) == 0)
            validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + ijs(in_ver, in_hor + 1));
        else if (piece_type(board[in_ver][in_hor + 1], !turn) == -1)
            validMoves.push_back(king + ijs(oppKing_square.first, oppKing_square.second) + "x" + ijs(in_ver, in_hor + 1));
    }
    if (!turn)
    {
        if (bck)
        {
            if (board[0][5] == '.' && board[0][6] == '.' && board[0][4] == 'k' && board[0][7] == 'r')
            {
                if (controlSquares[0][5].size() == 0 && controlSquares[0][6].size() == 0)
                {
                    validMoves.push_back("o-o");
                }
            }
        }
        else if (bcq)
        {
            if (board[0][2] == '.' && board[0][3] == '.' && board[0][1] == '.' && board[0][4] == 'k' && board[0][0] == 'r')
            {
                if (controlSquares[0][1].size() == 0 && controlSquares[0][2].size() == 0 && controlSquares[0][3].size() == 0)
                {
                    validMoves.push_back("o-o-o");
                }
            }
        }
    }
    else
    {
        if (wck)
        {
            if (board[7][5] == '.' && board[7][6] == '.' && board[7][4] == 'K' && board[7][7] == 'R')
            {
                if (controlSquares[7][5].size() == 0 && controlSquares[7][6].size() == 0)
                {
                    validMoves.push_back("O-O");
                }
            }
        }
        else if (wcq)
        {
            if (board[7][2] == '.' && board[7][3] == '.' && board[7][1] == '.' && board[7][4] == 'K' && board[7][0] == 'R')
            {
                if (controlSquares[7][1].size() == 0 && controlSquares[7][2].size() == 0 && controlSquares[7][3].size() == 0)
                {
                    validMoves.push_back("O-O-O");
                }
            }
        }
    }
    // For all pieces, attack Squares except the squares in which own piece is there
    // ^^ except the King
    // Make sure the piece is not pinned
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            int sd = piece_type(board[i][j], !turn);
            if (sd == 1)
                continue;
            if (oppControlSquares[i][j].size() > 0)
            {
                for (auto piece : oppControlSquares[i][j])
                {
                    if (piece.type == 'K' || piece.type == 'k')
                    {
                        continue;
                    }
                    string str;
                    str.push_back(piece.type);
                    str += ijs(piece.i, piece.j);
                    if (sd == -1)
                    {
                        str += "x" + ijs(i, j);
                    }
                    else
                    {
                        if (piece.type == 'P' || piece.type == 'p')
                            continue;
                        str += ijs(i, j);
                    }
                    validMoves.push_back(str);
                }
            }
        }
    }
    // Pawn Moves
    for (auto piece : oppPieces)
    {
        if (piece.type == 'P' || piece.type == 'p')
        {
            if (turn == 1) // !turn
            {
                if (piece.i == 1 && board[0][piece.j] == '.')
                {
                    validMoves.push_back("P" + ijs(1, piece.j) + "pp"); // pp = pawn promotion
                }
                else if (piece.i > 1)
                {
                    if (board[piece.i - 1][piece.j] == '.')
                    {
                        validMoves.push_back("P" + ijs(piece.i, piece.j) + ijs(piece.i - 1, piece.j));
                    }
                }
                if (piece.i == 6)
                {
                    if (board[piece.i - 2][piece.j] == '.' && board[piece.i - 1][piece.j] == '.')
                    {
                        validMoves.push_back("P" + ijs(piece.i, piece.j) + "z" + ijs(piece.i - 2, piece.j));
                    }
                }
            }
            if (turn == 0) // !turn
            {
                if (piece.i == 6 && board[7][piece.j] == '.')
                {
                    validMoves.push_back("p" + ijs(6, piece.j) + "pp"); // pp = pawn promotion
                }
                else if (piece.i < 6)
                {
                    if (board[piece.i + 1][piece.j] == '.')
                    {
                        validMoves.push_back("p" + ijs(piece.i, piece.j) + ijs(piece.i + 1, piece.j));
                    }
                }
                if (piece.i == 1)
                {
                    if (board[piece.i + 2][piece.j] == '.' && board[piece.i + 1][piece.j] == '.')
                    {
                        validMoves.push_back("p" + ijs(piece.i, piece.j) + "z" + ijs(piece.i + 2, piece.j));
                    }
                }
            }
        }
    }
    return validMoves;
}

vector<Piece> Moves::return_trappedPieces()
{
    vector<Piece> trappedPieces;

    map<string, vector<pair<int, int>>> piece_moves_mp;
    vector<string> all_moves = valid_Moves();

    for (string move : all_moves)
    {
        string pc = move.substr(0, 3);
        if (pc[0] == 'P' || pc[0] == 'p' || pc[0] == 'o' || pc[0] == 'O')
            continue;
        pair<int, int> ij = sij(move.substr(move.length() - 2, 2));
        piece_moves_mp[pc].push_back(ij);
    }
    for (auto piece_moves : piece_moves_mp)
    {
        char pc = piece_moves.first[0];
        if (piece_type(pc, turn) == 1)
        {
            bool flag = true;
            for (int k = 0; k < piece_moves.second.size(); k++)
            {
                int i = piece_moves.second[k].first;
                int j = piece_moves.second[k].second;
                if (oppControlSquares[i][j].size() == 0)
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                pair<int, int> ij = sij(piece_moves.first.substr(1, 2));
                Piece trapped_piece(pc, ij.first, ij.second);
                trappedPieces.push_back(trapped_piece);
            }
        }
    }
    if (turn)
        return black_trapped_pieces = trappedPieces;
    return white_trapped_pieces = trappedPieces;
}

vector<Piece> Moves::return_trappedOppPieces()
{
    vector<Piece> oppTrappedPieces;

    map<string, vector<pair<int, int>>> piece_moves_mp;
    vector<string> all_moves = valid_oppMoves();

    for (string move : all_moves)
    {
        string pc = move.substr(0, 3);
        if (pc[0] == 'P' || pc[0] == 'p' || pc[0] == 'o' || pc[0] == 'O')
            continue;
        pair<int, int> ij = sij(move.substr(move.length() - 2, 2));
        piece_moves_mp[pc].push_back(ij);
    }
    for (auto piece_moves : piece_moves_mp)
    {
        char pc = piece_moves.first[0];
        if (piece_type(pc, turn) == -1)
        {
            bool flag = true;
            for (int k = 0; k < piece_moves.second.size(); k++)
            {
                int i = piece_moves.second[k].first;
                int j = piece_moves.second[k].second;
                if (controlSquares[i][j].size() == 0)
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                pair<int, int> ij = sij(piece_moves.first.substr(1, 2));
                Piece trapped_piece(pc, ij.first, ij.second);
                oppTrappedPieces.push_back(trapped_piece);
            }
        }
    }
    if (!turn)
        return black_trapped_pieces = oppTrappedPieces;
    return white_trapped_pieces = oppTrappedPieces;
}