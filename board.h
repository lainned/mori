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
    //char mailbox[64];
    uint64_t bitboard[TYPES];
    uint64_t en_passant;
    uint8_t white_turn;
    uint8_t white_king_castling;
    uint8_t white_queen_castling;
    uint8_t black_king_castling;
    uint8_t black_queen_castling;
    uint8_t full_move;
    uint8_t half_move;
} Board;
