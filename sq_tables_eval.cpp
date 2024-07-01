#include "sq_tables_eval.h"

void psTables::init_tables()
{
    gamePhase = 100;
    int p, sq;
    for (p = PAWN; p <= KING; p++)
    {
        for (sq = 0; sq < 64; sq++)
        {
            mg_table_black[p][sq] = mg_value[p] + mg_pesto_table[p][sq];
            eg_table_black[p][sq] = eg_value[p] + eg_pesto_table[p][sq];
        }
    }
    for (p = PAWN; p <= KING; p++)
    {
        for (sq = 0; sq < 64; sq++)
        {
            mg_table_white[p][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
            eg_table_white[p][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
        }
    }
}

int psTables::eval_sq_tables(const vector<vector<char>> &board)
{
        int mg[2];
        int eg[2];
        gamePhase = 0;
        int side2move = 0;

        mg[WHITE] = 0;
        mg[BLACK] = 0;
        eg[WHITE] = 0;
        eg[BLACK] = 0;

        /* evaluate each piece */
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                char pc = board[i][j];
                switch (pc)
                {
                case 'p':
                    mg[BLACK] += mg_table_black[PAWN][i * 8 + j];
                    eg[BLACK] += eg_table_black[PAWN][i * 8 + j];
                    gamePhase += gamePhaseInc[PAWN];
                    break;
                case 'n':
                    mg[BLACK] += mg_table_black[KNIGHT][i * 8 + j];
                    eg[BLACK] += eg_table_black[KNIGHT][i * 8 + j];
                    gamePhase += gamePhaseInc[KNIGHT];
                    break;
                case 'b':
                    mg[BLACK] += mg_table_black[BISHOP][i * 8 + j];
                    eg[BLACK] += eg_table_black[BISHOP][i * 8 + j];
                    gamePhase += gamePhaseInc[BISHOP];
                    break;
                case 'r':
                    mg[BLACK] += mg_table_black[ROOK][i * 8 + j];
                    eg[BLACK] += eg_table_black[ROOK][i * 8 + j];
                    gamePhase += gamePhaseInc[ROOK];
                    break;
                case 'q':
                    mg[BLACK] += mg_table_black[QUEEN][i * 8 + j];
                    eg[BLACK] += eg_table_black[QUEEN][i * 8 + j];
                    gamePhase += gamePhaseInc[QUEEN];
                    break;
                case 'k':
                    mg[BLACK] += mg_table_black[KING][i * 8 + j];
                    eg[BLACK] += eg_table_black[KING][i * 8 + j];
                    gamePhase += gamePhaseInc[KING];
                    break;
                case 'P':
                    mg[WHITE] += mg_table_black[PAWN][i * 8 + j];
                    eg[WHITE] += eg_table_black[PAWN][i * 8 + j];
                    gamePhase += gamePhaseInc[PAWN];
                    break;
                case 'N':
                    mg[WHITE] += mg_table_black[KNIGHT][i * 8 + j];
                    eg[WHITE] += eg_table_black[KNIGHT][i * 8 + j];
                    gamePhase += gamePhaseInc[KNIGHT];
                    break;
                case 'B':
                    mg[WHITE] += mg_table_black[BISHOP][i * 8 + j];
                    eg[WHITE] += eg_table_black[BISHOP][i * 8 + j];
                    gamePhase += gamePhaseInc[BISHOP];
                    break;
                case 'R':
                    mg[WHITE] += mg_table_black[ROOK][i * 8 + j];
                    eg[WHITE] += eg_table_black[ROOK][i * 8 + j];
                    gamePhase += gamePhaseInc[ROOK];
                    break;
                case 'Q':
                    mg[WHITE] += mg_table_black[QUEEN][i * 8 + j];
                    eg[WHITE] += eg_table_black[QUEEN][i * 8 + j];
                    gamePhase += gamePhaseInc[QUEEN];
                    break;
                case 'K':
                    mg[WHITE] += mg_table_black[KING][i * 8 + j];
                    eg[WHITE] += eg_table_black[KING][i * 8 + j];
                    gamePhase += gamePhaseInc[KING];
                    break;
                }
            }
        }

        /* tapered eval */
        int mgScore = mg[side2move] - mg[OTHER(side2move)];
        int egScore = eg[side2move] - eg[OTHER(side2move)];
        int mgPhase = gamePhase;
        if (mgPhase > 24)
            mgPhase = 24; /* in case of early promotion */
        int egPhase = 24 - mgPhase;
        return (mgScore * mgPhase + egScore * egPhase) / 24;
    }