#include "piece.h"

const int LONG_PIECE[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

const int L_PIECE[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
};

const int L_PIECE_FLIPPED[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 0}
};

const int SQUARE_PIECE[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

const int S_PIECE[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};

const int S_PIECE_FLIPPED[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

const int T_PIECE[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 1, 0},
    {0, 0, 0, 0}
};

piece_t* construct_piece(piece_type type, int x, int y)
{
    const int (*piece_data)[4] = NULL;
    switch (type)
    {
    case LONG:
        piece_data = LONG_PIECE;
        break;
    case L:
        piece_data = L_PIECE;
        break;
    case SQUARE:
        piece_data = SQUARE_PIECE;
        break;
    case S:
        piece_data = S_PIECE;
        break;
    case T:
        piece_data = T_PIECE;
        break;
    case L_FLIPPED:
        piece_data = L_PIECE_FLIPPED;
        break;
    case S_FLIPPED:
        piece_data = S_PIECE_FLIPPED;
        break;
    default:
        return NULL;
    }

    piece_t* piece = (piece_t*)malloc(sizeof(piece_t));
    if (piece == NULL) {
        return NULL;
    }

    piece->type = type;
    piece->x = x;
    piece->y = y;

    // Allocate memory for blocks
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (piece_data[i][j] == 1) {
                piece->blocks[i][j] = construct_block(x + i*6, y + j*6);
            } else {
                piece->blocks[i][j].x = -1;
                piece->blocks[i][j].y = -1;
            }
        }
    }

    return piece;
}

void render_piece(piece_t* piece)
{
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (piece->blocks[i][j].x != -1)
            {
                render_block(piece->blocks[i][j]);
            }
        }
    }
}

void move_piece(piece_t* piece, int x, int y)
{
    if(piece == NULL)
    {
        return;
    }
    if(x == 0 && y == 0)
    {
        return;
    }

    piece->x += x;
    piece->y += y;

    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if (piece->blocks[i][j].x != -1)
            {
                if(piece->blocks[i][j].x + x < 0 || piece->blocks[i][j].x + x > 60)
                {
                    return;
                }
                if(piece->blocks[i][j].y + y < 0 || piece->blocks[i][j].y + y > 122)
                {
                    return;
                }
                piece->blocks[i][j].x += x;
                piece->blocks[i][j].y += y;
            }
        }
    }
}

void delete_piece(piece_t* piece)
{
    free(piece);
}

void rotate_piece(piece_t* piece, block_t blocks[10][20])
{
    block_t result[PIECE_SIZE][PIECE_SIZE];
    block_t backup[PIECE_SIZE][PIECE_SIZE];

    for(int i = 0; i < PIECE_SIZE; i++)
    {
        for(int j = 0; j < PIECE_SIZE; j++)
        {
            result[j][PIECE_SIZE - i - 1].x = piece->blocks[i][j].x;
            result[j][PIECE_SIZE - i - 1].y = piece->blocks[i][j].y;

            backup[i][j].x = piece->blocks[i][j].x;
            backup[i][j].y = piece->blocks[i][j].y;
        }
    }

    for(int i = 0; i < PIECE_SIZE; i++)
    {
        for(int j = 0; j < PIECE_SIZE; j++)
        {
            piece->blocks[i][j].x = result[i][j].x;
            piece->blocks[i][j].y = result[i][j].y;
        }
    }

    for(int i = 0; i < PIECE_SIZE; i++)
    {
        for(int j = 0; j < PIECE_SIZE; j++)
        {
            if(piece->blocks[i][j].x != -1)
            {
                piece->blocks[i][j].x = piece->x + i * 6;
                piece->blocks[i][j].y = piece->y + j * 6;
            }
        }
    }

    if(!check_is_rotation_valid(piece, blocks))
    {
        for(int i = 0; i < PIECE_SIZE; i++)
        {
            for(int j = 0; j < PIECE_SIZE; j++)
            {
                piece->blocks[i][j].x = backup[i][j].x;
                piece->blocks[i][j].y = backup[i][j].y;
            }
        }
    }
}

bool check_is_rotation_valid(piece_t* piece, block_t blocks[10][20])
{
    for(int i = 0; i < PIECE_SIZE; i++)
    {
        for(int j = 0; j < PIECE_SIZE; j++)
        {
            if(piece->blocks[i][j].x != -1)
            {
                if(piece->blocks[i][j].x < 0 || piece->blocks[i][j].x > 60)
                {
                    return false;
                }
                if(piece->blocks[i][j].y < 0 || piece->blocks[i][j].y > 120)
                {
                    return false;
                }
            }
        }
    }

    return true;
}