#pragma once
typedef struct {
    char pieces[71];
    char active_color;
    char castling[4];
    char en_passant[2];
    int half_move;
    int full_move;
} FEN;