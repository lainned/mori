#include <stdbool.h>
#include <stdint.h>
#include "gen.h"
#include "board.h"
uint64_t king_move_board[BOARD_SIZE];
uint64_t knight_move_board[BOARD_SIZE];

void init_king(void){
    for(int i = 0; i < BOARD_SIZE; i++){
        uint64_t king = (1ULL << i);
        uint64_t board = 0;
        board |= (king & (~H_FILE)) << 1;
        board |= (king & (~A_FILE)) >> 1;
        board |= (king & (~RANK_8)) << 8;
        board |= (king & (~RANK_1)) >> 8;

        board |= (king & (~(RANK_8|H_FILE))) << 9;
        board |= (king & (~(RANK_8|A_FILE))) << 7;
        board |= (king & (~(RANK_1|H_FILE))) >> 7;
        board |= (king & (~(RANK_1|A_FILE))) >> 9;
        king_move_board[i] = board;
    }
}

void init_knight(void){
    for(int i = 0; i < BOARD_SIZE; i++){
        uint64_t board = 0;
        uint64_t knight = (1ULL << i);

        board |= (knight & (~(G_FILE|H_FILE|RANK_8))) << 10;
        board |= (knight & (~(H_FILE|RANK_7|RANK_8))) << 17;
        board |= (knight & (~(A_FILE|RANK_7|RANK_8))) << 15;
        board |= (knight & (~(A_FILE|B_FILE|RANK_8))) << 6;
        board |= (knight & (~(A_FILE|B_FILE|RANK_1))) >> 10;
        board |= (knight & (~(A_FILE|RANK_2|RANK_1))) >> 17;
        board |= (knight & (~(H_FILE|RANK_2|RANK_1))) >> 15;
        board |= (knight & (~(G_FILE|H_FILE|RANK_1))) >> 6;

        knight_move_board[i] = board;
    }
}

bool is_square_attacked(const Board board, uint8_t square){

}

void generate_king_moves(const Board board, Move* moves, uint8_t* len){
    uint64_t king_board = board.white_turn ? board.bitboard[WHITE_KINGS] : board.bitboard[BLACK_KINGS];
    Type color = board.white_turn ? WHITE : BLACK;
    Type enemy = board.white_turn ? BLACK : WHITE;
    int square = __builtin_ctzll(king_board);
    while(square){
        uint64_t move_board = king_move_board[square];
        move_board &= (~board.bitboard[color]);
        while(move_board){
            int to_square = __builtin_ctzll(move_board);
            Move new_move;
            new_move.castling = CASTLING_NONE;
            new_move.from = square;
            new_move.to = to_square;
            new_move.legality = NOT_VALIDATED;
            new_move.piece = board.white_turn ? 'K' : 'k';
            moves[(*len)++] = new_move;
            move_board &= (move_board - 1);
        }
        king_board &= (king_board - 1);
        square = __builtin_ctzll(king_board);
    }

}

void generate_knight_moves(const Board board, Move* moves, uint8_t* len){

}

void generate_legal_moves(const Board board, Move* moves, uint8_t* len){
    
}
