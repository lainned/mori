#pragma once
#include <stdint.h>
#include "board.h"

#define BOARD_SIZE 64
#define A_FILE 0x101010101010101ULL
#define B_FILE 0x202020202020202ULL
#define C_FILE 0x404040404040404ULL
#define D_FILE 0x808080808080808ULL
#define E_FILE 0x1010101010101010ULL
#define F_FILE 0x2020202020202020ULL
#define G_FILE 0x4040404040404040ULL
#define H_FILE 0x8080808080808080ULL
#define RANK_1 0xFFULL
#define RANK_2 0xFF00ULL
#define RANK_3 0xFF0000ULL
#define RANK_4 0xFF000000ULL
#define RANK_5 0xFF00000000ULL
#define RANK_6 0xFF0000000000ULL
#define RANK_7 0xFF000000000000ULL
#define RANK_8 0xFF00000000000000ULL

#define LEGAL 0
#define ILLEGAL 1
#define NOT_VALIDATED 2
#define CASTLING_NONE 0
#define CASTLING_KING_SIDE 1
#define CASTLING_QUEEN_SIDE 2
#define PROMOTION_NONE 0
#define PROMOTION_KNIGHT 1
#define PROMOTION_BISHOP 2
#define PROMOTION_ROOK 3
#define PROMOTION_QUEEN 4

#define MAX_MOVE_COUNT 256

extern uint64_t king_move_board[BOARD_SIZE];
extern uint64_t knight_move_board[BOARD_SIZE];
extern uint64_t rook_move_board[BOARD_SIZE];
extern uint64_t pawn_attack_board[2][BOARD_SIZE];
extern uint64_t rook_attack_boards[64][4096];
extern uint64_t bishop_attack_boards[64][512];
typedef struct {
    uint16_t from;
    uint16_t to;
    uint16_t legality; // LEGAL, ILLEGAL O NON_VALIDATED
    uint16_t castling; // CASTLING_NONE, CASTLING_KING_SIDE OR CASTLING_QUEEN_SIDE
    uint16_t promotion; // PROMOTION_NONE,PROMOTION_KNIGHT, PROMOTION_BISHOP, PROMOTION_ROOK, PROMOTION_QUEEN
    char piece; // piece type, lower case is black, upper case is white
} Move;

void gen_init(void);
void generate_legal_moves(const Board board, Move* moves, uint16_t* len);
void make_move(Board* board, Move move);
void undo_move(Board* board, Move move);