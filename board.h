#pragma once
#include <stdint.h>
#include <stdbool.h>

#define TYPES 15

typedef enum Type {
    WHITE_PAWNS = 0,
    WHITE_ROOKS,
    WHITE_KNIGHTS,
    WHITE_BISHOPS,
    WHITE_QUEENS,
    WHITE_KINGS,
    BLACK_PAWNS,
    BLACK_ROOKS,
    BLACK_KNIGHTS,
    BLACK_BISHOPS,
    BLACK_QUEENS,
    BLACK_KINGS,
    WHITE,
    BLACK,
    BOTH
} Type;

typedef struct {
    char mailbox[64];
    uint64_t bitboard[TYPES];
    bool white_turn;
    bool white_king_castling;
    bool white_queen_castling;
    bool black_king_castling;
    bool black_queen_castling;
    bool en_passant;
    int full_move;
    int half_move;
} Board;
