#pragma once
#include "board.h"

typedef struct FEN {
    char pieces[71];
    char active_color;
    char castling[4];
    char en_passant[2];
    int half_move;
    int full_move;
} FEN;

void fen_to_board(FEN* fen, Board* board);
void board_to_fen(Board* board, FEN* fen);
